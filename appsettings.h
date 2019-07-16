#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMutex>
#include <QSettings>
#include <QString>

class AppSettings {
 public:
  static AppSettings& getInstance();
  void setConnectionSettings(const QString& server_address, const int port,
                             const QString& user, const QString& password);
  void setMainWindowSettings(const int action_font_size);
  void setTicketSettings(const int ticket_width, const int ticket_height);
  QString getServerAddress() const;
  int getPort() const;
  QString getUser() const;
  QString getPassword() const;
  int getActionFontSize() const;
  int getTicketWidth() const;
  int getTicketHeight() const;

  static const QString kConnectionGroup;
  static const QString kServerAddr;
  static const QString kPort;
  static const QString kUser;
  static const QString kPassword;
  static const QString kMainWindowGroup;
  static const QString kActionFontSize;
  static const QString kTicketGroup;
  static const QString kTicketWidth;
  static const QString kTicketHeight;

 private:
  AppSettings();
  AppSettings(const AppSettings&) = delete;
  AppSettings& operator=(AppSettings&) = delete;
  void ReadSettings();
  QMutex m;

  QSettings settings_;
  QString server_address_;
  QString user_;
  QString password_;
  int port_;
  int action_font_size_;
  int ticket_width_;
  int ticket_height_;
};

#endif  // SETTINGS_H
