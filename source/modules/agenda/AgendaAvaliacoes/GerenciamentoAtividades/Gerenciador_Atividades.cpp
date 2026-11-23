#include "Gerenciador_Atividades.hpp"

#include "source/modules/agenda/AgendaAvaliacoes/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlError>
#include <QUuid>

QString chave_atividade(const Atividade &atividade) {
    return QStringList{atividade.data, atividade.tipo, atividade.sequencia, atividade.nome, atividade.turma}.join(
        QChar(0x1f));
}

QString atividade_para_linha(const Atividade &atividade) {
    return QString("%1 %2 – %3 %4 %5")
        .arg(atividade.data, atividade.tipo, atividade.sequencia, atividade.nome, atividade.turma);
}

GerenciamentoAtividades::GerenciamentoAtividades(bool persistir_atividades_locais_)
    : persistir_atividades_locais(persistir_atividades_locais_) {
    try {
        QString persistDir = obter_caminho_persistente();
        QDir dir(persistDir);
        dir.mkpath(".");
        QFile config(dir.filePath("agenda_config.json"));
        if (!config.exists()) {
            config.setFileName(dir.filePath("config.json"));
        }
        if (config.open(QIODevice::ReadOnly)) {
            const QJsonObject obj = QJsonDocument::fromJson(config.readAll()).object();
            const QString modo = obj.value("modo_cores").toString();
            if (modo == "preto" || modo == "coloridas") {
                modo_cores = modo;
                modo_cores_eisenhower = modo;
                modo_cores_pomodoro = modo;
                modo_cores_feynman = modo;
            } else {
                const QString modo_eisenhower = obj.value("modo_cores_eisenhower").toString();
                if (modo_eisenhower == "preto" || modo_eisenhower == "coloridas") {
                    modo_cores = modo_eisenhower;
                    modo_cores_eisenhower = modo_eisenhower;
                    modo_cores_pomodoro = modo_eisenhower;
                    modo_cores_feynman = modo_eisenhower;
                }
            }
        }
        if (!persistir_atividades_locais) {
            return;
        }
        connectionName = "atividades_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            db.setDatabaseName(dir.filePath("atividades.db"));
            if (!db.open()) {
                source::utils::LogManager::get_logger()->error(QString("Erro ao abrir atividades.db: %1").arg(db.lastError().text()));
            }
        }
        criar_tabela();
    } catch (const std::exception &e) {
        source::utils::LogManager::get_logger()->error(QString("Erro ao inicializar GerenciamentoAtividades: %1").arg(e.what()));
    } catch (...) {
        source::utils::LogManager::get_logger()->error("Erro ao inicializar GerenciamentoAtividades");
    }
}

QSqlDatabase GerenciamentoAtividades::obter_conexao() const {
    if (connectionName.isEmpty()) {
        return QSqlDatabase();
    }
    return QSqlDatabase::database(connectionName);
}

bool GerenciamentoAtividades::usa_atividades_locais() const {
    if (!persistir_atividades_locais || connectionName.isEmpty()) {
        return false;
    }
    const QSqlDatabase db = QSqlDatabase::database(connectionName, false);
    return db.isValid() && db.isOpen();
}

GerenciamentoAtividades::~GerenciamentoAtividades() {
    const QString name = connectionName;
    if (!name.isEmpty()) {
        {
            QSqlDatabase db = QSqlDatabase::database(name, false);
            if (db.isOpen()) {
                db.close();
            }
        }
        QSqlDatabase::removeDatabase(name);
    }
}
