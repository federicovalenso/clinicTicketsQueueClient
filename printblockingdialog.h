#ifndef BLOCKINGPROGRESSDIALOG_H
#define BLOCKINGPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class PrintBlockingDialog;
}

class PrintBlockingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrintBlockingDialog(QWidget *parent = nullptr);
    ~PrintBlockingDialog();

private:
    Ui::PrintBlockingDialog *ui;
};

#endif // BLOCKINGPROGRESSDIALOG_H
