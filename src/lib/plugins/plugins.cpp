/* ============================================================
* Falkon - Qt web browser
* Copyright (C) 2010-2018 David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "pluginproxy.h"
#include "plugininterface.h"
#include "mainapplication.h"
#include "speeddial.h"
#include "settings.h"
#include "datapaths.h"
#include "adblock/adblockplugin.h"
#include "../config.h"
#include "desktopfile.h"

#include <iostream>
#include <QPluginLoader>
#include <QDir>

Plugins::Plugins(QObject* parent)
    : QObject(parent)
    , m_pluginsLoaded(false)
    , m_speedDial(new SpeedDial(this))
{
    loadSettings();
}

QList<Plugins::Plugin> Plugins::getAvailablePlugins()
{
    loadAvailablePlugins();

    return m_availablePlugins;
}

bool Plugins::loadPlugin(Plugins::Plugin* plugin)
{
    if (plugin->isLoaded()) {
        return true;
    }

    plugin->pluginLoader->setFileName(plugin->fullPath);
    PluginInterface* iPlugin = qobject_cast<PluginInterface*>(plugin->pluginLoader->instance());
    if (!iPlugin) {
        return false;
    }

    m_availablePlugins.removeOne(*plugin);
    plugin->instance = initPlugin(PluginInterface::LateInitState, iPlugin, plugin->pluginLoader);
    m_availablePlugins.prepend(*plugin);

    refreshLoadedPlugins();

    return plugin->isLoaded();
}

void Plugins::unloadPlugin(Plugins::Plugin* plugin)
{
    if (!plugin->isLoaded()) {
        return;
    }

    plugin->instance->unload();
    plugin->pluginLoader->unload();
    emit pluginUnloaded(plugin->instance);

    m_availablePlugins.removeOne(*plugin);
    plugin->instance = 0;
    m_availablePlugins.append(*plugin);

    refreshLoadedPlugins();
}

void Plugins::loadSettings()
{
    Settings settings;
    settings.beginGroup("Plugin-Settings");
    m_allowedPlugins = settings.value("AllowedPlugins", QStringList()).toStringList();
    settings.endGroup();
}

void Plugins::shutdown()
{
    foreach (PluginInterface* iPlugin, m_loadedPlugins) {
        iPlugin->unload();
    }
}

void Plugins::loadPlugins()
{
    QDir settingsDir(DataPaths::currentProfilePath() + "/extensions/");
    if (!settingsDir.exists()) {
        settingsDir.mkdir(settingsDir.absolutePath());
    }

    foreach (const QString &pluginFile, m_allowedPlugins) {
        QString fullPath;
        if (QFileInfo(pluginFile).isAbsolute()) {
            fullPath = pluginFile;
        } else {
            const QStringList dirs = DataPaths::allPaths(DataPaths::Plugins);
            for (const QString &dir : dirs) {
                fullPath = dir + QL1C('/') + pluginFile;
                if (QFileInfo::exists(fullPath)) {
                    break;
                }
            }
            if (!QFileInfo::exists(fullPath)) {
                qWarning() << "Plugin" << pluginFile << "not found";
                continue;
            }
        }

        QPluginLoader* loader = new QPluginLoader(fullPath);
        PluginInterface* iPlugin = qobject_cast<PluginInterface*>(loader->instance());

        if (!iPlugin) {
            qWarning() << "Plugins::loadPlugins Loading" << fullPath << "failed:" << loader->errorString();
            continue;
        }

        Plugin plugin;
        plugin.fileName = QFileInfo(fullPath).fileName();
        plugin.fullPath = fullPath;
        plugin.pluginLoader = loader;
        plugin.instance = initPlugin(PluginInterface::StartupInitState, iPlugin, loader);

        if (plugin.isLoaded()) {
            plugin.pluginSpec = createSpec(iPlugin->metaData());
            if (!plugin.pluginSpec.name.isEmpty()) {
                m_availablePlugins.append(plugin);
            }
        }
    }

    // Internal plugins
    AdBlockPlugin *adBlock = new AdBlockPlugin();
    if (initPlugin(PluginInterface::StartupInitState, adBlock, nullptr)) {
        m_internalPlugins.append(adBlock);
    }

    refreshLoadedPlugins();

    std::cout << "Falkon: " << (m_loadedPlugins.count() - m_internalPlugins.count()) << " extensions loaded"  << std::endl;
}

void Plugins::loadAvailablePlugins()
{
    if (m_pluginsLoaded) {
        return;
    }

    m_pluginsLoaded = true;

    QStringList dirs = DataPaths::allPaths(DataPaths::Plugins);

    // Portable build: Load only plugins from DATADIR/plugins/ directory.
    if (mApp->isPortable())
        dirs = QStringList(DataPaths::path(DataPaths::Plugins));

    foreach (const QString &dir, dirs) {
        QDir pluginsDir = QDir(dir);
        foreach (const QString &fileName, pluginsDir.entryList(QDir::Files)) {
            const QString absolutePath = pluginsDir.absoluteFilePath(fileName);

            QPluginLoader* loader = new QPluginLoader(absolutePath);
            PluginInterface* iPlugin = qobject_cast<PluginInterface*>(loader->instance());

            if (!iPlugin) {
                qWarning() << "Plugins::loadAvailablePlugins" << loader->errorString();
                continue;
            }

            Plugin plugin;
            plugin.fileName = fileName;
            plugin.fullPath = absolutePath;
            plugin.pluginSpec = createSpec(iPlugin->metaData());
            plugin.pluginLoader = loader;
            plugin.instance = 0;

            loader->unload();

            if (!plugin.pluginSpec.name.isEmpty() && !alreadySpecInAvailable(plugin.pluginSpec)) {
                m_availablePlugins.append(plugin);
            }
        }
    }
}

PluginInterface* Plugins::initPlugin(PluginInterface::InitState state, PluginInterface* pluginInterface, QPluginLoader* loader)
{
    if (!pluginInterface) {
        return 0;
    }

    pluginInterface->init(state, DataPaths::currentProfilePath() + QL1S("/extensions"));

    if (!pluginInterface->testPlugin()) {
        pluginInterface->unload();
        if (loader) {
            loader->unload();
        }

        emit pluginUnloaded(pluginInterface);

        return 0;
    }

    qApp->installTranslator(pluginInterface->getTranslator(mApp->currentLanguageFile()));

    return pluginInterface;
}

void Plugins::refreshLoadedPlugins()
{
    m_loadedPlugins = m_internalPlugins;

    foreach (const Plugin &plugin, m_availablePlugins) {
        if (plugin.isLoaded()) {
            m_loadedPlugins.append(plugin.instance);
        }
    }
}

bool Plugins::alreadySpecInAvailable(const PluginSpec &spec)
{
    foreach (const Plugin &plugin, m_availablePlugins) {
        if (plugin.pluginSpec == spec) {
            return true;
        }
    }

    return false;
}

PluginSpec Plugins::createSpec(const DesktopFile &metaData) const
{
    PluginSpec spec;
    spec.name = metaData.name(mApp->currentLanguage());
    spec.description = metaData.comment(mApp->currentLanguage());
    spec.version = metaData.value(QSL("X-Falkon-Version")).toString();
    spec.author = QSL("%1 <%2>").arg(metaData.value(QSL("X-Falkon-Author")).toString(), metaData.value(QSL("X-Falkon-Email")).toString());
    spec.hasSettings = metaData.value(QSL("X-Falkon-Settings")).toBool();

    const QString iconName = metaData.icon();
    if (!iconName.isEmpty()) {
        if (QFileInfo::exists(iconName)) {
            spec.icon = QIcon(iconName).pixmap(32);
        } else {
            spec.icon = QIcon::fromTheme(iconName).pixmap(32);
        }
    }

    return spec;
}
