QT += testlib sql
CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = addressbook_tests
TEMPLATE = app

INCLUDEPATH += ../src

SOURCES += \
    main_test.cpp \
    ../src/validator.cpp \
    ../src/database.cpp \
    ../src/contact.cpp

HEADERS += \
    test_validator.h \
    test_database.h

OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
DESTDIR     = .
