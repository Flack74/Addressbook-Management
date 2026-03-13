QT += testlib sql
CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = addressbook_tests
TEMPLATE = app

INCLUDEPATH += ../src

SOURCES += \
    test_validator.cpp \
    test_database.cpp \
    ../src/validator.cpp \
    ../src/database.cpp \
    ../src/contact.cpp

OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
DESTDIR     = .
