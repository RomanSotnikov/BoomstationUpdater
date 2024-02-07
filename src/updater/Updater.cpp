#include "Updater.hpp"
#include <Globals.hpp>
#include <QUrlQuery>
#include <QNetworkReply>

boomstation::Updater::Updater(QObject *parent) :
    QObject(parent),
    m_accessManager(new QNetworkAccessManager(this))
{}

void boomstation::Updater::checkForUpdates()
{
    collectApplicationData();
    const QUrl url = createCheckUrl();

    QNetworkRequest request;
    request.setTransferTimeout(3000);
    request.setUrl(url);

    m_reply = m_accessManager->get(request);

    QObject::connect(m_reply, &QNetworkReply::finished, this, &Updater::onResponse);
    QObject::connect(m_reply, &QNetworkReply::sslErrors, this, &Updater::onError);
}

QUrl boomstation::Updater::createCheckUrl()
{
    QUrlQuery query;
    query.addQueryItem(Globals::QUERY_SHOPUID_KEY, m_shopUID);
    query.addQueryItem(Globals::QUERY_CURRENT_VERSION_KEY, m_currentVersion);

    QUrl url = Globals::BOOMSTATION_URL;
    url.setQuery(query);
    return url;
}

void boomstation::Updater::collectApplicationData()
{

}

void boomstation::Updater::updateApplicationData()
{

}

void boomstation::Updater::startUpdate(const QUrl &url)
{

}

void boomstation::Updater::onResponse()
{

}

void boomstation::Updater::onError()
{
    qInfo() << "Error:" << m_reply->errorString();
}
