#pragma once

#include <QString>
#include <QVariantMap>
#include <QVariantList>
#include <functional>
#include <QObject>
#include <QWidget>

namespace source::plugin {

#if !__has_include("plugin/audiolivro/audiolivro_host_bridge.hpp")
class AudiolivroHostBridge {
public:
    void set_language_code(const QString&) {}
    void set_standard_buttons_handler(std::function<void(QObject*)>) {}
};

class IAudiolivroPlugin {
public:
    virtual ~IAudiolivroPlugin() = default;
    virtual void initialize(AudiolivroHostBridge*) = 0;
    virtual QWidget* create_widget(QWidget*) = 0;
    virtual QString display_name() const = 0;
    virtual QString open_menu_text() const = 0;
    virtual QString font_settings_menu_text() const = 0;
    virtual QVariantList voice_options() const = 0;
    virtual QString current_voice_id() const = 0;
    virtual void set_voice_id(const QString&) = 0;
    virtual void open_font_settings(QWidget*) = 0;
    virtual void retranslate_ui() = 0;
    virtual void refresh_task_views() = 0;
};
#endif

#if !__has_include("plugin/mapamental/mapamental_host_bridge.hpp")
class MapaMentalHostBridge {
public:
    void set_app_instance(void*) {}
    void set_language_code(const QString&) {}
    void set_standard_buttons_handler(std::function<void(QObject*)>) {}
    void set_model_availability_checker(std::function<bool()>) {}
    void set_model_diagnostic_handler(std::function<QVariantMap()>) {}
    void set_model_analysis_handler(std::function<QVariantMap(const QString&, const QString&, const QVariantList&, const QVariantList&)>) {}
};

class IMapaMentalPlugin {
public:
    virtual ~IMapaMentalPlugin() = default;
    virtual void initialize(MapaMentalHostBridge*) = 0;
    virtual QWidget* create_widget(QWidget*) = 0;
    virtual QString display_name() const = 0;
    virtual QString open_menu_text() const = 0;
    virtual QString layout_mode_key() const = 0;
    virtual void set_layout_mode_key(const QString&) = 0;
    virtual QString summary_method_key() const = 0;
    virtual void set_summary_method_key(const QString&) = 0;
    virtual void reload_shared_state() = 0;
    virtual void refresh_task_views() = 0;
    virtual QVariantList get_task_font_overrides(bool) const = 0;
    virtual void reset_task_font_overrides(bool) = 0;
    virtual void retranslate_ui() = 0;
};
#endif

#if !__has_include("plugin/modelo/modelo_plugin_interface.hpp")
class IModeloPlugin {
public:
    virtual ~IModeloPlugin() = default;
    virtual bool is_available() const = 0;
    virtual QVariantMap diagnostic() const = 0;
    virtual QVariantMap analyze_document(const QString&, const QString&, const QVariantList&, const QVariantList&) = 0;
};
#endif

} // namespace source::plugin

#if !__has_include("plugin/audiolivro/audiolivro_host_bridge.hpp")
Q_DECLARE_INTERFACE(source::plugin::IAudiolivroPlugin, "source.plugin.IAudiolivroPlugin/1.0")
#endif

#if !__has_include("plugin/mapamental/mapamental_host_bridge.hpp")
Q_DECLARE_INTERFACE(source::plugin::IMapaMentalPlugin, "source.plugin.IMapaMentalPlugin/1.0")
#endif

#if !__has_include("plugin/modelo/modelo_plugin_interface.hpp")
Q_DECLARE_INTERFACE(source::plugin::IModeloPlugin, "source.plugin.IModeloPlugin/1.0")
#endif
