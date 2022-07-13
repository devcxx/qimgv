
TARGET = qimgv
TEMPLATE = app

QT += core gui svg printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/bin

HEADERS += \
    appversion.h \
    core.h \
    settings.h \
    themestore.h \
    sharedresources.h \
    shortcutbuilder.h \
    proxystyle.h \
    macosapplication.h \

SOURCES += \
    appversion.cpp \
    core.cpp \
    main.cpp \
    settings.cpp \
    themestore.cpp \
    sharedresources.cpp \
    shortcutbuilder.cpp \
    proxystyle.cpp \
    macosapplication.cpp \

RESOURCES += \
    resources.qrc \
    edittool.qrc \

RC_FILE = qimgv.rc

TRANSLATIONS += \
    res/translations/zh_CN.ts \

win32 {
    LIBS += User32.lib
}

macos {
    ICON = distrib/qimgv.icns
}

# components
HEADERS += \
    components/directorymodel.h \
    components/directorypresenter.h \
    components/scriptmanager/scriptmanager.h \
    components/actionmanager/actionmanager.h \
    components/cache/cache.h \
    components/cache/cacheitem.h \
    components/cache/thumbnailcache.h \
    components/loader/loader.h \
    components/loader/loaderrunnable.h \
    components/scaler/scaler.h \
    components/scaler/scalerrunnable.h \
    components/thumbnailer/thumbnailer.h \
    components/thumbnailer/thumbnailerrunnable.h \
    components/directorymanager/directorymanager.h \
    components/directorymanager/watchers/directorywatcher.h \
    components/directorymanager/watchers/directorywatcher_p.h \
    components/directorymanager/watchers/dummywatcher.h \
    components/directorymanager/watchers/watcherevent.h \
    components/directorymanager/watchers/watcherworker.h \

# components
SOURCES += \
    components/directorymodel.cpp \
    components/directorypresenter.cpp \
    components/scriptmanager/scriptmanager.cpp \
    components/actionmanager/actionmanager.cpp \
    components/cache/cache.cpp \
    components/cache/cacheitem.cpp \
    components/cache/thumbnailcache.cpp \
    components/loader/loader.cpp \
    components/loader/loaderrunnable.cpp \
    components/scaler/scaler.cpp \
    components/scaler/scalerrunnable.cpp \
    components/thumbnailer/thumbnailer.cpp \
    components/thumbnailer/thumbnailerrunnable.cpp \
    components/directorymanager/directorymanager.cpp \
    components/directorymanager/watchers/directorywatcher.cpp \
    components/directorymanager/watchers/dummywatcher.cpp \
    components/directorymanager/watchers/watcherevent.cpp \
    components/directorymanager/watchers/watcherworker.cpp \

win32 {
HEADERS += \
    components/directorymanager/watchers/windows/windowswatcher.h \
    components/directorymanager/watchers/windows/windowswatcher_p.h \
    components/directorymanager/watchers/windows/windowsworker.h \

SOURCES += \
    components/directorymanager/watchers/windows/windowswatcher.cpp \
    components/directorymanager/watchers/windows/windowsworker.cpp \
}

unix {
HEADERS += \
    components/directorymanager/watchers/linux/linuxfsevent.h \
    components/directorymanager/watchers/linux/linuxwatcher.h \
    components/directorymanager/watchers/linux/linuxwatcher_p.h \
    components/directorymanager/watchers/linux/linuxworker.h \

SOURCES += \
    components/directorymanager/watchers/linux/linuxfsevent.cpp \
    components/directorymanager/watchers/linux/linuxwatcher.cpp \
    components/directorymanager/watchers/linux/linuxworker.cpp \
}

macos {
#HEADERS += \
#    components/directorymanager/watchers/linux/linuxfsevent.h \
#    components/directorymanager/watchers/linux/linuxwatcher.h \
#    components/directorymanager/watchers/linux/linuxworker.h \

#SOURCES += \
#    components/directorymanager/watchers/linux/linuxfsevent.cpp \
#    components/directorymanager/watchers/linux/linuxwatcher.cpp \
#    components/directorymanager/watchers/linux/linuxworker.cpp \
}

# sourcecontainers
HEADERS += \
    sourcecontainers/fsentry.h \
    sourcecontainers/documentinfo.h \
    sourcecontainers/image.h \
    sourcecontainers/imageanimated.h \
    sourcecontainers/imagestatic.h \
    sourcecontainers/thumbnail.h \
    sourcecontainers/video.h \

# sourcecontainers
SOURCES += \
    sourcecontainers/fsentry.cpp \
    sourcecontainers/documentinfo.cpp \
    sourcecontainers/image.cpp \
    sourcecontainers/imageanimated.cpp \
    sourcecontainers/imagestatic.cpp \
    sourcecontainers/thumbnail.cpp \
    sourcecontainers/video.cpp \

# gui
HEADERS += \
    gui/centralwidget.h \
    gui/contextmenu.h \
    gui/flowlayout.h \
    gui/idirectoryview.h \
    gui/mainwindow.h \

# gui
SOURCES += \
    gui/centralwidget.cpp \
    gui/contextmenu.cpp \
    gui/flowlayout.cpp \
    gui/idirectoryview.cpp \
    gui/mainwindow.cpp \

# gui/customwidgets
HEADERS += \
    gui/customwidgets/actionbutton.h \
    gui/customwidgets/clickablelabel.h \
    gui/customwidgets/contextmenuitem.h \
    gui/customwidgets/floatingwidget.h \
    gui/customwidgets/floatingwidgetcontainer.h \
    gui/customwidgets/iconbutton.h \
    gui/customwidgets/iconwidget.h \
    gui/customwidgets/keysequenceedit.h \
    gui/customwidgets/menuitem.h \
    gui/customwidgets/overlaywidget.h \
    gui/customwidgets/pathselectormenuitem.h \
    gui/customwidgets/sidepanelwidget.h \
    gui/customwidgets/slidepanel.h \
    gui/customwidgets/spinboxinputfix.h \
    gui/customwidgets/thumbnailview.h \
    gui/customwidgets/thumbnailwidget.h \
    gui/customwidgets/videoslider.h \
    gui/customwidgets/pushbuttonfocusind.h \
    gui/customwidgets/styledcombobox.h \
    gui/customwidgets/colorselectorbutton.h \
    gui/customwidgets/entryinfoitem.h \

# gui/customwidgets
SOURCES += \
    gui/customwidgets/actionbutton.cpp \
    gui/customwidgets/clickablelabel.cpp \
    gui/customwidgets/contextmenuitem.cpp \
    gui/customwidgets/floatingwidget.cpp \
    gui/customwidgets/floatingwidgetcontainer.cpp \
    gui/customwidgets/iconbutton.cpp \
    gui/customwidgets/iconwidget.cpp \
    gui/customwidgets/keysequenceedit.cpp \
    gui/customwidgets/menuitem.cpp \
    gui/customwidgets/overlaywidget.cpp \
    gui/customwidgets/pathselectormenuitem.cpp \
    gui/customwidgets/sidepanelwidget.cpp \
    gui/customwidgets/slidepanel.cpp \
    gui/customwidgets/spinboxinputfix.cpp \
    gui/customwidgets/thumbnailview.cpp \
    gui/customwidgets/thumbnailwidget.cpp \
    gui/customwidgets/videoslider.cpp \
    gui/customwidgets/pushbuttonfocusind.cpp \
    gui/customwidgets/styledcombobox.cpp \
    gui/customwidgets/colorselectorbutton.cpp \
    gui/customwidgets/entryinfoitem.cpp \

# gui/dialogs
HEADERS += \
    gui/dialogs/filereplacedialog.h \
    gui/dialogs/resizedialog.h \
    gui/dialogs/scripteditordialog.h \
    gui/dialogs/settingsdialog.h \
    gui/dialogs/shortcutcreatordialog.h \
    gui/dialogs/printdialog.h \

# gui/dialogs
SOURCES += \
    gui/dialogs/filereplacedialog.cpp \
    gui/dialogs/resizedialog.cpp \
    gui/dialogs/scripteditordialog.cpp \
    gui/dialogs/settingsdialog.cpp \
    gui/dialogs/shortcutcreatordialog.cpp \
    gui/dialogs/printdialog.cpp \

# gui/overlays
HEADERS += \
    gui/overlays/changelogwindow.h \
    gui/overlays/controlsoverlay.h \
    gui/overlays/copyoverlay.h \
    gui/overlays/cropoverlay.h \
    gui/overlays/floatingmessage.h \
    gui/overlays/floatingmessageproxy.h \
    gui/overlays/fullscreeninfooverlay.h \
    gui/overlays/fullscreeninfooverlayproxy.h \
    gui/overlays/imageinfooverlay.h \
    gui/overlays/imageinfooverlayproxy.h \
    gui/overlays/mapoverlay.h \
    gui/overlays/renameoverlay.h \
    gui/overlays/saveconfirmoverlay.h \
    gui/overlays/videocontrols.h \
    gui/overlays/videocontrolsproxy.h \
    gui/overlays/zoomindicatoroverlay.h \
    gui/overlays/zoomindicatoroverlayproxy.h \

# gui/overlays
SOURCES += \
    gui/overlays/changelogwindow.cpp \
    gui/overlays/controlsoverlay.cpp \
    gui/overlays/copyoverlay.cpp \
    gui/overlays/cropoverlay.cpp \
    gui/overlays/floatingmessage.cpp \
    gui/overlays/floatingmessageproxy.cpp \
    gui/overlays/fullscreeninfooverlay.cpp \
    gui/overlays/fullscreeninfooverlayproxy.cpp \
    gui/overlays/imageinfooverlay.cpp \
    gui/overlays/imageinfooverlayproxy.cpp \
    gui/overlays/mapoverlay.cpp \
    gui/overlays/renameoverlay.cpp \
    gui/overlays/saveconfirmoverlay.cpp \
    gui/overlays/videocontrols.cpp \
    gui/overlays/videocontrolsproxy.cpp \
    gui/overlays/zoomindicatoroverlay.cpp \
    gui/overlays/zoomindicatoroverlayproxy.cpp \

# gui/panels
HEADERS += \
    gui/panels/croppanel/croppanel.h \
    gui/panels/mainpanel/mainpanel.h \
    gui/panels/mainpanel/thumbnailstrip.h \
    gui/panels/mainpanel/thumbnailstripproxy.h \
    gui/panels/sidepanel/sidepanel.h \
    gui/panels/infobar/infobar.h \
    gui/panels/infobar/infobarproxy.h \

# gui/panels
SOURCES += \
    gui/panels/croppanel/croppanel.cpp \
    gui/panels/mainpanel/mainpanel.cpp \
    gui/panels/mainpanel/thumbnailstrip.cpp \
    gui/panels/mainpanel/thumbnailstripproxy.cpp \
    gui/panels/sidepanel/sidepanel.cpp \
    gui/panels/infobar/infobar.cpp \
    gui/panels/infobar/infobarproxy.cpp \

# components/edittool
HEADERS += \
    components/edittool/editeviewer.h \
    components/edittool/core/edit_core.h \
    components/edittool/core/core_fs_check.h \
    components/edittool/core/core_system.h \
    components/edittool/drawer/arrowdrawer.h \
    components/edittool/drawer/drawer.h \
    components/edittool/drawer/erasedrawer.h \
    components/edittool/drawer/imageblurdrawer.h \
    components/edittool/drawer/pendrawer.h \
    components/edittool/drawer/rectangleblurdrawer.h \
    components/edittool/drawer/shapedrawer.h \
    components/edittool/drawer/textdrawer.h \
    components/edittool/tool/arrowtool.h \
    components/edittool/tool/basetool.h \
    components/edittool/tool/erasetool.h \
    components/edittool/tool/pentool.h \
    components/edittool/tool/shapetool.h \
    components/edittool/tool/texttool.h \
    components/edittool/widget/basebutton.h \
    components/edittool/widget/basecombobox.h \
    components/edittool/widget/colorpanel.h \
    components/edittool/widget/colorpicker.h \
    components/edittool/widget/customtextedit.h \
    components/edittool/widget/inputtextedit.h \
    components/edittool/widget/pinwidget.h \
    components/edittool/widget/pixelpanel.h \
    components/edittool/widget/pixelwidget.h \
    components/edittool/widget/separator.h \
    components/edittool/widget/toolwidget.h

# components/edittool
SOURCES += \
    components/edittool/editeviewer.cpp \
    components/edittool/core/edit_core.cpp \
    components/edittool/core/core_fs_check.cpp \
    components/edittool/core/core_system.cpp \
    components/edittool/drawer/arrowdrawer.cpp \
    components/edittool/drawer/drawer.cpp \
    components/edittool/drawer/erasedrawer.cpp \
    components/edittool/drawer/imageblurdrawer.cpp \
    components/edittool/drawer/pendrawer.cpp \
    components/edittool/drawer/rectangleblurdrawer.cpp \
    components/edittool/drawer/shapedrawer.cpp \
    components/edittool/drawer/textdrawer.cpp \
    components/edittool/tool/arrowtool.cpp \
    components/edittool/tool/basetool.cpp \
    components/edittool/tool/erasetool.cpp \
    components/edittool/tool/pentool.cpp \
    components/edittool/tool/shapetool.cpp \
    components/edittool/tool/texttool.cpp \
    components/edittool/widget/basebutton.cpp \
    components/edittool/widget/basecombobox.cpp \
    components/edittool/widget/colorpanel.cpp \
    components/edittool/widget/colorpicker.cpp \
    components/edittool/widget/customtextedit.cpp \
    components/edittool/widget/inputtextedit.cpp \
    components/edittool/widget/pinwidget.cpp \
    components/edittool/widget/pixelpanel.cpp \
    components/edittool/widget/pixelwidget.cpp \
    components/edittool/widget/separator.cpp \
    components/edittool/widget/toolwidget.cpp

# gui/viewers
HEADERS += \
    gui/viewers/documentwidget.h \
    gui/viewers/imageviewerv2.h \
    gui/viewers/videoplayer.h \
    gui/viewers/videoplayerinitproxy.h \
    gui/viewers/viewerwidget.h \

# gui/viewers
SOURCES += \
    gui/viewers/documentwidget.cpp \
    gui/viewers/imageviewerv2.cpp \
    gui/viewers/videoplayer.cpp \
    gui/viewers/videoplayerinitproxy.cpp \
    gui/viewers/viewerwidget.cpp \

# gui/folderview
HEADERS += \
    gui/folderview/foldergridview.h \
    gui/folderview/folderview.h \
    gui/folderview/folderviewproxy.h \
    gui/folderview/filesystemmodelcustom.h \
    gui/folderview/treeviewcustom.h \
    gui/folderview/bookmarkswidget.h \
    gui/folderview/bookmarksitem.h \
    gui/folderview/fvoptionspopup.h \

# gui/folderview
SOURCES += \
    gui/folderview/foldergridview.cpp \
    gui/folderview/folderview.cpp \
    gui/folderview/folderviewproxy.cpp \
    gui/folderview/filesystemmodelcustom.cpp \
    gui/folderview/treeviewcustom.cpp \
    gui/folderview/bookmarkswidget.cpp \
    gui/folderview/bookmarksitem.cpp \
    gui/folderview/fvoptionspopup.cpp \

# UI Files
FORMS += \
    gui/contextmenu.ui \
    gui/dialogs/filereplacedialog.ui \
    gui/dialogs/resizedialog.ui \
    gui/dialogs/scripteditordialog.ui \
    gui/dialogs/settingsdialog.ui \
    gui/dialogs/shortcutcreatordialog.ui \
    gui/dialogs/printdialog.ui \
    gui/overlays/changelogwindow.ui \
    gui/overlays/copyoverlay.ui \
    gui/overlays/floatingmessage.ui \
    gui/overlays/fullscreeninfooverlay.ui \
    gui/overlays/imageinfooverlay.ui \
    gui/overlays/renameoverlay.ui \
    gui/overlays/saveconfirmoverlay.ui \
    gui/overlays/videocontrols.ui \
    gui/overlays/qcolorpicker.ui \
    gui/panels/croppanel/croppanel.ui \
    gui/panels/sidepanel/sidepanel.ui \
    gui/panels/infobar/infobar.ui \
    gui/folderview/folderview.ui \
    gui/folderview/fvoptionspopup.ui \

# utils
HEADERS += \
    utils/actions.h \
    utils/cmdoptionsrunner.h \
    utils/imagefactory.h \
    utils/imagelib.h \
    utils/inputmap.h \
    utils/randomizer.h \
    utils/script.h \
    utils/stuff.h \
    utils/wallpapersetter.h \
    utils/fileoperations.h \
    utils/orderedmap.h \

# utils
SOURCES += \
    utils/actions.cpp \
    utils/cmdoptionsrunner.cpp \
    utils/imagefactory.cpp \
    utils/imagelib.cpp \
    utils/inputmap.cpp \
    utils/randomizer.cpp \
    utils/script.cpp \
    utils/sleep.cpp \
    utils/stuff.cpp \
    utils/wallpapersetter.cpp \
    utils/fileoperations.cpp \
