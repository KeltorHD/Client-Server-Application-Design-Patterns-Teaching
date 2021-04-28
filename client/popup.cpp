#include "popup.h"
#include "ui_popup.h"

Popup::Popup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Popup)
{
    ui->setupUi(this);
}

Popup::~Popup()
{
    delete ui;
}

void Popup::set_title(const QString &title)
{
     this->setWindowTitle(title);
}

void Popup::set_description(const QString &desc)
{
    this->ui->label->setText(desc);
}
