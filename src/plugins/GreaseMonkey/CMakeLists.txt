include_directories(settings)

set( GreaseMonkey_SRCS
	gm_plugin.cpp
	gm_manager.cpp
	gm_script.cpp
	gm_downloader.cpp
	gm_addscriptdialog.cpp
	gm_notification.cpp
	gm_icon.cpp
	gm_jsobject.cpp
	settings/gm_settings.cpp
	settings/gm_settingslistdelegate.cpp
	settings/gm_settingsscriptinfo.cpp
	settings/gm_settingslistwidget.cpp
	)

ecm_create_qm_loader( GreaseMonkey_SRCS falkon_greasemonkey_qt )

set( GreaseMonkey_UIS
	gm_addscriptdialog.ui
	gm_notification.ui
	settings/gm_settings.ui
	settings/gm_settingsscriptinfo.ui
	)
qt_wrap_ui(UIS ${GreaseMonkey_UIS})

set( GreaseMonkey_RSCS
	greasemonkey.qrc
	)
qt_add_resources(RSCS ${GreaseMonkey_RSCS})

add_library(GreaseMonkey MODULE ${GreaseMonkey_SRCS} ${UIS} ${RSCS})
install(TARGETS GreaseMonkey DESTINATION ${FALKON_INSTALL_PLUGINDIR})
target_link_libraries(GreaseMonkey FalkonPrivate)

