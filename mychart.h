#ifndef MYCHART_H
#define MYCHART_H
#include <QWidget>
#include"QSerialPort"
#include<QtCharts>
#include<QChart>
#include<QChartView>
#include <QLineSeries>
#include<QValueAxis>
#include <QDateTimeAxis>
#include <QVector>
#include <QTimer>
#include<QLineSeries>
#include <QRubberBand>
#include <QtMath>



class myChart : public QChart
{
public:
   myChart();
   ~myChart();

   void init();

    QChart *chart;
    QLineSeries *series;


    QValueAxis *axisX;
    QValueAxis *axisY;


    bool isClicking;
    int xOld ;
    int yOld ;

};

#endif // MYCHART_H
