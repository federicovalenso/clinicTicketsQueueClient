#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include <QDialog>
#include <QString>
#include "appsettings.h"

namespace Ui {
class DlgSettings;
}

class DlgSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSettings(QWidget *parent = nullptr);
    ~DlgSettings();

private slots:
    void on_buttonBox_accepted();

private:
    AppSettings& settings_ = AppSettings::getInstance();
    Ui::DlgSettings *ui;
};

#endif // DLGSETTINGS_H
