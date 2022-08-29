#include "widget.h"
#include "ui_widget.h"
#include <QChart>
#include<QChartView>
#include<QFileDialog>
#include <QCoreApplication>
#include<QTabWidget>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include"iostream"
#include<QLabel>
#include<QPixmap>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,global_port(new QSerialPort)

{
    ui->setupUi(this);
//    _image.load(":/pix/fuwo.png");
//    setAutoFillBackground(true);   // 这个属性一定要设置
//    QPalette pal(palette());
//    pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
//    setPalette(pal);
    System_int();
    initChart();

}

void Widget::System_int()
{

    global_port->setStopBits(QSerialPort::OneStop);
    global_port->setParity(QSerialPort::NoParity);
    global_port->setDataBits(QSerialPort::Data8);
    global_port->setFlowControl(QSerialPort::NoFlowControl);

    //初始化模型

    qDebug()<< m_Matrix.m1(0,1);
    m_Matrix.Matrix_init();
    hidden_matrix1 = m_Matrix.m1;//运算矩阵1
    hidden_matrix2 = m_Matrix.m2;//运算矩阵2
    output_matrix3 = m_Matrix.m3;//运算矩阵3
    net_b1 = m_Matrix.b1;//偏置1
    net_b2 = m_Matrix.b2;//偏置2
    output_b3 = m_Matrix.b3;//偏置3
    qDebug()<<hidden_matrix1(0,1);

    m_MatrixB.Matrix_init();
    hidden_matrix1B = m_MatrixB.m1;//运算矩阵1
    hidden_matrix2B = m_MatrixB.m2;//运算矩阵2
    output_matrix3B = m_MatrixB.m3;//运算矩阵3
    net_b1B = m_MatrixB.b1;//偏置1
    net_b2B = m_MatrixB.b2;//偏置2
    output_b3B = m_MatrixB.b3;//偏置3

    //创建子线程
    mtd = new mythread;
    thread = new QThread;
    //将定时器加入子线程
    mtd->moveToThread(thread);
    thread->start();
    connect(mtd,&mythread::mySignal,this,&Widget::SPD);
    connect(this,&Widget::runThread,mtd,&mythread::myTimeout);

    //建立关系
    connect(ui->pushButton_3,&QPushButton::clicked,[=]{
        ui->tabWidget->setCurrentIndex(1);

    });
    connect(ui->pushButton,&QPushButton::clicked,this,&Widget::Setort);//设置串口
    connect(ui->pushButton_2,&QPushButton::clicked,[=]{
        Disconnect();//断开连接
        vectorclear();//清空所有数据

    });
    connect(global_port,&QSerialPort::readyRead,this,&Widget::Receive);
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,[=]{
        int tabNum = ui->tabWidget->currentIndex();
        ui->tabWidget_2->setCurrentIndex(tabNum);
    });
    connect(ui->pushButton_5,&QPushButton::clicked,[=]{
        QFileDialog fileDialog;
        QString fileName = fileDialog.getSaveFileName(this,tr("Open File"),"/SPD",tr("Text File(*.xls)"));
        QFile file(fileName);
        bool ret = file.open(QIODevice::WriteOnly);
        if(ret==false)
        {
            return;
        }
        QTextStream out(&file);

        int time = Tarray.length()/4;

        for(int i = 0; i<time; i++)
        {
            for(int j=0; j<4; j++)
            {
                out<<Tarray[i*4+j];
                out<<"\t";
            }
            out<<"\r\n";
        }
        Tarray.clear();

        file.close();
        //输出特征到execl（弃用）
    });

    connect(ui->pushButton_4,&QPushButton::clicked,[=]{
       //平滑处理后的段数据
       QVector<double> s_arr1 = Smooth(v1);
       QVector<double> s_arr2 = Smooth(v2);
       QVector<double> s_arr3 = Smooth(v3);
       QVector<double> s_arr4 = Smooth(v4);
       QVector<double> s_arr5 = Smooth(v5);
       QVector<double> s_arr6 = Smooth(v6);
       QVector<double> s_arr7 = Smooth(v7);
       QVector<double> s_arr8 = Smooth(v8);

       vectorclear();//清空八通道数据

       QVector<double> mynumarry[8] = {s_arr1,s_arr2,s_arr3,s_arr4,s_arr5,s_arr6,s_arr7,s_arr8};//储存平滑后的段数据

       QFileDialog fileDialog;
       QString fileName = fileDialog.getSaveFileName(this,tr("Open File"),"/mydata",tr("Text File(*.xls)"));
       QFile file(fileName);
       bool ret = file.open(QIODevice::WriteOnly);
       if(ret==false)
       {
           return;
       }
       QTextStream out(&file);

       int time = mynumarry[7].length();

       for(int i = 0; i<time; i++)
       {
           for(int j=0; j<8; j++)
           {
               out<<mynumarry[j][i];
               out<<"\t";
           }
           out<<"\r\n";
       }

       file.close();
       //输出该时间内平滑处理后的八通道数据

    });
    connect(ui->pushButton_3,&QPushButton::clicked,[=]{
        flag = 1;
        httpFlag = 0;
    });//调0


    //线段组
    c_series.append(c1->series);
    c_series.append(c2->series);
    c_series.append(c3->series);
    c_series.append(c4->series);
    c_series.append(c5->series);
    c_series.append(c6->series);
    c_series.append(c7->series);
    c_series.append(c8->series);

    c_serieB.append(ca->series);
    c_serieB.append(cb->series);
    c_serieB.append(cc->series);
    c_serieB.append(cd->series);
    c_serieB.append(ce->series);
    c_serieB.append(cf->series);
    c_serieB.append(cg->series);
    c_serieB.append(ch->series);


    //mychart组
    c_array.append(c1);
    c_array.append(c2);
    c_array.append(c3);
    c_array.append(c4);
    c_array.append(c5);
    c_array.append(c6);
    c_array.append(c7);
    c_array.append(c8);

    c_arrayB.append(ca);
    c_arrayB.append(cb);
    c_arrayB.append(cc);
    c_arrayB.append(cd);
    c_arrayB.append(ce);
    c_arrayB.append(cf);
    c_arrayB.append(cg);
    c_arrayB.append(ch);




    ui->plainTextEdit->appendPlainText("初始化");

}

double Widget::average(QVector<double> arr)//平均值输出
{
    int len = arr.size();
        double sum = 0;
        int i;
        for ( i = 0;i<len;i++){
            sum += arr[i];
        }
        return sum / (len)/1.0;

}

void Widget::Setort()//设置串口参数
{


    global_port->setBaudRate(9600);
    global_port->setPortName(ui->comboBox->currentText());
    global_port->setReadBufferSize(1024);
    global_port->open(QIODevice::ReadWrite);
    global_port->flush();

    flag = 1;

    //启动线程，但函数未启动。需要用信号和槽来启动
    thread->start();
    mtd->setFlag(1);

    //启动线程函数
    emit runThread();

    qDebug()<<"主："<<QThread::currentThread();
    ui->plainTextEdit->appendPlainText("成功");
}

void Widget::initChart()//初始化图表
{
    c1->init();
    c2->init();
    c3->init();
    c4->init();
    c5->init();
    c6->init();
    c7->init();
    c8->init();

    ca->init();
    cb->init();
    cc->init();
    cd->init();
    ce->init();
    cf->init();
    cg->init();
    ch->init();


    //pixmap->scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(*pixmap8);
    ui->label_15->setScaledContents(true);
    ui->label_15->setPixmap(*pixmap8);


//    QPixmap *pixmap7=new QPixmap(":/pix/sp_a.png");
//    QPixmap *pixmap8=new QPixmap(":/pix/sp_a.png");





    ui->widget->setChart(c1->chart);
    ui->widget_2->setChart(c2->chart);
    ui->widget_3->setChart(c3->chart);
    ui->widget_4->setChart(c4->chart);
    ui->widget_5->setChart(c5->chart);
    ui->widget_6->setChart(c6->chart);
    ui->widget_7->setChart(c7->chart);
    ui->widget_8->setChart(c8->chart);

    ui->widget_a->setChart(ca->chart);
    ui->widget_b->setChart(cb->chart);
    ui->widget_c->setChart(cc->chart);
    ui->widget_d->setChart(cd->chart);
    ui->widget_e->setChart(ce->chart);
    ui->widget_f->setChart(cf->chart);
    ui->widget_g->setChart(cg->chart);
    ui->widget_h ->setChart(ch->chart);

    ui->widget->show();
    ui->widget_2->show();
    ui->widget_3->show();
    ui->widget_4->show();
    ui->widget_5->show();
    ui->widget_6->show();
    ui->widget_7->show();
    ui->widget_8->show();

    ui->widget_a->show();
    ui->widget_b->show();
    ui->widget_c->show();
    ui->widget_d->show();
    ui->widget_e->show();
    ui->widget_f->show();
    ui->widget_g->show();
    ui->widget_h->show();



}

void Widget::Disconnect()//断开连接，还原图表，清空所有数据
{

    ui->label->setPixmap(*pixmap8);
    ui->label_15->setPixmap(*pixmap8);
    global_port->clear();
    global_port->close();

    mtd->setFlag(0);
    thread->exit();
    thread->wait();

    for (int i = 0; i <8; i++)
    {
        c_series[i]->clear();
        c_serieB[i]->clear();
    }

    receiveNumber=0;
    httpNumber = 0;

    for(int j=0; j<8; j++)
    {
        c_array[j]->axisX->setMin(0);
        c_array[j]->axisX->setMax(20);
        c_arrayB[j]->axisX->setMin(0);
        c_arrayB[j]->axisX->setMax(20);
    }

    ui->plainTextEdit->setPlainText("已断开");
    QString str1;
    str1 =  QString::number(change_number,'f',0);
    str1 = "翻身次数："+str1;
    QString str2;
    str2 =  QString::number(wc_number,'f',0);
    str2 = "起夜次数："+str2;
    ui->plainTextEdit->appendPlainText("本次睡眠情况如下：");
    ui->plainTextEdit->appendPlainText(str1);
    ui->plainTextEdit->appendPlainText(str2);
    ui->plainTextEdit->appendPlainText("根据您的睡眠状况，给您提出以下建议：");
    wc_number=0;
    change_number=0;
    for(int i=0;i<3;i++)
    {
        WCjudge_1[i]=0;
    }

}



void Widget::Receive()//读串口数据
{
   int pos = 0;
   int num = 0;
   QString dex;

   plain_Date =  global_port->readAll();
//   long t1 = GetTickCount();
//   qDebug()<<"收到数据时间："<<t1;
    st +=plain_Date;
    if(st[st.length()-1]=='\n')
    {
         while(st.indexOf(QRegExp("'*'"), 0) != -1)
         {
                   if(st[pos]=='A')
                   {
                           receiveNumber++;
                           //qDebug()<<receiveNumber;
                   }
                   if((st[pos]<='A'+7)&&(st[pos]>='A'))
                   {
                   dex = st[pos];//通道位
                   num =div(dex);//A转1
                   st = st.remove(0, pos+1);
                   update(num);//将通道数传入更新图表函数
                   pos = 0;
                   }
                   else
                   {
                       if(st[pos]==0x27&&st[pos+1]<'A')
                       {
                           st= st.remove(0,1);
                           break;
                       }
                       pos++;
                   }

          }
         if(receiveNumber%30==0)//每30个数据判断一次睡姿
         {
//            long t1 = GetTickCount();
//            qDebug()<<"收到数据时间："<<t1;
//            SPD();
//            long t2 = GetTickCount();
//            qDebug()<<"处理完毕时间："<<t2;
         }
         st.clear();//清空临时空间
    }
//    long t2 = GetTickCount();
//    qDebug()<<"处理完毕时间："<<t2;

}

int Widget::div(QString str)
{
   if(str=='A')
   {
       return 0;
   }
   else if(str=='B')
   {
       return 1;
   }
   else if(str=='C')
   {
       return 2;
   }
   else if(str=='D')
   {
       return 3;
   }
   else if(str=='E')
   {
       return 4;
   }
   else if(str=='F')
   {
       return 5;
   }
   else if(str=='G')
   {
       return 6;
   }
   else if(str=='H')
   {
       return 7;
   }

   return 0;
}

void Widget::update( int num)//接收数据更新函数  123456.789
{
    int p = 0;
    int pos = 1;
        while(st[pos] != 0x27)
        {
            pos++;          
        }
        p = pos;

        QByteArray ddd = (st.mid(1,p-1)).toLocal8Bit();//123456 .123
        const char *ss = ddd;//123456.123
        st = st.remove(0, pos+1);
        double mmm = int(atof(ss))/1000.0;//123.4
        if(receiveNumber==10 || flag%9!=0)
        {
        tiao(num,mmm) ;//用的s  123.4  更新调零数组
        }
        double mynum = mmm-preV[num];//用的 s 波动值123.4-123.4
        if(receiveNumber>=10)//从第10个数据开始记录
        {
        switch(num)//更新通道数组
        {
            case 0 : v1.append(mynum);SPD_v1.append(mynum);break;
            case 1 : v2.append(mynum);SPD_v2.append(mynum);break;
            case 2 : v3.append(mynum);SPD_v3.append(mynum);break;
            case 3 : v4.append(mynum);SPD_v4.append(mynum);break;
            case 4 : v5.append(mynum);SPD_v5.append(mynum);break;
            case 5 : v6.append(mynum);SPD_v6.append(mynum);break;
            case 6 : v7.append(mynum);SPD_v7.append(mynum);break;
            case 7 : v8.append(mynum);SPD_v8.append(mynum);break;
        }
        }
        prediv( num , mynum);     
}

void Widget::tiao(int num ,double dd)//调零
{
    preV[num] = dd;
    flag ++;
    if(flag%9==0)
    {
        flag = 0;
    }
}

void Widget::prediv(int i,double j)//更新图表A
{
    if(receiveNumber>=20)
    {
        for(int j=0; j<8; j++)
        {
            c_array[j]->axisX->setMin(receiveNumber-20);
            c_array[j]->axisX->setMax((receiveNumber));
        }
    }
    c_series[i]->append(receiveNumber,j);
}

QVector<double> Widget::Smooth(QVector<double> arr)//平滑滤波
{
    double Sum1=0;
    int size = arr.length();
        for(int j=0;j<size;j++)
        {
            if(j<N/2)
            {
                for(int k=0;k<N;k++)
                {
                    Sum1+=arr[j+k];
                }
                arr[j]=Sum1/N;
            }
            else
                if(j<size -N/2)
                {
                    for(int k=0;k<N/2;k++)
                    {
                        Sum1+=(arr[j+k]+arr[j-k]);
                    }
                    arr[j]=Sum1/N;
                }
                else
                {
                    for(int k=0;k<size-j;k++)
                    {
                        Sum1+=arr[j+k];
                    }
                    for(int k=0;k<(N-size+j);k++)
                    {
                        Sum1+=arr[j-k];
                    }
                    arr[j]=Sum1/N;
                }
            Sum1=0;
        }
     QVector<double> m_arr = arr;
      return  m_arr;

}



void Widget::vectorclear()//清空八通道原始数据
{
    v1.clear();
    v2.clear();
    v3.clear();
    v4.clear();
    v5.clear();
    v6.clear();
    v7.clear();
    v8.clear();
}

void Widget::SPDvectorclear()//清空容量为30的临时数组
{
    SPD_v1.clear();
    SPD_v2.clear();
    SPD_v3.clear();
    SPD_v4.clear();
    SPD_v5.clear();
    SPD_v6.clear();
    SPD_v7.clear();
    SPD_v8.clear();
}

MatrixXd Widget::ReLu(MatrixXd m)
{
    MatrixXd h=m;
    for(int i =0;i<9;i++)
    {
        h(0,i)=max(0.0,h(0,i));
    }
    return h;

}

MatrixXd Widget::ReLuB(MatrixXd m)
{
    MatrixXd h=m;
    for(int i =0;i<8;i++)
    {
        h(0,i)=max(0.0,h(0,i));
    }
    return h;

}

void Widget:: SPD()//睡姿分析
{

    httpNumber++;
    GetData();//更新图B从云平台下载数据

    if((abs(SPD_vector_f[0]-SPD_vector[0])<0.5)&&(abs(SPD_vector_f[1]-SPD_vector[1])<0.5)&&(abs(SPD_vector_f[2]-SPD_vector[2])<0.5)&&(abs(SPD_vector_f[3]-SPD_vector[3])<1)&&(abs(SPD_vector_f[4]-SPD_vector[4])<1)&&(abs(SPD_vector_f[5]-SPD_vector[5])<1)&&(abs(SPD_vector_f[6]-SPD_vector[6])<1)&&(abs(SPD_vector_f[7]-SPD_vector[7])<1))
    {
    SPrecognization();//睡姿判断
    Empty_judge();//判段空床
    WCBuffer();//buffer移动
    WC_judge();//各种判断
    //SPDvectorclear();//清空30数据的临时数组
    //在床标志位，睡姿
    http->myPost(Posture,change_number,wc_number);//上传识别结果
    ui->lcdNumber_2->display(change_number);
    }
    if((abs(SPD_vectorB_f[0]-SPD_vectorB[0])<0.5)&&(abs(SPD_vectorB_f[1]-SPD_vectorB[1])<0.5)&&(abs(SPD_vectorB_f[2]-SPD_vectorB[2])<0.5)&&(abs(SPD_vectorB_f[3]-SPD_vectorB[3])<1)&&(abs(SPD_vectorB_f[4]-SPD_vectorB[4])<1)&&(abs(SPD_vectorB_f[5]-SPD_vectorB[5])<1)&&(abs(SPD_vectorB_f[6]-SPD_vectorB[6])<1)&&(abs(SPD_vectorB_f[7]-SPD_vectorB[7])<1))
    {
    SPrecognizationB();//睡姿判断
    Empty_judge();//判段空床
    WCBufferB();//buffer移动
    WC_judgeB();//各种判断
    //SPDvectorclear();//清空30数据的临时数组
    //在床标志位，睡姿
    http_B->myPost(PostureB,change_numberB,wc_numberB);//上传识别结果
    ui->lcdNumber_2->display(change_numberB);
    }


    for (int i=0;i<8;i++)
    {
        SPD_vector_f[i]=SPD_vector[i];
        SPD_vectorB_f[i]=SPD_vectorB[i];
    }
}

void Widget::SPrecognization()
{
    int maxPosition=0;//计算睡姿
    m_input<<SPD_vector[0],SPD_vector[1],SPD_vector[2],SPD_vector[3],SPD_vector[4],SPD_vector[5],SPD_vector[6],SPD_vector[7];
    m_output = ReLu(ReLu(m_input*hidden_matrix1+net_b1)*hidden_matrix2+net_b2)*output_matrix3+output_b3;
    QVector<double> result {m_output(0,0),m_output(0,1),m_output(0,2),m_output(0,3),m_output(0,4),m_output(0,5)};
    maxPosition = max_element(result.begin(),result.end()) - result.begin();
    Posture = maxPosition+1;
}

void Widget::SPrecognizationB()
{
    int maxPosition=0;//计算睡姿
    m_inputB<<SPD_vectorB[0],SPD_vectorB[1],SPD_vectorB[2],SPD_vectorB[3],SPD_vectorB[4],SPD_vectorB[5],SPD_vectorB[6],SPD_vectorB[7];
    m_outputB = ReLu(ReLu(m_inputB*hidden_matrix1B+net_b1B)*hidden_matrix2B+net_b2B)*output_matrix3B+output_b3B;
    QVector<double> result {m_outputB(0,0),m_outputB(0,1),m_outputB(0,2),m_outputB(0,3),m_outputB(0,4),m_outputB(0,5)};
    maxPosition = max_element(result.begin(),result.end()) - result.begin();
    PostureB = maxPosition+1;
}

void Widget::WCBuffer()
{
    QVector<int>::iterator k = WCjudge_1.begin();
    WCjudge_1.erase(k);
    WCjudge_1.append(Posture);
}

void Widget::WCBufferB()
{
    QVector<int>::iterator k = WCjudge_2.begin();
    WCjudge_2.erase(k);
    WCjudge_2.append(PostureB);
}

void Widget::Empty_judge()//空床判断
{
    double leg = max(max(SPD_vector[0],SPD_vector[1]),max(SPD_vector[2],SPD_vector[3]));
    double body = max(max(SPD_vector[0],max(SPD_vector[6],SPD_vector[7])),SPD_vector[4]);
    if(abs(body)<1&&abs(leg)<1)
    {
        Posture = 0;
    }
    else if (abs(body)<1&&abs(leg)>1)
    {
        Posture = 7;
    }
    double legB = max(max(SPD_vectorB[0],SPD_vectorB[1]),max(SPD_vectorB[2],SPD_vectorB[3]));
    double bodyB = max(max(SPD_vectorB[0],max(SPD_vectorB[6],SPD_vectorB[7])),SPD_vectorB[4]);
    if(abs(bodyB)<1&&abs(legB)<2&&SPD_vectorB[4]<2)
    {
        PostureB = 0;
    }
    else if (abs(bodyB)<1&&abs(legB)>1)
    {
        PostureB = 7;
    }
}

void Widget::WC_judge()//各种判断
{
    if(WCjudge_1[0]==0)
    {
        //上床过程
        if(WCjudge_1[1]==0)
        {
            if(WCjudge_1[2]!=0)
            {
                //回床ing
                wc_number++;             
                ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"上床");
                if(WCjudge_1[2]!=7)
                {
                    change_number++;
                    zhen_ShiBie();
                }
                return;
            }
            else
            {
                //空床
                //ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"空床");
                return;
            }
        }
        else{
            if(WCjudge_1[1]!=WCjudge_1[2])
            {
                change_number++;
                zhen_ShiBie();
                return;
            }
            ui->lcdNumber->display(wc_number);
            return;
        }
    }
    else{
        //下床或改变睡姿
        if(WCjudge_1[2]!=0&&WCjudge_1[2]!=7)
        {
            //改变睡姿
            if(WCjudge_1[2]!=WCjudge_1[1])
            {
                change_number++;
                zhen_ShiBie();
                return;
            }
            else {
                //睡姿未变
                //ui->plainTextEdit->appendPlainText("");
                return;
            }
        }
        if(WCjudge_1[2]==0)
        {
            if(WCjudge_1[1]==7)
            {
                //起身起夜
                wc_number++;
                ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"离床起夜");
                ui->label->setPixmap(*pixmap8);
                ui->lcdNumber->display(wc_number);
                return;
            }
            else if(WCjudge_1[1]!=0)
            {
                //坠床
                fall=1;
                ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"警告！！！！！！！！已坠床");
                ui->label->setPixmap(*pixmap7);
                return;
            }
        }
    }
}

void Widget::WC_judgeB()//各种判断
{
    if(WCjudge_2[0]==0)
    {
        //上床过程
        if(WCjudge_2[1]==0)
        {
            if(WCjudge_2[2]!=0)
            {
                //回床ing
                wc_numberB++;
                ui->plainTextEdit_2->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"上床");
                if(WCjudge_2[2]!=7)
                {
                    change_numberB++;
                    zhen_ShiBieB();
                }
                return;
            }
            else
            {
                //空床
                //ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"空床");
                return;
            }
        }
        else{
            if(WCjudge_2[1]!=WCjudge_2[2])
            {
                change_numberB++;
                zhen_ShiBieB();
                return;
            }
            ui->lcdNumber->display(wc_numberB);
            return;
        }
    }
    else{
        //下床或改变睡姿
        if(WCjudge_2[2]!=0&&WCjudge_2[2]!=7)
        {
            //改变睡姿
            if(WCjudge_2[2]!=WCjudge_2[1])
            {
                change_numberB++;
                zhen_ShiBieB();
                return;
            }
            else {
                //睡姿未变
                //ui->plainTextEdit->appendPlainText("");
                return;
            }
        }
        if(WCjudge_2[2]==0)
        {
            if(WCjudge_2[1]==7)
            {
                //起身起夜
                wc_numberB++;
                ui->plainTextEdit_2->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"离床起夜");
                ui->label_15->setPixmap(*pixmap8);
                ui->lcdNumber->display(wc_number);
                return;
            }
            else if(WCjudge_2[1]!=0)
            {
                //坠床
                fall=1;
                ui->plainTextEdit_2->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"警告！！！！！！！！已坠床");
                ui->label_15->setPixmap(*pixmap7);
                return;
            }
        }
    }
}

void Widget::GetData()
{
    if(httpNumber>=20)
    {
        for(int j=0; j<8; j++)
        {
            c_array[j]->axisX->setMin(httpNumber-20);
            c_array[j]->axisX->setMax((httpNumber));
            c_arrayB[j]->axisX->setMin(httpNumber-20);
            c_arrayB[j]->axisX->setMax((httpNumber));
        }
    }
    http->myGet();
    http_B->myGet();
    if(httpNumber==2 || flag!=0)
    {
      httpTiao();
    }

    if(httpNumber>=2)
    {
    for(int i=0;i<8;i++)
    {
      SPD_vector[i] = (http->current_value[i]- preVA[i])/1000;
      SPD_vectorB[i] = (http_B->current_value[i]- preVB[i])/1000;
      c_series[i]->append(httpNumber,SPD_vector[i]);
      c_serieB[i]->append(httpNumber,SPD_vectorB[i]);
    }
    }
}

void Widget::httpTiao()
{
    for (int i=0;i<8;i++)
    {
        preVA[i] = http->current_value[i];
        preVB[i] = http_B->current_value[i];
    }
    flag = 0;
}

void Widget::zhen_ShiBieB()
{

    QString s =  QString::number(PostureB);
    ui->plainTextEdit_2->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"睡姿改变为"+s);
    ui->lcdNumber_2->display(change_number);
    switch(PostureB)
    {
    case 0:
        ui->label_15->setPixmap(*pixmap8);
        break;
    case 1:
        ui->label_15->setPixmap(*pixmap1);
        break;
    case 2:
        ui->label_15->setPixmap(*pixmap2);
        break;
    case 3:
        ui->label_15->setPixmap(*pixmap3);
        break;
    case 4:
        ui->label_15->setPixmap(*pixmap4);
        break;
    case 5:
        ui->label_15->setPixmap(*pixmap5);
        break;
    case 6:
        ui->label_15->setPixmap(*pixmap6);
        break;
    case 7:
        break;
    }
}

void Widget::zhen_ShiBie()
{

    QString s =  QString::number(Posture);
    ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+":"+"睡姿改变为"+s);
    ui->lcdNumber_2->display(change_number);
    switch(Posture)
    {
    case 0:
        ui->label->setPixmap(*pixmap8);
        break;
    case 1:
        ui->label->setPixmap(*pixmap1);
        break;
    case 2:
        ui->label->setPixmap(*pixmap2);
        break;
    case 3:
        ui->label->setPixmap(*pixmap3);
        break;
    case 4:
        ui->label->setPixmap(*pixmap4);
        break;
    case 5:
        ui->label->setPixmap(*pixmap5);
        break;
    case 6:
        ui->label->setPixmap(*pixmap6);
        break;
    case 7:
        break;
    }
}

Widget::~Widget()
{
    mtd->setFlag(0);
    thread->quit();
    thread->wait();
    delete mtd;
    delete ui;
}

