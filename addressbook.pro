QT += core gui sql widgets

CONFIG += c++17

TARGET = addressbook
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/contact.cpp \
    src/database.cpp \
    src/contactdialog.cpp \
    src/validator.cpp

HEADERS += \
    src/mainwindow.h \
    src/contact.h \
    src/database.h \
    src/contactdialog.h \
    src/validator.h

FORMS += \
    ui/mainwindow.ui \
    ui/contactdialog.ui

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui

DESTDIR = .
