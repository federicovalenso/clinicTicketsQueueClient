#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QPrinter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnBook_clicked();
    void on_btnDisp_clicked();
    void on_btnReceipt_clicked();
    void on_btnEmergency_clicked();
    void on_btnReport_clicked();
    void on_btnVaccine_clicked();
    void on_btnOther_clicked();
    void replyFinished(QNetworkReply*);

private:
    static const QString TICKET_NUMBER_PARAM;
    static const QString ACTION_PARAM;
    static const QString BOOK_ACTION;
    static const QString DISP_ACTION;
    static const QString RECEIPT_ACTION;
    static const QString EMERGENCY_ACTION;
    static const QString REPORT_ACTION;
    static const QString VACCINE_ACTION;
    static const QString OTHER_ACTION;
    static const QMap<QString, QString> ACTIONS_DESCRIBES;
    Ui::MainWindow *ui;
    QNetworkAccessManager *mNetworkManager;

    void sendTicketRequest(const QString& action);
    void printTicket(const QJsonDocument& ticket);
};

#endif // MAINWINDOW_H
