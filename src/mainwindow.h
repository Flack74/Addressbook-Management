#pragma once

#include "contact.h"
#include <QMainWindow>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onSelectionChanged();

private:
    Ui::MainWindow  *ui;
    QList<Contact>   m_contacts;

    void loadContacts();
    void populateTable();
    void updateContactCount();
    Contact *selectedContact();
};
