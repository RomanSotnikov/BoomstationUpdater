#pragma once

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>

namespace boomstation
{

class Updater : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Updater)

public:
    explicit Updater(QObject *parent = nullptr);
    ~Updater() override = default;

    void checkForUpdates();

private:
    QUrl createCheckUrl();
    void collectApplicationData();
    void updateApplicationData();
    void startUpdate(const QUrl &url);

    void onResponse();
    void onError();

private:
    QNetworkAccessManager *m_accessManager;
    QNetworkReply *m_reply;

    QString m_shopUID;
    QString m_currentVersion;

}; // class Updater

} // namespace boomstation
