#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"QSerialPort"
#include<QtCharts>
#include<QChartView>
#include <QLineSeries>
#include <QVector>
#include <QTimer>
#include <QRubberBand>
#include <QtMath>
#include<QValueAxis>
#include<cmath>
#include"mychart.h"
#include"QDebug"
#include <stdlib.h>
#include <QString>
#include<QCharRef>
#include"mymatrix.h"
#include"peimatrix.h"
#include<QTimer>
#include<iostream>
#include<stdlib.h>
#include<myhttp.h>
#include<mythread.h>
#include<QtSql/QSqlDatabase>

using namespace std;
#define N 12
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

   void System_int();//初始化串口，连接信号和槽
   void initChart();//初始化图表
   void Setort();//设置串口配置
   void Disconnect();//断开连接，清空数据
   void Receive();//接收数据并处理
   void prediv(int i , double j);//更新曲线
   void update( int num);//更新函数
   int div(QString st);//匹配数据与通道
   void tiao(int num , double dd);//调零函数
   double average(QVector<double> arr);//平均值函数，标定用，lcd显示
   QVector<double> Smooth(QVector<double> arr);//输出窗口滤波，长度为12
   void vectorclear();//清空vector
   void SPDvectorclear();
   void GetData();

   void HeadJudge(double num);
   void ShoulderJudge(double num1,double num2,double num3,double num4);
   void LegJudge(double num1,double num2,double num3);
   void httpTiao();

   void zhen_ShiBie();
   void zhen_ShiBieB();

   mythread *mtd ;
   QThread *thread ;

   QImage _image;

   //矩阵运算（识别过程）
   MatrixXd m_input = MatrixXd::Ones(1,8);//输入矩阵
   MatrixXd hidden_matrix1 = MatrixXd::Ones(8,9);//运算矩阵1
   MatrixXd hidden_matrix2 = MatrixXd::Ones(9,9);//运算矩阵2
   MatrixXd output_matrix3 = MatrixXd::Ones(9,6);//运算矩阵3
   MatrixXd net_b1 = MatrixXd::Ones(1,9);//偏置1
   MatrixXd net_b2 = MatrixXd::Ones(1,9);//偏置2
   MatrixXd output_b3 = MatrixXd::Ones(1,6);//偏置3
   MatrixXd m_output = MatrixXd::Ones(1,6);

   //矩阵运算（识别过程）
   MatrixXd m_inputB = MatrixXd::Ones(1,8);//输入矩阵
   MatrixXd hidden_matrix1B = MatrixXd::Ones(8,9);//运算矩阵1
   MatrixXd hidden_matrix2B = MatrixXd::Ones(9,9);//运算矩阵2
   MatrixXd output_matrix3B = MatrixXd::Ones(9,6);//运算矩阵3
   MatrixXd net_b1B = MatrixXd::Ones(1,9);//偏置1
   MatrixXd net_b2B = MatrixXd::Ones(1,9);//偏置2
   MatrixXd output_b3B = MatrixXd::Ones(1,6);//偏置3
   MatrixXd m_outputB = MatrixXd::Ones(1,6);

   PeiMatrix m_Matrix;
   myMatrix m_MatrixB;

   MatrixXd ReLu(MatrixXd m);
   MatrixXd ReLuB(MatrixXd m);
   void SPD();//分析函数
   void SPrecognization();//识别
   void SPrecognizationB();
   void WCBuffer();//
   void WCBufferB();//
   void Empty_judge();
   void WC_judge();
   void WC_judgeB();
//   void BedEmpty();


private:
    Ui::Widget *ui;
    QSerialPort *global_port;
    QByteArray plain_Date;//保存缓冲区数据
    QString st;//合并接收到的数据
    int flag = 1;//用作调零标志
    int httpFlag = 1;
    myChart* c1 = new myChart();//自定义图表实例化
    myChart* c2 = new myChart();
    myChart* c3 = new myChart();
    myChart* c4 = new myChart();
    myChart* c5 = new myChart();
    myChart* c6 = new myChart();
    myChart* c7 = new myChart();
    myChart* c8 = new myChart();

    myChart* ca = new myChart();//自定义图表实例化
    myChart* cb = new myChart();
    myChart* cc = new myChart();
    myChart* cd = new myChart();
    myChart* ce = new myChart();
    myChart* cf = new myChart();
    myChart* cg = new myChart();
    myChart* ch = new myChart();


    QVector<myChart*> c_array;
    QVector<myChart*> c_arrayB;

    QVector<QLineSeries*> c_series; //存放线段的Vector
    QVector<QLineSeries*> c_serieB;

    double preV[8]={0,0,0,0,0,0,0,0};//调零数据
    double preVA[8]={0,0,0,0,0,0,0,0};//调零数据
    double preVB[8]={0,0,0,0,0,0,0,0};//调零数据

    QVector<double>SPD_vector{0,0,0,0,0,0,0,0};//存放当前时间点的数据
    QVector<double>SPD_vector_f{0,0,0,0,0,0,0,0};//斜率判断
    QVector<double>SPD_vectorB{0,0,0,0,0,0,0,0};//存放当前时间点的数据
    QVector<double>SPD_vectorB_f{0,0,0,0,0,0,0,0};//斜率判断

    QVector<int> Tarray;//存放特征值
    //QVector<int>SP_now ={0,0,0} ;//当前睡姿

    int change_number=0;//睡姿改变次数
    int wc_number=-1;//起夜次数
    int fall = 0;//是否坠床？
    int Posture = 0;//睡姿

    int change_numberB=0;//睡姿改变次数
    int wc_numberB=-1;//起夜次数
    int fallB = 0;//是否坠床？
    int PostureB = 0;//睡姿


    QVector<QDateTime>up_dateTime;//上床时间点
    QVector<QDateTime>down_dateTime;//下床时间点
    //QDateTime sleep_dateTime;

    //http请求实例化
    QString url_A = "https://api.heclouds.com/devices/xxxxxxxx/datastreams?datastream_ids=sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,sensor7,sensor8";
    QByteArray api_A = "mYxxxxxxxxxxxxxxo=";
    QString post_A = "http://api.heclouds.com/devices/xxxxxxxxxx/datapoints";
    QByteArray post_api_A = "9nSJZTxxxxxxxxxxxFpnvFKkCsX0=";
    myhttp* http = new myhttp(url_A,api_A,post_A,post_api_A);

    QString url_get_B = "https://api.heclouds.com/devices/xxxxxxxxxxxx/datastreams?datastream_ids=sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,sensor7,sensor8";
    QByteArray api_get_B = "UNHxxxxxxxxxxxxxxxxxCvg=";
    QString url_post_B = "http://api.heclouds.com/devices/xxxxxxxxxxxxxxx/datapoints";
    QByteArray api_post_B = "zcgx3TFoxxxxxxxxxxxxxw=";
    myhttp* http_B = new myhttp(url_get_B,api_get_B,url_post_B,api_post_B);


    QPixmap *pixmap1=new QPixmap(":/pix/p.png");
    QPixmap *pixmap4=new QPixmap(":/pix/fuwo.png");
    QPixmap *pixmap5=new QPixmap(":/pix/youce.png");
    QPixmap *pixmap6=new QPixmap(":/pix/youcequtui.png");
    QPixmap *pixmap2=new QPixmap(":/pix/zuoce.png");
    QPixmap *pixmap3=new QPixmap(":/pix/zuocequtui.png");
    QPixmap *pixmap7=new QPixmap(":/pix/warning.png");
    QPixmap *pixmap8=new QPixmap(":/pix/kongchuang.png");



    //存放八个通道的数据
    QVector<double> v1;
    QVector<double> v2;
    QVector<double> v3;
    QVector<double> v4;
    QVector<double> v5;
    QVector<double> v6;
    QVector<double> v7;
    QVector<double> v8;
    //记录存放了多少组数据
    int receiveNumber = 0;
    int httpNumber = 0;
    //存放识别数据 每个通道50
    QVector<double> SPD_v1;
    QVector<double> SPD_v2;
    QVector<double> SPD_v3;
    QVector<double> SPD_v4;
    QVector<double> SPD_v5;
    QVector<double> SPD_v6;
    QVector<double> SPD_v7;
    QVector<double> SPD_v8;
    //三时间点睡姿
    QVector<int>WCjudge_1={0,0,0};
    QVector<int>WCjudge_2={0,0,0};

signals:
    void runThread();

};
#endif // WIDGET_H
