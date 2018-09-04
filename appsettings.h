#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QMutex>

class AppSettings
{
public:
    static AppSettings& getInstance();
    void setConnectionSettings(const QString& server_address,
                               const int port);
    void setMainWindowSettings(const int action_font_size);
    void setTicketSettings(const int ticket_width,
                           const int ticket_height);
    QString getServerAddress() const;
    int getPort() const;
    int getActionFontSize() const;
    int getTicketWidth() const;
    int getTicketHeight() const;

    static const QString CONNECTION_GROUP;
    static const QString SERVER_ADDR;
    static const QString PORT;
    static const QString MAIN_WINDOW_GROUP;
    static const QString ACTION_FONT_SIZE;
    static const QString TICKET_GROUP;
    static const QString TICKET_WIDTH;
    static const QString TICKET_HEIGHT;

private:
    AppSettings();
    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(AppSettings&) = delete;
    void ReadSettings();
    QMutex m;

    QSettings settings_;
    QString server_address_;
    int port_;
    int action_font_size_;
    int ticket_width_;
    int ticket_height_;
};

#endif // SETTINGS_H
