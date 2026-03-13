#include "contactdialog.h"
#include "ui_contactdialog.h"
#include "validator.h"

#include <QMessageBox>

ContactDialog::ContactDialog(const Contact &contact, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ContactDialog)
    , m_contact(contact)
{
    ui->setupUi(this);
    setWindowTitle(contact.id == -1 ? "Add Contact" : "Edit Contact");

    // Pre-fill fields when editing
    ui->editName->setText(contact.name);
    ui->editMobile->setText(contact.mobile);
    ui->editEmail->setText(contact.email);
    ui->editBirthday->setText(contact.birthday);

    // Use placeholder text so the user knows the expected format
    ui->editBirthday->setPlaceholderText("YYYY-MM-DD");
    ui->editMobile->setPlaceholderText("+919876543210");

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ContactDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ContactDialog::~ContactDialog()
{
    delete ui;
}

void ContactDialog::onAccepted()
{
    QString name     = ui->editName->text().trimmed();
    QString mobile   = ui->editMobile->text().trimmed();
    QString email    = ui->editEmail->text().trimmed();
    QString birthday = ui->editBirthday->text().trimmed();

    // Validate before accepting — show the first error we find
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Name cannot be empty.");
        ui->editName->setFocus();
        return;
    }

    if (!mobile.isEmpty() && !Validator::isValidPhone(mobile)) {
        QMessageBox::warning(this, "Validation Error",
            "Mobile number format is invalid.\nExpected: +CountryCode followed by 7–12 digits.");
        ui->editMobile->setFocus();
        return;
    }

    if (!email.isEmpty() && !Validator::isValidEmail(email)) {
        QMessageBox::warning(this, "Validation Error",
            "Email address format is invalid.");
        ui->editEmail->setFocus();
        return;
    }

    if (!birthday.isEmpty() && !Validator::isValidDate(birthday)) {
        QMessageBox::warning(this, "Validation Error",
            "Birthday must be in YYYY-MM-DD format (e.g. 1995-07-21).");
        ui->editBirthday->setFocus();
        return;
    }

    m_contact.name     = name;
    m_contact.mobile   = mobile;
    m_contact.email    = email;
    m_contact.birthday = birthday;

    accept(); // closes the dialog with Accepted result
}

Contact ContactDialog::contact() const
{
    return m_contact;
}
