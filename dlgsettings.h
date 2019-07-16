#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include <QDialog>
#include <QString>
#include <memory>
#include "appsettings.h"
#include "ui_dlgsettings.h"

namespace Ui {
class DlgSettings;
}

class DlgSettings : public QDialog {
  Q_OBJECT

 public:
  explicit DlgSettings(QWidget *parent);
  ~DlgSettings() = default;

 private slots:
  void on_buttonBox_accepted();

 private:
  AppSettings &settings_ = AppSettings::getInstance();
  std::unique_ptr<Ui::DlgSettings> ui;
};

#endif  // DLGSETTINGS_H
