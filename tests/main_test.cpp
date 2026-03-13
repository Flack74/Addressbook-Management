#include <QtTest>
#include "test_validator.h"
#include "test_database.h"

int main(int argc, char *argv[])
{
    int status = 0;

    TestValidator tv;
    status |= QTest::qExec(&tv, argc, argv);

    TestDatabase td;
    status |= QTest::qExec(&td, argc, argv);

    return status;
}
