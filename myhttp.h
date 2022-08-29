#ifndef MYHTTP_H
#define MYHTTP_H

#include <QWidget>
#include<QDebug>

#include<QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include<QNetworkProxy>
#include<QEventLoop>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class myhttp:public QWidget
{
    Q_OBJECT
public:
    myhttp();
    myhttp(QString url_get,QByteArray get_api,QString url_post,QByteArray post_api);
    ~myhttp();

    QString url_A;// = "https://api.heclouds.com/devices/xxxxxxx/datastreams?datastream_ids=sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,sensor7,sensor8";
    QByteArray api_A;// = "mYoMxxxxxxxxxxxxxxx8e0uo=";
    QString post_A ;//= "http://api.heclouds.com/devices/xxxxxxxxxx/datapoints";
    QByteArray post_api_A ;//= "9nSJZTxxxxxxxxxxxxxxpnvFKkCsX0=";
    QNetworkAccessManager *manager = new QNetworkAccessManager;//管理者
    QNetworkReply *post_reply;
    QNetworkReply *get_reply;

    QByteArray one_net_json;
    QVector<double>current_value{0,0,0,0,0,0,0,0};


    void myGet();
    void myPost(int SP,int change,int wc);
    int Parse_onenet_Now_Json(QByteArray jsonBag);
};

#endif // MYHTTP_H
