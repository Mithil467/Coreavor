QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

ICON = icon.ico

install.commands += (cp icon.png /usr/share/icons/coreavor.png)
QMAKE_EXTRA_TARGETS += install
uninstall.commands += (rm -f /usr/share/icons/coreavor.png)
QMAKE_EXTRA_TARGETS += uninstall

target.path = /usr/local/bin
desktop.path = /usr/share/applications
desktop.files += coreavor.desktop

INSTALLS += target desktop
