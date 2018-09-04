#include <QSettings>
#include "dlgsettings.h"
#include "ui_dlgsettings.h"

DlgSettings::DlgSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSettings)
{
    ui->setupUi(this);
    ui->leServer->setText(settings_.getServerAddress());
    ui->sbPort->setValue(settings_.getPort());
    ui->sbFontSize->setValue(settings_.getActionFontSize());
    ui->sbTicketWidth->setValue(settings_.getTicketWidth());
    ui->sbTicketHeight->setValue(settings_.getTicketHeight());
}

DlgSettings::~DlgSettings()
{
    delete ui;
}

void DlgSettings::on_buttonBox_accepted()
{
    settings_.setConnectionSettings(ui->leServer->text(), ui->sbPort->value());
    settings_.setMainWindowSettings(ui->sbFontSize->value());
    settings_.setTicketSettings(ui->sbTicketWidth->value(), ui->sbTicketHeight->value());
}
