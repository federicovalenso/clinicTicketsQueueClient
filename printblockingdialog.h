#ifndef BLOCKINGPROGRESSDIALOG_H
#define BLOCKINGPROGRESSDIALOG_H

#include <QDialog>
#include <memory>
#include "ui_printblockingdialog.h"

namespace Ui {
class PrintBlockingDialog;
}

class PrintBlockingDialog : public QDialog {
  Q_OBJECT

 public:
  explicit PrintBlockingDialog(QWidget *parent = nullptr);
  ~PrintBlockingDialog() = default;

 private:
  std::unique_ptr<Ui::PrintBlockingDialog> ui;
};

#endif  // BLOCKINGPROGRESSDIALOG_H
