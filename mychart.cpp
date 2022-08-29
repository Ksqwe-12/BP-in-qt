#include "mychart.h"

myChart::myChart():
     chart(new QChart)
   ,series(new QLineSeries)
   ,axisX(new QValueAxis) , axisY(new QValueAxis)
    ,isClicking(false)
   ,xOld(0), yOld(0)
{

}

void  myChart :: init()
{
    chart->addSeries(series);//加入曲线
    chart->setBackgroundVisible(false);

    axisX->setLabelsColor(QColor(0,0,0));
    axisY->setLabelsColor(QColor(0,0,0));
    axisX->setLinePenColor(QColor(0,0,0));
    axisY->setLinePenColor(QColor(0,0,0));

    axisY->setTickCount(5);  //设置网格数

    axisY->setMin(0); //设置刻度范围
    axisY->setMax(20);
    axisX->setMin(0);
    axisX->setMax(20);
    axisX->setLabelFormat("%d");
    /***--设置文字和字体--****/
    axisX->setTitleFont(QFont("Microsoft YaHei", 5, QFont::Normal, true));
    axisY->setTitleFont(QFont("Microsoft YaHei", 5, QFont::Normal, true));
   // axisX->setTitleText("Time/s");
   // axisY->setTitleText("Voltage/mV");
    //不显示网格
    axisX->setGridLineVisible(false);
   // axisY->setGridLineVisible(false);
    //将坐标轴加入图表
    chart->setAxisX(axisX,series);
    chart->setAxisY(axisY,series);


    //隐藏标题
    chart->legend()->hide();
    chart->layout()->setContentsMargins(0, 0, 0, 0);//设置外边界全部为0
    chart->setMargins(QMargins(0, 0, 0, 0));//设置内边界全部为0
    chart->setBackgroundRoundness(0);//设置背景区域无圆角


}


myChart::~myChart()
{

}
