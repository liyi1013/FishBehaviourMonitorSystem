#ifndef PLOTDATA_H
#define PLOTDATA_H

#pragma execution_character_set("utf-8")

#include<fstream>

#include <qstring.h>
#include <QtCore\qdebug.h>
#include <QtCore\qvector.h>
#include <QtWidgets\QWidget>
#include <QtWidgets\qpushbutton.h>

#include"qcustomplot.h"

// 这个类与视频显示的类一起用，用于视频帧对应的数据的显示
// 读取一个数据文件，
// 将数据以曲线图的形式显示出来（蓝色）
// 提供红色的曲线，表示实时绘制
class plotData : public QWidget
{
	Q_OBJECT
 public:
	 plotData(QWidget *parent = 0, QTimer *timer=0);
	~plotData();
	
	bool openfile(QString filename);
	
	void timeouttodo();
	void goto_x(double positon_x);

 public slots:
	void openfile();

 private:
	int length;  //数据长度
	int i;       //当前数据位置

	std::ifstream datafile; //文件名（.txt）

	QVector<double> x;      //x坐标

	QVector<double> qspeed;
	QVector<double> qwp;
	
	QCustomPlot* ui_qcustomplot;
	QTimer *_t;
	QPen pen;
	QPushButton *button;
};

#endif // PLOTDATA_H
