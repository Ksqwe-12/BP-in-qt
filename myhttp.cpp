#include "myhttp.h"

myhttp::myhttp()
{

}

myhttp::myhttp(QString url_get,QByteArray get_api,QString url_post,QByteArray post_api):url_A(url_get),api_A(get_api),post_A(url_post),post_api_A(post_api)
{

}

myhttp::~myhttp()
{

}


void myhttp::myGet()
{
        //QString baseUrl = "https://api.heclouds.com/devices/760182968/datastreams?datastream_ids=sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,sensor7,sensor8";
        QNetworkRequest request;
        request.setRawHeader("api-key",api_A);
        // request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)"); 伪装
        request.setUrl(QUrl(url_A));
        get_reply = manager->get(request);
        QEventLoop loop;
        connect(get_reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        one_net_json = get_reply->readAll();
        Parse_onenet_Now_Json(one_net_json);

}

void myhttp::myPost(int SP,int change,int wc)
{
        //QString baseUrl = "http://api.heclouds.com/devices/773877873/datapoints";
        QNetworkRequest request;
        request.setHeader(request.ContentTypeHeader, ("text/html; charset=utf-8"));//转格式
        request.setRawHeader("api-key",post_api_A);
        request.setUrl(QUrl(post_A));

        qDebug()<<"睡姿为："<<SP;
        QString data1;
        data1 =  QString::number(SP);
        QString data2;
        data2 =  QString::number(change);
        QString data3;
        data3 =  QString::number(wc);

        QString str1 = "{\"datastreams\": [{\"id\": \"SP\",\"datapoints\": [{\"value\": \"";
        QString str2 ="\"}]},{\"id\": \"change\",\"datapoints\": [{\"value\": \"";
        QString str3 ="\"}]},{\"id\": \"wc\",\"datapoints\": [{\"value\": \"";
        QString str4 = "\"}]},]}";

        QString json = str1+data1+str2+data2+str3+data3+str4;
        QEventLoop loop;//实例化循环体
        post_reply= manager->post(request,json.toUtf8());
        connect(post_reply, SIGNAL(finished()), &loop, SLOT(quit()));//退出循环条件
        loop.exec();//开启事件循环
        QByteArray bytes = post_reply->readAll();
        QString str = bytes;
        qDebug() << str;

}

int myhttp::Parse_onenet_Now_Json(QByteArray jsonBag)
{
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(jsonBag, &err_rpt);//字符串格式化为JSON

    if(err_rpt.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON格式错误";
        return -1;
    }
    else    //JSON格式正确
    {
                //qDebug() << "JSON格式正确：\n" << root_Doc;
                QJsonObject root_Obj = root_Doc.object();

                QString data1 = root_Obj.value("errno").toString();
                QString data2 = root_Obj.value("error").toString();
               // qDebug() << data1 + "/r" + data2 ;

                /* data键 */
                QJsonValue data_Vaule = root_Obj.value("data");
                if(data_Vaule.isArray())
                {
                    for(int idx = 0; idx<8; idx++)
                    {
                        QJsonObject data_Obj = data_Vaule.toArray().at(idx).toObject();
                        QString update_at = "update_at:" + data_Obj.value("update_at").toString();
                        QString id = "id:" + data_Obj.value("id").toString();
                        QString create_time = " create_time:" + data_Obj.value("create_time").toString();
                        current_value[idx] =data_Obj.value("current_value").toDouble();
                       // qDebug() << update_at +"/r"+ id +"/r"+ create_time +"/r"<<current_value[idx] ;
                    }
                }
     }
}
