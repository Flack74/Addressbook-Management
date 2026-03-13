#include "contactdialog.h"
#include "ui_contactdialog.h"
#include "validator.h"

#include <QMessageBox>

static const QString DIALOG_STYLE = R"(
QDialog {
    background-color: #1a1b26;
}

QWidget#formWidget {
    background-color: #1a1b26;
    padding: 24px 24px 0px 24px;
}

QWidget#btnWidget {
    background-color: #1a1b26;
    padding: 0px 24px 20px 24px;
}

QLabel {
    color: #565f89;
    font-size: 12px;
    font-weight: bold;
    letter-spacing: 1px;
    min-width: 70px;
}

QLineEdit {
    background-color: #16161e;
    border: 1px solid #2a2b3d;
    border-radius: 6px;
    color: #c0caf5;
    font-size: 13px;
    padding: 8px 12px;
    selection-background-color: #283457;
}

QLineEdit:focus {
    border: 1px solid #7aa2f7;
    background-color: #1e1f2e;
}

QLineEdit:hover {
    border: 1px solid #3b3c52;
}

/* Save button */
QPushButton#btnSave {
    background-color: #7aa2f7;
    color: #1a1b26;
    border: none;
    border-radius: 6px;
    font-size: 13px;
    font-weight: bold;
    padding: 0 20px;
}

QPushButton#btnSave:hover {
    background-color: #89b4ff;
}

QPushButton#btnSave:pressed {
    background-color: #5d7fc4;
}

/* Cancel button */
QPushButton#btnCancel {
    background-color: transparent;
    color: #565f89;
    border: 1px solid #2a2b3d;
    border-radius: 6px;
    font-size: 13px;
    padding: 0 20px;
}

QPushButton#btnCancel:hover {
    background-color: #1e1f2e;
    color: #a9b1d6;
    border-color: #3b3c52;
}
)";

ContactDialog::ContactDialog(const Contact &contact, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ContactDialog)
    , m_contact(contact)
{
    ui->setupUi(this);
    setStyleSheet(DIALOG_STYLE);
    setWindowTitle(contact.id == -1 ? "Add Contact" : "Edit Contact");
    setFixedSize(420, 280);

    // Set layout margins in code since .ui doesn't support contentsMargins
    ui->formLayout->setContentsMargins(24, 24, 24, 0);
    ui->formLayout->setSpacing(14);
    ui->formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->btnLayout->setContentsMargins(24, 12, 24, 20);

    // Pre-fill when editing
    ui->editName->setText(contact.name);
    ui->editMobile->setText(contact.mobile);
    ui->editEmail->setText(contact.email);
    ui->editBirthday->setText(contact.birthday);

    ui->editBirthday->setPlaceholderText("YYYY-MM-DD");
    ui->editMobile->setPlaceholderText("+919876543210");

    connect(ui->btnSave,   &QPushButton::clicked, this, &ContactDialog::onSaveClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

ContactDialog::~ContactDialog()
{
    delete ui;
}

void ContactDialog::onSaveClicked()
{
    QString name     = ui->editName->text().trimmed();
    QString mobile   = ui->editMobile->text().trimmed();
    QString email    = ui->editEmail->text().trimmed();
    QString birthday = ui->editBirthday->text().trimmed();

    if (name.isEmpty()) {
        ui->editName->setFocus();
        ui->editName->setStyleSheet("border: 1px solid #f7768e;");
        return;
    }

    if (!mobile.isEmpty() && !Validator::isValidPhone(mobile)) {
        ui->editMobile->setFocus();
        ui->editMobile->setStyleSheet("border: 1px solid #f7768e;");
        return;
    }

    if (!email.isEmpty() && !Validator::isValidEmail(email)) {
        ui->editEmail->setFocus();
        ui->editEmail->setStyleSheet("border: 1px solid #f7768e;");
        return;
    }

    if (!birthday.isEmpty() && !Validator::isValidDate(birthday)) {
        ui->editBirthday->setFocus();
        ui->editBirthday->setStyleSheet("border: 1px solid #f7768e;");
        return;
    }

    m_contact.name     = name;
    m_contact.mobile   = mobile;
    m_contact.email    = email;
    m_contact.birthday = birthday;

    accept();
}

Contact ContactDialog::contact() const
{
    return m_contact;
}
