#include "dlgsettings.h"
#include <QSettings>

DlgSettings::DlgSettings(QWidget *parent)
    : QDialog(parent), ui(new Ui::DlgSettings) {
  ui->setupUi(this);
  ui->leServer->setText(settings_.getServerAddress());
  ui->sbPort->setValue(settings_.getPort());
  ui->leUser->setText(settings_.getUser());
  ui->lePass->setText(settings_.getPassword());
  ui->sbFontSize->setValue(settings_.getActionFontSize());
  ui->sbTicketWidth->setValue(settings_.getTicketWidth());
  ui->sbTicketHeight->setValue(settings_.getTicketHeight());
}

void DlgSettings::on_buttonBox_accepted() {
  settings_.setConnectionSettings(ui->leServer->text(), ui->sbPort->value(),
                                  ui->leUser->text(), ui->lePass->text());
  settings_.setMainWindowSettings(ui->sbFontSize->value());
  settings_.setTicketSettings(ui->sbTicketWidth->value(),
                              ui->sbTicketHeight->value());
}
