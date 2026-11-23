#include "source/modules/feynman/feynman_plugin.hpp"

#include <QCoreApplication>
#include <QLocale>
#include <QWidget>

#include "source/modules/feynman/Feynman/feynman_metodo_helpers.hpp"
#include "source/modules/feynman/Feynman/feynman_support.hpp"
#include "source/modules/feynman/Feynman/fey_08_editar_rotulos_dialog.hpp"
#include "source/modules/feynman/Feynman/language/tr_01_gerenciadorTraducao.hpp"
#include "source/modules/feynman/mod_05_metodo_feynman.hpp"

namespace source::plugin {
namespace {

QString effective_language_code(FeynmanHostBridge *host_bridge)
{
    if (host_bridge == nullptr) {
        return QLocale::system().name();
    }

    const QString code = host_bridge->language_code().trimmed();
    return code.isEmpty() ? QLocale::system().name() : code;
}

}  // namespace

FeynmanPlugin::FeynmanPlugin(QObject *parent)
    : QObject(parent)
{
}

FeynmanPlugin::~FeynmanPlugin() = default;

QString FeynmanPlugin::plugin_id() const
{
    return QStringLiteral("feynman");
}

QString FeynmanPlugin::display_name() const
{
    return QCoreApplication::translate("FeynmanPlugin", "Feynman");
}

QString FeynmanPlugin::open_menu_text() const
{
    return source::plugin::Feynman::compat::tr(nullptr, QStringLiteral("Descrição Feynman"), QStringLiteral("Feynman Description"));
}

void FeynmanPlugin::initialize(FeynmanHostBridge *host_bridge)
{
    host_bridge_ = host_bridge;
    if (translation_manager_ == nullptr) {
        translation_manager_ = new source::plugin::Feynman::language::GerenciadorTraducao(this);
    }
    translation_manager_->aplicar_traducao(effective_language_code(host_bridge_));
    source::plugin::Feynman::compat::set_host_bridge(host_bridge_);
}

QVariantMap FeynmanPlugin::open_feynman_dialog_for_task(const QVariantMap &task, QWidget *parent)
{
    if (translation_manager_ != nullptr) {
        translation_manager_->aplicar_traducao(effective_language_code(host_bridge_));
    }

    if (widget_ != nullptr) {
        delete widget_;
    }
    widget_ = new source::plugin::Feynman::MetodoFeynman(parent);

    widget_->atualizar_traducoes();
    widget_->set_task_context(task);
    
    if (widget_->exec() == QDialog::Accepted) {
        return widget_->get_task_context();
    }
    return QVariantMap();
}

void FeynmanPlugin::retranslate_ui()
{
    if (translation_manager_ != nullptr) {
        translation_manager_->aplicar_traducao(effective_language_code(host_bridge_));
    }

    if (widget_ != nullptr) {
        widget_->atualizar_traducoes();
        widget_->update();
    }
}

void FeynmanPlugin::receive_mindmap_concept(const QVariantMap &mindmap_concept)
{
    Q_UNUSED(mindmap_concept);
}

void FeynmanPlugin::receive_task(const QVariantMap &task)
{
    Q_UNUSED(task);
}
void FeynmanPlugin::reload_shared_state()
{
}

void FeynmanPlugin::refresh_task_views()
{

}

int FeynmanPlugin::selected_task_count() const
{
    return 0;
}

QString FeynmanPlugin::selected_task_color() const
{
    return QString();
}

int FeynmanPlugin::apply_color_to_selected_tasks(const QString &color)
{
    Q_UNUSED(color);
    return 0;
}

void FeynmanPlugin::reset_task_font_overrides(bool target_linked)
{
    Q_UNUSED(target_linked);
}

QVariantList FeynmanPlugin::get_task_font_overrides(bool target_linked) const
{
    Q_UNUSED(target_linked);
    return {};
}

void FeynmanPlugin::open_edit_labels_dialog(QWidget *parent)
{
    source::plugin::Feynman::EditarRotulosDialog dialog(parent);
    QObject::connect(&dialog, &source::plugin::Feynman::EditarRotulosDialog::labels_changed, [this]() {
        retranslate_ui();
    });
    dialog.exec();
}

}  // namespace source::plugin
