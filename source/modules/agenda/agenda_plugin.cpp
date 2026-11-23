#include "source/modules/agenda/agenda_plugin.hpp"

#include "source/modules/agenda/AgendaAvaliacoes/BancoDeDados/Banco_Dados.hpp"
#include "source/modules/agenda/AgendaAvaliacoes/utils/CaminhoPersistenteUtils.hpp"

#include <QCoreApplication>
#include <QEvent>
#include <QMenu>
#include <QPointer>
#include <QTimer>

namespace source::plugin {

namespace {
class AgendaParentCloseFilter : public QObject {
public:
    explicit AgendaParentCloseFilter(QWidget *target, QObject *parent = nullptr)
        : QObject(parent), target_(target) {}
protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event && event->type() == QEvent::Close && target_) {
            target_->close();
        }
        return QObject::eventFilter(obj, event);
    }
private:
    QPointer<QWidget> target_;
};
} // namespace

AgendaPlugin::AgendaPlugin(QObject *parent)
    : QObject(parent)
{
}

AgendaPlugin::~AgendaPlugin() = default;

QString AgendaPlugin::plugin_id() const
{
    return QStringLiteral("agenda");
}

QString AgendaPlugin::display_name() const
{
    return QCoreApplication::translate("AgendaPlugin", "Registro de Atividades");
}

QString AgendaPlugin::open_menu_text() const
{
    return QCoreApplication::translate("AgendaPlugin", "Registro de Atividades");
}

void AgendaPlugin::initialize(AgendaHostBridge *host_bridge)
{
    if (host_bridge == nullptr && window_ != nullptr) {
        QObject::disconnect(host_destroyed_connection_);
        host_destroyed_connection_ = {};
        delete window_;
        window_ = nullptr;
    }

    host_bridge_ = host_bridge;
    if (host_bridge_ != nullptr && !host_bridge_->persistent_dir().trimmed().isEmpty()) {
        definir_caminho_persistente_override(host_bridge_->persistent_dir());
    } else {
        limpar_caminho_persistente_override();
    }
    if (host_bridge_ != nullptr) {
        host_bridge_->set_color_resolver_handler([](const QString &title) {
            return obter_cor_por_nome(title);
        });
        GerenciamentoAtividades config_reader(false);
        host_bridge_->notify_color_mode_changed(config_reader.obter_modo_cores());
    }
    apply_host_language();
}

void AgendaPlugin::open_window(QWidget *parent)
{
    InterfaceGerenciadorAtividades *window = ensure_window(parent);
    if (window == nullptr) {
        return;
    }

    window->setAttribute(Qt::WA_DeleteOnClose, false);
    window->show();
    window->raise();
    window->activateWindow();
}

void AgendaPlugin::populate_delete_item_menu(QMenu *menu)
{
    if (menu == nullptr) {
        return;
    }

    if (InterfaceGerenciadorAtividades *window = ensure_window(menu->parentWidget())) {
        window->populate_delete_item_menu(menu);
    }
}

void AgendaPlugin::populate_color_menu(QMenu *menu)
{
    if (menu == nullptr) {
        return;
    }

    if (InterfaceGerenciadorAtividades *window = ensure_window(menu->parentWidget())) {
        window->populate_color_menu(menu);
    }
}

void AgendaPlugin::retranslate_ui()
{
    apply_host_language();
    if (window_ != nullptr) {
        window_->retranslate_ui();
    }
}

InterfaceGerenciadorAtividades *AgendaPlugin::ensure_window(QWidget *parent)
{
    if (window_ == nullptr) {
        window_ = new InterfaceGerenciadorAtividades(nullptr, nullptr, true, true, false);
        window_->setWindowFlag(Qt::Window, true);
        window_->setAttribute(Qt::WA_DeleteOnClose, false);
        if (parent != nullptr) {
            QObject::disconnect(host_destroyed_connection_);
            host_destroyed_connection_ = QObject::connect(parent, &QObject::destroyed, this, [this]() {
                if (window_ != nullptr) {
                    delete window_;
                    window_ = nullptr;
                }
                host_destroyed_connection_ = {};
            });
            if (parent_close_filter_ == nullptr) {
                parent_close_filter_ = new AgendaParentCloseFilter(window_, parent);
                parent->installEventFilter(parent_close_filter_);
            }
        }
        QObject::connect(window_,
                         &InterfaceGerenciadorAtividades::atividade_registrada,
                         this,
                         [this](const QVariantMap &task) {
                             if (host_bridge_ != nullptr) {
                                 host_bridge_->publish_task_to_eisenhower(task);
                             }
                         });
        QObject::connect(window_,
                         &InterfaceGerenciadorAtividades::modo_cores_alterado,
                         this,
                         [this](const QString &modo) {
                             if (host_bridge_ != nullptr) {
                                 host_bridge_->notify_color_mode_changed(modo);
                             }
                         });
        apply_host_language();
        if (host_bridge_ != nullptr) {
            host_bridge_->notify_color_mode_changed(window_->gerenciamento_atividades.obter_modo_cores());
        }
    } else if (parent != nullptr && !host_destroyed_connection_) {
        host_destroyed_connection_ = QObject::connect(parent, &QObject::destroyed, this, [this]() {
            if (window_ != nullptr) {
                delete window_;
                window_ = nullptr;
            }
            host_destroyed_connection_ = {};
        });
        parent->installEventFilter(new AgendaParentCloseFilter(window_, parent));
    }

    return window_;
}

void AgendaPlugin::apply_host_language()
{
    if (window_ == nullptr || host_bridge_ == nullptr) {
        return;
    }

    const QString language_code = host_bridge_->language_code();
    if (!language_code.trimmed().isEmpty()) {
        window_->change_language(language_code);
    }
}

}  // namespace source::plugin
