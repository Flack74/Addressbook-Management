#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("AddressBook");

    // Init DB before showing any UI — if it fails, tell the user and exit
    if (!Database::init()) {
        QMessageBox::critical(nullptr, "Startup Error",
            "Could not open or create the contacts database.\n"
            "Check that the current directory is writable.");
        return 1;
    }

    MainWindow w;
    w.show();

    return app.exec();
}
