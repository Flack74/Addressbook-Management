#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "contactdialog.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

static const QString APP_STYLE = R"(
QMainWindow, QWidget#centralwidget {
    background-color: #1a1b26;
}

QWidget#headerWidget {
    background-color: #16161e;
    border-bottom: 1px solid #2a2b3d;
}

QLabel#lblTitle {
    color: #c0caf5;
    font-size: 18px;
    font-weight: bold;
    letter-spacing: 1px;
    padding-left: 8px;
}

QLabel#lblCount {
    color: #565f89;
    font-size: 12px;
    padding-right: 8px;
}

QTableWidget {
    background-color: #1a1b26;
    alternate-background-color: #1e1f2e;
    color: #a9b1d6;
    font-size: 13px;
    border: none;
    gridline-color: transparent;
    selection-background-color: #283457;
    selection-color: #c0caf5;
    outline: none;
}

QTableWidget::item {
    padding: 0px 20px;
    border: none;
}

QTableWidget::item:selected {
    background-color: #283457;
    color: #c0caf5;
}

QTableWidget::item:hover {
    background-color: #222336;
}

QHeaderView::section {
    background-color: #16161e;
    color: #565f89;
    font-size: 11px;
    font-weight: bold;
    letter-spacing: 1px;
    padding: 10px 20px;
    border: none;
    border-bottom: 2px solid #2a2b3d;
    text-align: left;
}

QScrollBar:vertical {
    background: #1a1b26;
    width: 6px;
    border-radius: 3px;
}

QScrollBar::handle:vertical {
    background: #2a2b3d;
    border-radius: 3px;
    min-height: 30px;
}

QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    height: 0px;
}

QWidget#toolbarWidget {
    background-color: #16161e;
    border-top: 1px solid #2a2b3d;
}

QPushButton {
    border-radius: 6px;
    font-size: 13px;
    font-weight: 500;
    padding: 0 18px;
    border: none;
}

QPushButton#btnAdd {
    background-color: #7aa2f7;
    color: #1a1b26;
    font-weight: bold;
}

QPushButton#btnAdd:hover {
    background-color: #89b4ff;
}

QPushButton#btnAdd:pressed {
    background-color: #5d7fc4;
}

QPushButton#btnEdit {
    background-color: #1e1f2e;
    color: #a9b1d6;
    border: 1px solid #3b3c52;
}

QPushButton#btnEdit:hover {
    background-color: #2a2b3d;
    color: #c0caf5;
}

QPushButton#btnEdit:disabled {
    background-color: #16161e;
    color: #2a2b3d;
    border-color: #1e1f2e;
}

QPushButton#btnDelete {
    background-color: #1e1f2e;
    color: #f7768e;
    border: 1px solid #3b3c52;
}

QPushButton#btnDelete:hover {
    background-color: #2d1b22;
    border-color: #f7768e;
}

QPushButton#btnDelete:disabled {
    background-color: #16161e;
    color: #2a2b3d;
    border-color: #1e1f2e;
}
)";

// ── Custom delete confirmation dialog ──────────────────────────────────────

static bool confirmDelete(const QString &name, QWidget *parent)
{
    QDialog dlg(parent);
    dlg.setWindowTitle("Delete Contact");
    dlg.setFixedSize(380, 160);
    dlg.setStyleSheet(R"(
        QDialog {
            background-color: #1a1b26;
            border: 1px solid #2a2b3d;
            border-radius: 8px;
        }
        QLabel#msgLabel {
            color: #a9b1d6;
            font-size: 13px;
        }
        QLabel#nameLabel {
            color: #c0caf5;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton {
            border-radius: 6px;
            font-size: 13px;
            font-weight: 500;
            padding: 0 20px;
            min-height: 34px;
            border: none;
        }
        QPushButton#btnConfirm {
            background-color: #f7768e;
            color: #1a1b26;
            font-weight: bold;
        }
        QPushButton#btnConfirm:hover {
            background-color: #ff8fa3;
        }
        QPushButton#btnConfirm:pressed {
            background-color: #c4526a;
        }
        QPushButton#btnCancel {
            background-color: #1e1f2e;
            color: #a9b1d6;
            border: 1px solid #3b3c52;
        }
        QPushButton#btnCancel:hover {
            background-color: #2a2b3d;
            color: #c0caf5;
        }
    )");

    auto *root    = new QVBoxLayout(&dlg);
    root->setContentsMargins(28, 24, 28, 20);
    root->setSpacing(16);

    auto *nameLabel = new QLabel(QString("Delete \"%1\"?").arg(name), &dlg);
    nameLabel->setObjectName("nameLabel");

    auto *msgLabel = new QLabel("This action cannot be undone.", &dlg);
    msgLabel->setObjectName("msgLabel");

    root->addWidget(nameLabel);
    root->addWidget(msgLabel);
    root->addStretch();

    auto *btnRow    = new QHBoxLayout;
    auto *btnCancel  = new QPushButton("Cancel",  &dlg);
    auto *btnConfirm = new QPushButton("Delete",  &dlg);
    btnCancel->setObjectName("btnCancel");
    btnConfirm->setObjectName("btnConfirm");
    btnCancel->setMinimumWidth(90);
    btnConfirm->setMinimumWidth(90);

    btnRow->addStretch();
    btnRow->addWidget(btnCancel);
    btnRow->addWidget(btnConfirm);
    root->addLayout(btnRow);

    bool confirmed = false;
    QObject::connect(btnConfirm, &QPushButton::clicked, [&]{ confirmed = true; dlg.accept(); });
    QObject::connect(btnCancel,  &QPushButton::clicked, &dlg, &QDialog::reject);

    dlg.exec();
    return confirmed;
}

// ── MainWindow ─────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet(APP_STYLE);
    setWindowTitle("Address Book");
    resize(860, 540);
    setMinimumSize(640, 400);

    // Header margins
    ui->headerLayout->setContentsMargins(20, 0, 20, 0);
    ui->toolbarLayout->setContentsMargins(20, 0, 20, 0);
    ui->toolbarLayout->setSpacing(10);
    ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

    // Table setup
    ui->tableContacts->setColumnCount(4);
    ui->tableContacts->setHorizontalHeaderLabels({"NAME", "MOBILE", "EMAIL", "BIRTHDAY"});

    QHeaderView *hh = ui->tableContacts->horizontalHeader();
    hh->setStretchLastSection(false);
    hh->setSectionResizeMode(0, QHeaderView::Stretch);
    hh->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(2, QHeaderView::Stretch);
    hh->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    hh->setHighlightSections(false);
    hh->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ui->tableContacts->verticalHeader()->setVisible(false);
    ui->tableContacts->verticalHeader()->setDefaultSectionSize(44);
    ui->tableContacts->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableContacts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableContacts->setMouseTracking(true);
    ui->tableContacts->setFocusPolicy(Qt::StrongFocus);

    connect(ui->btnAdd,    &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->btnEdit,   &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(ui->tableContacts->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
    connect(ui->tableContacts, &QTableWidget::itemDoubleClicked,
            this, &MainWindow::onEditClicked);

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
    updateContactCount();
}

void MainWindow::populateTable()
{
    ui->tableContacts->setRowCount(0);

    for (const Contact &c : m_contacts) {
        int row = ui->tableContacts->rowCount();
        ui->tableContacts->insertRow(row);

        auto makeItem = [](const QString &text) {
            auto *item = new QTableWidgetItem(text);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            return item;
        };

        ui->tableContacts->setItem(row, 0, makeItem(c.name));
        ui->tableContacts->setItem(row, 1, makeItem(c.mobile));
        ui->tableContacts->setItem(row, 2, makeItem(c.email));
        ui->tableContacts->setItem(row, 3, makeItem(c.birthday));

        ui->tableContacts->item(row, 0)->setData(Qt::UserRole, c.id);
    }
}

void MainWindow::updateContactCount()
{
    int n = m_contacts.size();
    ui->lblCount->setText(QString("%1 %2").arg(n).arg(n == 1 ? "contact" : "contacts"));
}

Contact *MainWindow::selectedContact()
{
    int row = ui->tableContacts->currentRow();
    if (row < 0 || row >= m_contacts.size()) return nullptr;

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
        if (Database::save(c)) loadContacts();
    }
}

void MainWindow::onEditClicked()
{
    Contact *c = selectedContact();
    if (!c) return;

    ContactDialog dlg(*c, this);
    if (dlg.exec() == QDialog::Accepted) {
        *c = dlg.contact();
        if (Database::save(*c)) loadContacts();
    }
}

void MainWindow::onDeleteClicked()
{
    Contact *c = selectedContact();
    if (!c) return;

    if (confirmDelete(c->name, this)) {
        Database::remove(c->id);
        loadContacts();
    }
}
