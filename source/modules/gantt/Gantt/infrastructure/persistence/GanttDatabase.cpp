#include "GanttDatabase.hpp"
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"

namespace source::gantt {

GanttDatabase::GanttDatabase()
    : connection_name_(QStringLiteral("EisenPoGanttDb_") + QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

GanttDatabase::~GanttDatabase()
{
    close();
}

GanttDatabase &GanttDatabase::instance()
{
    static GanttDatabase s_instance;
    if (!s_instance.is_open()) {
        s_instance.initialize();
    }
    return s_instance;
}

Schedule GanttDatabase::load_default_schedule(QString *error_message)
{
    QList<Schedule> list = list_schedules(error_message);
    return list.isEmpty() ? Schedule() : list.first();
}

bool GanttDatabase::initialize(const QString &custom_db_path, QString *error_message)
{
    QString effective_path = custom_db_path;
    if (effective_path.isEmpty()) {
        QString dir = source::utils::obter_caminho_persistente();
        QDir().mkpath(dir);
        effective_path = dir + QStringLiteral("/eisenpo_gantt.db");
    }

    if (QSqlDatabase::contains(connection_name_)) {
        {
            QSqlDatabase old_db = QSqlDatabase::database(connection_name_, false);
            if (old_db.isOpen()) {
                old_db.close();
            }
        }
        QSqlDatabase::removeDatabase(connection_name_);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connection_name_);
    db.setDatabaseName(effective_path);

    if (!db.open()) {
        if (error_message) {
            *error_message = QStringLiteral("Falha ao abrir banco de dados SQLite (%1): %2")
                                 .arg(effective_path, db.lastError().text());
        }
        return false;
    }

    // Enable foreign keys and WAL journal mode for high performance
    QSqlQuery pragma(db);
    pragma.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));
    pragma.exec(QStringLiteral("PRAGMA journal_mode = WAL;"));
    pragma.exec(QStringLiteral("PRAGMA synchronous = NORMAL;"));

    return run_migrations(db, error_message);
}

void GanttDatabase::close()
{
    if (QSqlDatabase::contains(connection_name_)) {
        {
            QSqlDatabase db = QSqlDatabase::database(connection_name_);
            if (db.isOpen()) {
                db.close();
            }
        }
        QSqlDatabase::removeDatabase(connection_name_);
    }
}

bool GanttDatabase::is_open() const
{
    if (!QSqlDatabase::contains(connection_name_)) return false;
    return QSqlDatabase::database(connection_name_).isOpen();
}

bool GanttDatabase::run_migrations(QSqlDatabase &db, QString *error_message)
{
    // Fast-path de alto desempenho: se o esquema do banco já foi inicializado,
    // não reexecuta dezenas de instruções DDL a cada abertura.
    if (db.tables().contains(QStringLiteral("schedule_items")) &&
        db.tables().contains(QStringLiteral("schedules")) &&
        db.tables().contains(QStringLiteral("bim_links"))) {
        return true;
    }

    static const char *migration_sql =
        "CREATE TABLE IF NOT EXISTS projects ("
        "  id TEXT PRIMARY KEY, name TEXT NOT NULL, description TEXT, created_at TEXT NOT NULL, updated_at TEXT NOT NULL);"

        "CREATE TABLE IF NOT EXISTS calendars ("
        "  id TEXT PRIMARY KEY, name TEXT NOT NULL, hours_per_day REAL DEFAULT 8.0, working_days TEXT DEFAULT '1,2,3,4,5',"
        "  morning_start TEXT DEFAULT '08:00', morning_end TEXT DEFAULT '12:00', afternoon_start TEXT DEFAULT '13:00', afternoon_end TEXT DEFAULT '17:00',"
        "  created_at TEXT NOT NULL);"

        "CREATE TABLE IF NOT EXISTS calendar_exceptions ("
        "  id TEXT PRIMARY KEY, calendar_id TEXT NOT NULL, date TEXT NOT NULL, name TEXT NOT NULL, is_working_day INTEGER DEFAULT 0, working_hours REAL DEFAULT 0.0);"

        "CREATE TABLE IF NOT EXISTS schedules ("
        "  id TEXT PRIMARY KEY, project_id TEXT, name TEXT NOT NULL, description TEXT, calendar_id TEXT, active_baseline_id TEXT,"
        "  created_at TEXT NOT NULL, updated_at TEXT NOT NULL);"

        "CREATE TABLE IF NOT EXISTS schedule_items ("
        "  id TEXT PRIMARY KEY, schedule_id TEXT NOT NULL, task_id TEXT, wbs_code TEXT, name TEXT NOT NULL, description TEXT,"
        "  start_date TEXT, end_date TEXT, duration_hours REAL DEFAULT 8.0, percent_complete REAL DEFAULT 0.0, status TEXT DEFAULT 'not_started',"
        "  is_milestone INTEGER DEFAULT 0, critical INTEGER DEFAULT 0, early_start TEXT, early_finish TEXT, late_start TEXT, late_finish TEXT,"
        "  total_float_hours REAL DEFAULT 0.0, free_float_hours REAL DEFAULT 0.0, baseline_start_date TEXT, baseline_end_date TEXT,"
        "  baseline_duration_hours REAL DEFAULT 0.0, external_system TEXT, external_id TEXT, version INTEGER DEFAULT 1,"
        "  created_at TEXT NOT NULL, updated_at TEXT NOT NULL);"

        "CREATE INDEX IF NOT EXISTS idx_items_sched ON schedule_items(schedule_id);"
        "CREATE INDEX IF NOT EXISTS idx_items_task ON schedule_items(task_id);"

        "CREATE TABLE IF NOT EXISTS dependencies ("
        "  id TEXT PRIMARY KEY, schedule_id TEXT NOT NULL, predecessor_item_id TEXT NOT NULL, successor_item_id TEXT NOT NULL,"
        "  dependency_type TEXT DEFAULT 'FS', lag_hours REAL DEFAULT 0.0);"

        "CREATE TABLE IF NOT EXISTS baselines ("
        "  id TEXT PRIMARY KEY, schedule_id TEXT NOT NULL, name TEXT NOT NULL, created_at TEXT NOT NULL, frozen INTEGER DEFAULT 1,"
        "  created_by TEXT, notes TEXT);"

        "CREATE TABLE IF NOT EXISTS baseline_items ("
        "  id TEXT PRIMARY KEY, baseline_id TEXT NOT NULL, item_id TEXT NOT NULL, start_date TEXT, end_date TEXT, duration_hours REAL);"

        "CREATE TABLE IF NOT EXISTS progress_updates ("
        "  id TEXT PRIMARY KEY, schedule_item_id TEXT NOT NULL, timestamp TEXT NOT NULL, percent_complete REAL NOT NULL,"
        "  previous_percent REAL DEFAULT 0.0, actual_start TEXT, actual_end TEXT, notes TEXT, reported_by TEXT, variance_hours REAL DEFAULT 0.0);"

        "CREATE TABLE IF NOT EXISTS bim_links ("
        "  id TEXT PRIMARY KEY, schedule_item_id TEXT NOT NULL, bim_source TEXT DEFAULT 'ifc', bim_element_id TEXT NOT NULL,"
        "  category TEXT, level TEXT, discipline TEXT, metadata_json TEXT);"

        "CREATE INDEX IF NOT EXISTS idx_bim_item ON bim_links(schedule_item_id);"
        "CREATE INDEX IF NOT EXISTS idx_bim_elem ON bim_links(bim_element_id);"

        "CREATE TABLE IF NOT EXISTS audit_logs ("
        "  id TEXT PRIMARY KEY, entity_type TEXT NOT NULL, entity_id TEXT NOT NULL, action TEXT NOT NULL, timestamp TEXT NOT NULL,"
        "  user_id TEXT, user_role TEXT, details_json TEXT);"

        "CREATE INDEX IF NOT EXISTS idx_audit_time ON audit_logs(timestamp);";

    QStringList statements = QString::fromUtf8(migration_sql).split(QLatin1Char(';'), Qt::SkipEmptyParts);

    const bool in_tx = db.transaction();
    QSqlQuery q(db);
    for (const QString &stmt : statements) {
        QString s = stmt.trimmed();
        if (s.isEmpty()) continue;
        if (!q.exec(s)) {
            if (in_tx) db.rollback();
            if (error_message) {
                *error_message = QStringLiteral("Erro ao executar migração SQL: %1 (%2)").arg(q.lastError().text(), s);
            }
            return false;
        }
    }
    if (in_tx) {
        db.commit();
    }
    return true;
}

bool GanttDatabase::save_schedule(const Schedule &schedule, QString *error_message)
{
    if (!is_open()) {
        if (error_message) *error_message = QStringLiteral("Banco de dados não está aberto.");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database(connection_name_);
    if (!db.transaction()) {
        if (error_message) *error_message = db.lastError().text();
        return false;
    }

    QSqlQuery q(db);

    // Save Calendar
    q.prepare(QStringLiteral("INSERT OR REPLACE INTO calendars (id, name, hours_per_day, morning_start, morning_end, afternoon_start, afternoon_end, created_at) "
                             "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"));
    q.addBindValue(schedule.calendar.id);
    q.addBindValue(schedule.calendar.name);
    q.addBindValue(schedule.calendar.hours_per_day);
    q.addBindValue(schedule.calendar.morning_start.toString(QStringLiteral("HH:mm")));
    q.addBindValue(schedule.calendar.morning_end.toString(QStringLiteral("HH:mm")));
    q.addBindValue(schedule.calendar.afternoon_start.toString(QStringLiteral("HH:mm")));
    q.addBindValue(schedule.calendar.afternoon_end.toString(QStringLiteral("HH:mm")));
    q.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    if (!q.exec()) {
        db.rollback();
        if (error_message) *error_message = q.lastError().text();
        return false;
    }

    // Save Schedule header
    q.prepare(QStringLiteral("INSERT OR REPLACE INTO schedules (id, project_id, name, description, calendar_id, active_baseline_id, created_at, updated_at) "
                             "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"));
    q.addBindValue(schedule.id);
    q.addBindValue(schedule.project_id);
    q.addBindValue(schedule.name);
    q.addBindValue(schedule.description);
    q.addBindValue(schedule.calendar.id);
    q.addBindValue(schedule.active_baseline_id);
    q.addBindValue(schedule.created_at.toString(Qt::ISODate));
    q.addBindValue(schedule.updated_at.toString(Qt::ISODate));
    if (!q.exec()) {
        db.rollback();
        if (error_message) *error_message = q.lastError().text();
        return false;
    }

    // Clear existing items for this schedule
    q.prepare(QStringLiteral("DELETE FROM schedule_items WHERE schedule_id = ?;"));
    q.addBindValue(schedule.id);
    q.exec();

    // Insert Items using batch execution
    if (!schedule.items.isEmpty()) {
        q.prepare(QStringLiteral("INSERT INTO schedule_items (id, schedule_id, task_id, wbs_code, name, description, start_date, end_date, duration_hours, "
                                 "percent_complete, status, is_milestone, critical, early_start, early_finish, late_start, late_finish, "
                                 "total_float_hours, free_float_hours, baseline_start_date, baseline_end_date, baseline_duration_hours, "
                                 "external_system, external_id, version, created_at, updated_at) "
                                 "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));

        const int count = schedule.items.size();
        QVariantList ids, schedule_ids, task_ids, wbs_codes, names, descriptions, start_dates, end_dates;
        QVariantList duration_hours, percent_completes, statuses, is_milestones, criticals;
        QVariantList early_starts, early_finishes, late_starts, late_finishes;
        QVariantList total_float_hours, free_float_hours, baseline_start_dates, baseline_end_dates, baseline_duration_hours;
        QVariantList external_systems, external_ids, versions, created_ats, updated_ats;

        ids.reserve(count); schedule_ids.reserve(count); task_ids.reserve(count); wbs_codes.reserve(count);
        names.reserve(count); descriptions.reserve(count); start_dates.reserve(count); end_dates.reserve(count);
        duration_hours.reserve(count); percent_completes.reserve(count); statuses.reserve(count);
        is_milestones.reserve(count); criticals.reserve(count); early_starts.reserve(count);
        early_finishes.reserve(count); late_starts.reserve(count); late_finishes.reserve(count);
        total_float_hours.reserve(count); free_float_hours.reserve(count); baseline_start_dates.reserve(count);
        baseline_end_dates.reserve(count); baseline_duration_hours.reserve(count); external_systems.reserve(count);
        external_ids.reserve(count); versions.reserve(count); created_ats.reserve(count); updated_ats.reserve(count);

        for (const auto &item : schedule.items) {
            ids.append(item.id);
            schedule_ids.append(schedule.id);
            task_ids.append(item.task_id);
            wbs_codes.append(item.wbs_code);
            names.append(item.name);
            descriptions.append(item.description);
            start_dates.append(item.start_date.isValid() ? item.start_date.toString(Qt::ISODate) : QString());
            end_dates.append(item.end_date.isValid() ? item.end_date.toString(Qt::ISODate) : QString());
            duration_hours.append(item.duration_hours);
            percent_completes.append(item.percent_complete);
            statuses.append(activity_status_to_string(item.status));
            is_milestones.append(item.is_milestone ? 1 : 0);
            criticals.append(item.critical ? 1 : 0);
            early_starts.append(item.early_start.isValid() ? item.early_start.toString(Qt::ISODate) : QString());
            early_finishes.append(item.early_finish.isValid() ? item.early_finish.toString(Qt::ISODate) : QString());
            late_starts.append(item.late_start.isValid() ? item.late_start.toString(Qt::ISODate) : QString());
            late_finishes.append(item.late_finish.isValid() ? item.late_finish.toString(Qt::ISODate) : QString());
            total_float_hours.append(item.total_float_hours);
            free_float_hours.append(item.free_float_hours);
            baseline_start_dates.append(item.baseline_start_date.isValid() ? item.baseline_start_date.toString(Qt::ISODate) : QString());
            baseline_end_dates.append(item.baseline_end_date.isValid() ? item.baseline_end_date.toString(Qt::ISODate) : QString());
            baseline_duration_hours.append(item.baseline_duration_hours);
            external_systems.append(item.external_system);
            external_ids.append(item.external_id);
            versions.append(item.version);
            created_ats.append(item.created_at.toString(Qt::ISODate));
            updated_ats.append(item.updated_at.toString(Qt::ISODate));
        }

        q.addBindValue(ids);
        q.addBindValue(schedule_ids);
        q.addBindValue(task_ids);
        q.addBindValue(wbs_codes);
        q.addBindValue(names);
        q.addBindValue(descriptions);
        q.addBindValue(start_dates);
        q.addBindValue(end_dates);
        q.addBindValue(duration_hours);
        q.addBindValue(percent_completes);
        q.addBindValue(statuses);
        q.addBindValue(is_milestones);
        q.addBindValue(criticals);
        q.addBindValue(early_starts);
        q.addBindValue(early_finishes);
        q.addBindValue(late_starts);
        q.addBindValue(late_finishes);
        q.addBindValue(total_float_hours);
        q.addBindValue(free_float_hours);
        q.addBindValue(baseline_start_dates);
        q.addBindValue(baseline_end_dates);
        q.addBindValue(baseline_duration_hours);
        q.addBindValue(external_systems);
        q.addBindValue(external_ids);
        q.addBindValue(versions);
        q.addBindValue(created_ats);
        q.addBindValue(updated_ats);

        if (!q.execBatch()) {
            db.rollback();
            if (error_message) *error_message = q.lastError().text();
            return false;
        }
    }

    // Clear and re-insert dependencies
    q.prepare(QStringLiteral("DELETE FROM dependencies WHERE schedule_id = ?;"));
    q.addBindValue(schedule.id);
    q.exec();

    if (!schedule.dependencies.isEmpty()) {
        q.prepare(QStringLiteral("INSERT INTO dependencies (id, schedule_id, predecessor_item_id, successor_item_id, dependency_type, lag_hours) "
                                 "VALUES (?, ?, ?, ?, ?, ?);"));
        const int dep_count = schedule.dependencies.size();
        QVariantList dep_ids, dep_sched_ids, dep_preds, dep_succs, dep_types, dep_lags;
        dep_ids.reserve(dep_count); dep_sched_ids.reserve(dep_count); dep_preds.reserve(dep_count);
        dep_succs.reserve(dep_count); dep_types.reserve(dep_count); dep_lags.reserve(dep_count);

        for (const auto &dep : schedule.dependencies) {
            dep_ids.append(dep.id);
            dep_sched_ids.append(schedule.id);
            dep_preds.append(dep.predecessor_item_id);
            dep_succs.append(dep.successor_item_id);
            dep_types.append(dependency_type_to_string(dep.type));
            dep_lags.append(dep.lag_hours);
        }

        q.addBindValue(dep_ids);
        q.addBindValue(dep_sched_ids);
        q.addBindValue(dep_preds);
        q.addBindValue(dep_succs);
        q.addBindValue(dep_types);
        q.addBindValue(dep_lags);

        if (!q.execBatch()) {
            db.rollback();
            if (error_message) *error_message = q.lastError().text();
            return false;
        }
    }

    // Clear and re-insert BIM links
    q.prepare(QStringLiteral("DELETE FROM bim_links WHERE schedule_item_id IN (SELECT id FROM schedule_items WHERE schedule_id = ?);"));
    q.addBindValue(schedule.id);
    q.exec();

    q.prepare(QStringLiteral("INSERT INTO bim_links (id, schedule_item_id, bim_source, bim_element_id, category, level, discipline, metadata_json) "
                             "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"));
    for (const auto &link : schedule.bim_links) {
        q.bindValue(0, link.id);
        q.bindValue(1, link.schedule_item_id);
        q.bindValue(2, bim_source_to_string(link.bim_source));
        q.bindValue(3, link.bim_element_id);
        q.bindValue(4, link.category);
        q.bindValue(5, link.level);
        q.bindValue(6, link.discipline);
        q.bindValue(7, QString::fromUtf8(QJsonDocument(link.metadata).toJson(QJsonDocument::Compact)));
        if (!q.exec()) {
            db.rollback();
            if (error_message) *error_message = q.lastError().text();
            return false;
        }
    }

    if (!db.commit()) {
        if (error_message) *error_message = db.lastError().text();
        return false;
    }

    return true;
}

bool GanttDatabase::load_schedule(const QString &schedule_id, Schedule &out_schedule, QString *error_message)
{
    if (!is_open()) {
        if (error_message) *error_message = QStringLiteral("Banco de dados não está aberto.");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database(connection_name_);
    QSqlQuery q(db);

    q.prepare(QStringLiteral("SELECT id, project_id, name, description, calendar_id, active_baseline_id, created_at, updated_at "
                             "FROM schedules WHERE id = ?;"));
    q.addBindValue(schedule_id);
    if (!q.exec() || !q.next()) {
        if (error_message) *error_message = QStringLiteral("Cronograma não encontrado no banco.");
        return false;
    }

    out_schedule.id = q.value(0).toString();
    out_schedule.project_id = q.value(1).toString();
    out_schedule.name = q.value(2).toString();
    out_schedule.description = q.value(3).toString();
    out_schedule.active_baseline_id = q.value(5).toString();
    out_schedule.created_at = QDateTime::fromString(q.value(6).toString(), Qt::ISODate);
    out_schedule.updated_at = QDateTime::fromString(q.value(7).toString(), Qt::ISODate);

    // Load Items
    out_schedule.items.clear();
    q.prepare(QStringLiteral("SELECT id, task_id, wbs_code, name, description, start_date, end_date, duration_hours, "
                             "percent_complete, status, is_milestone, critical, early_start, early_finish, late_start, late_finish, "
                             "total_float_hours, free_float_hours, baseline_start_date, baseline_end_date, baseline_duration_hours, "
                             "external_system, external_id, version, created_at, updated_at FROM schedule_items WHERE schedule_id = ?;"));
    q.addBindValue(schedule_id);
    if (q.exec()) {
        while (q.next()) {
            ScheduleItem item;
            item.id = q.value(0).toString();
            item.schedule_id = schedule_id;
            item.task_id = q.value(1).toString();
            item.wbs_code = q.value(2).toString();
            item.name = q.value(3).toString();
            item.description = q.value(4).toString();
            item.start_date = QDateTime::fromString(q.value(5).toString(), Qt::ISODate);
            item.end_date = QDateTime::fromString(q.value(6).toString(), Qt::ISODate);
            item.duration_hours = q.value(7).toDouble();
            item.percent_complete = q.value(8).toDouble();
            item.status = string_to_activity_status(q.value(9).toString());
            item.is_milestone = (q.value(10).toInt() != 0);
            item.critical = (q.value(11).toInt() != 0);
            item.early_start = QDateTime::fromString(q.value(12).toString(), Qt::ISODate);
            item.early_finish = QDateTime::fromString(q.value(13).toString(), Qt::ISODate);
            item.late_start = QDateTime::fromString(q.value(14).toString(), Qt::ISODate);
            item.late_finish = QDateTime::fromString(q.value(15).toString(), Qt::ISODate);
            item.total_float_hours = q.value(16).toDouble();
            item.free_float_hours = q.value(17).toDouble();
            item.baseline_start_date = QDateTime::fromString(q.value(18).toString(), Qt::ISODate);
            item.baseline_end_date = QDateTime::fromString(q.value(19).toString(), Qt::ISODate);
            item.baseline_duration_hours = q.value(20).toDouble();
            item.external_system = q.value(21).toString();
            item.external_id = q.value(22).toString();
            item.version = q.value(23).toInt();
            item.created_at = QDateTime::fromString(q.value(24).toString(), Qt::ISODate);
            item.updated_at = QDateTime::fromString(q.value(25).toString(), Qt::ISODate);
            out_schedule.items.insert(item.id, item);
        }
    }

    // Load Dependencies
    out_schedule.dependencies.clear();
    q.prepare(QStringLiteral("SELECT id, predecessor_item_id, successor_item_id, dependency_type, lag_hours "
                             "FROM dependencies WHERE schedule_id = ?;"));
    q.addBindValue(schedule_id);
    if (q.exec()) {
        while (q.next()) {
            Dependency dep;
            dep.id = q.value(0).toString();
            dep.schedule_id = schedule_id;
            dep.predecessor_item_id = q.value(1).toString();
            dep.successor_item_id = q.value(2).toString();
            dep.type = string_to_dependency_type(q.value(3).toString());
            dep.lag_hours = q.value(4).toDouble();
            out_schedule.dependencies.append(dep);
        }
    }

    // Load BIM Links
    out_schedule.bim_links.clear();
    q.prepare(QStringLiteral("SELECT b.id, b.schedule_item_id, b.bim_source, b.bim_element_id, b.category, b.level, b.discipline, b.metadata_json "
                             "FROM bim_links b JOIN schedule_items i ON b.schedule_item_id = i.id WHERE i.schedule_id = ?;"));
    q.addBindValue(schedule_id);
    if (q.exec()) {
        while (q.next()) {
            BimLink link;
            link.id = q.value(0).toString();
            link.schedule_item_id = q.value(1).toString();
            link.bim_source = string_to_bim_source(q.value(2).toString());
            link.bim_element_id = q.value(3).toString();
            link.category = q.value(4).toString();
            link.level = q.value(5).toString();
            link.discipline = q.value(6).toString();
            QString meta_str = q.value(7).toString();
            if (!meta_str.isEmpty()) {
                link.metadata = QJsonDocument::fromJson(meta_str.toUtf8()).object();
            }
            out_schedule.bim_links.append(link);
        }
    }

    return true;
}

QList<Schedule> GanttDatabase::list_schedules(QString *error_message)
{
    QList<Schedule> list;
    if (!is_open()) {
        if (error_message) *error_message = QStringLiteral("Banco de dados não está aberto.");
        return list;
    }

    QSqlDatabase db = QSqlDatabase::database(connection_name_);
    QSqlQuery q(db);
    if (q.exec(QStringLiteral("SELECT id FROM schedules ORDER BY created_at DESC;"))) {
        while (q.next()) {
            QString id = q.value(0).toString();
            Schedule s;
            if (load_schedule(id, s)) {
                list.append(s);
            }
        }
    }
    return list;
}

bool GanttDatabase::delete_schedule(const QString &schedule_id, QString *error_message)
{
    if (!is_open()) {
        if (error_message) *error_message = QStringLiteral("Banco de dados não está aberto.");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database(connection_name_);
    QSqlQuery q(db);
    q.prepare(QStringLiteral("DELETE FROM schedules WHERE id = ?;"));
    q.addBindValue(schedule_id);
    if (!q.exec()) {
        if (error_message) *error_message = q.lastError().text();
        return false;
    }
    return true;
}

bool GanttDatabase::save_audit_log(const AuditLog &log, QString *error_message)
{
    if (!is_open()) return false;

    QSqlDatabase db = QSqlDatabase::database(connection_name_);
    QSqlQuery q(db);
    q.prepare(QStringLiteral("INSERT INTO audit_logs (id, entity_type, entity_id, action, timestamp, user_id, user_role, details_json) "
                             "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"));
    q.addBindValue(log.id);
    q.addBindValue(log.entity_type);
    q.addBindValue(log.entity_id);
    q.addBindValue(log.action);
    q.addBindValue(log.timestamp.toString(Qt::ISODate));
    q.addBindValue(log.user_id);
    q.addBindValue(user_role_to_string(log.user_role));
    q.addBindValue(QString::fromUtf8(QJsonDocument(log.details).toJson(QJsonDocument::Compact)));

    if (!q.exec()) {
        if (error_message) *error_message = q.lastError().text();
        return false;
    }
    return true;
}

QList<AuditLog> GanttDatabase::load_audit_logs(const QString &entity_type, const QString &entity_id, int limit)
{
    QList<AuditLog> list;
    if (!is_open()) return list;

    QSqlDatabase db = QSqlDatabase::database(connection_name_);
    QSqlQuery q(db);

    if (!entity_type.isEmpty() && !entity_id.isEmpty()) {
        q.prepare(QStringLiteral("SELECT id, entity_type, entity_id, action, timestamp, user_id, user_role, details_json "
                                 "FROM audit_logs WHERE entity_type = ? AND entity_id = ? ORDER BY timestamp DESC LIMIT ?;"));
        q.addBindValue(entity_type);
        q.addBindValue(entity_id);
        q.addBindValue(limit);
    } else {
        q.prepare(QStringLiteral("SELECT id, entity_type, entity_id, action, timestamp, user_id, user_role, details_json "
                                 "FROM audit_logs ORDER BY timestamp DESC LIMIT ?;"));
        q.addBindValue(limit);
    }

    if (q.exec()) {
        while (q.next()) {
            AuditLog log;
            log.id = q.value(0).toString();
            log.entity_type = q.value(1).toString();
            log.entity_id = q.value(2).toString();
            log.action = q.value(3).toString();
            log.timestamp = QDateTime::fromString(q.value(4).toString(), Qt::ISODate);
            log.user_id = q.value(5).toString();
            log.user_role = string_to_user_role(q.value(6).toString());
            QString det = q.value(7).toString();
            if (!det.isEmpty()) {
                log.details = QJsonDocument::fromJson(det.toUtf8()).object();
            }
            list.append(log);
        }
    }

    return list;
}

} // namespace source::gantt
