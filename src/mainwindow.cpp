#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "contactdialog.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Address Book");
    resize(720, 480);

    // Table setup — 4 columns, headers, no editing directly in the table
    ui->tableContacts->setColumnCount(4);
    ui->tableContacts->setHorizontalHeaderLabels({"Name", "Mobile", "Email", "Birthday"});
    ui->tableContacts->horizontalHeader()->setStretchLastSection(true);
    ui->tableContacts->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableContacts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableContacts->setAlternatingRowColors(true);
    ui->tableContacts->verticalHeader()->setVisible(false);

    connect(ui->btnAdd,    &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->btnEdit,   &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(ui->tableContacts->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);

    // Edit and Delete start disabled — nothing is selected yet
    ui->btnEdit->setEnabled(false);
    ui->btnDelete->setEnabled(false);

    loadContacts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadContacts()
{
    m_contacts = Database::fetchAll();
    populateTable();
}

void MainWindow::populateTable()
{
    ui->tableContacts->setRowCount(0); // clear existing rows

    for (const Contact &c : m_contacts) {
        int row = ui->tableContacts->rowCount();
        ui->tableContacts->insertRow(row);

        ui->tableContacts->setItem(row, 0, new QTableWidgetItem(c.name));
        ui->tableContacts->setItem(row, 1, new QTableWidgetItem(c.mobile));
        ui->tableContacts->setItem(row, 2, new QTableWidgetItem(c.email));
        ui->tableContacts->setItem(row, 3, new QTableWidgetItem(c.birthday));

        // Store the contact's DB id in the first cell so we can retrieve it later
        ui->tableContacts->item(row, 0)->setData(Qt::UserRole, c.id);
    }
}

Contact *MainWindow::selectedContact()
{
    int row = ui->tableContacts->currentRow();
    if (row < 0 || row >= m_contacts.size()) return nullptr;

    // The table rows match m_contacts order because both come from fetchAll()
    // which sorts by name — we use the stored ID to find the right one
    int id = ui->tableContacts->item(row, 0)->data(Qt::UserRole).toInt();
    for (Contact &c : m_contacts) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

void MainWindow::onSelectionChanged()
{
    bool has = (ui->tableContacts->currentRow() >= 0);
    ui->btnEdit->setEnabled(has);
    ui->btnDelete->setEnabled(has);
}

void MainWindow::onAddClicked()
{
    Contact blank;
    ContactDialog dlg(blank, this);

    if (dlg.exec() == QDialog::Accepted) {
        Contact c = dlg.contact();
        if (Database::save(c)) {
            loadContacts();
        }
    }
}

void MainWindow::onEditClicked()
{
    Contact *c = selectedContact();
    if (!c) return;

    ContactDialog dlg(*c, this);
    if (dlg.exec() == QDialog::Accepted) {
        *c = dlg.contact();
        if (Database::save(*c)) {
            loadContacts();
        }
    }
}

void MainWindow::onDeleteClicked()
{
    Contact *c = selectedContact();
    if (!c) return;

    auto reply = QMessageBox::question(
        this, "Delete Contact",
        QString("Delete \"%1\"? This cannot be undone.").arg(c->name),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        Database::remove(c->id);
        loadContacts();
    }
}
