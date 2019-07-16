#include "printblockingdialog.h"

PrintBlockingDialog::PrintBlockingDialog(QWidget *parent)
    : QDialog(parent, Qt::Popup), ui(new Ui::PrintBlockingDialog) {
  ui->setupUi(this);
}
