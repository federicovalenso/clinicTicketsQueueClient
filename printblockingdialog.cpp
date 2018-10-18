#include "printblockingdialog.h"
#include "ui_printblockingdialog.h"

PrintBlockingDialog::PrintBlockingDialog(QWidget *parent) :
    QDialog(parent, Qt::Popup),
    ui(new Ui::PrintBlockingDialog)
{
    ui->setupUi(this);
}

PrintBlockingDialog::~PrintBlockingDialog()
{
    delete ui;
}
