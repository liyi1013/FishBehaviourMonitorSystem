#include<qfiledialog.h>
#include "plotdata.h"


plotData::plotData(QWidget *parent,QTimer *timer)
	: QWidget(parent), _t(timer)
{
	i = 0;

	ui_qcustomplot = new QCustomPlot(this);

	QVBoxLayout * ui_layout_main = new QVBoxLayout(this);
	ui_layout_main->setMargin(0);

	ui_layout_main->addWidget(ui_qcustomplot);
	
	if (_t){}
	else{
		_t = new QTimer(this);
	}
	//connect(_t, &QTimer::timeout, this, &plotData::timeouttodo);
	//button = new QPushButton(this);
	//ui_layout_main->addWidget(button);
	//connect(button, SIGNAL(clicked()), this, SLOT(openfile()));
}

plotData::~plotData()
{

}

void plotData::timeouttodo()
{
	if (i < length)
	{
		ui_qcustomplot->graph(1)->addData(i, qspeed[i-1]);
		ui_qcustomplot->replot();
		++i;
	} else {
		//t.stop();
	}
}

void plotData::goto_x(double positon_x)
{
	if (positon_x < length)
	{
		ui_qcustomplot->graph(1)->clearData();
		i = 0;
		while (++i <= positon_x)
		{
			ui_qcustomplot->graph(1)->addData(i, qspeed[i - 1]);
		}
	}
	ui_qcustomplot->replot();
}

bool plotData::openfile(QString filename)
{
	datafile.open(filename.toStdString());
	if (datafile.is_open())
	{
		qspeed.clear();
		   qwp.clear();
		     x.clear();
		
	    double s, w;
		while (datafile >> s)
		{
			qspeed.push_back(s);
			x.push_back(++i);
		}

		int maxval = 0;
		for (qreal i : qspeed)
		{
			if (i > maxval)
			{
				maxval = i;
			}
		}

		ui_qcustomplot->addGraph();

		ui_qcustomplot->graph(0)->setData(x, qspeed);

		ui_qcustomplot->xAxis->setRange(-1, x.size());
		//ui_qcustomplot->xAxis->set
		ui_qcustomplot->yAxis->setRange(0, maxval);
		ui_qcustomplot->xAxis->setLabel(tr("时间 秒"));
		//ui_qcustomplot->yAxis->setLabel("speed");
		ui_qcustomplot->xAxis->setTickStep(60*5);
		ui_qcustomplot->xAxis->setSubTickCount(60);

		i = 1;
		length = x.size();

		{
			ui_qcustomplot->addGraph();
			pen.setColor(Qt::red);
			pen.setWidth(2);
			ui_qcustomplot->graph(1)->setPen(pen);
			//t.start(660);
		}
		ui_qcustomplot->replot();
		return true;
	} else {
		qDebug() << "open file fail!";
		return false;
	}
}

void plotData::openfile()
{
	QString path = QFileDialog::getOpenFileName(this, tr("打开视频文件"), ".", tr("(*.txt)"));
	if (path.isEmpty()){
		//QMessageBox::information(this, tr("Information"), tr("Can Not Open The File !"));
		return  ;
	}
	datafile.open(path.toStdString());
	qDebug() << path;
	if (datafile.is_open())
	{
		qspeed.clear();
		qwp.clear();
		x.clear();

		double s, w;
		while (datafile >> s && datafile >> w)
		{
			qspeed.push_back(s);
			qwp.push_back(w);
			x.push_back(++i);
		}

		ui_qcustomplot->addGraph();

		ui_qcustomplot->graph(0)->setData(x, qspeed);

		ui_qcustomplot->xAxis->setRange(-1, x.size());
		ui_qcustomplot->yAxis->setRange(0, 20);
		ui_qcustomplot->xAxis->setLabel("time");
		ui_qcustomplot->yAxis->setLabel("speed");

		i = 1;
		length = x.size();

		{
			ui_qcustomplot->addGraph();
			pen.setColor(Qt::red);
			pen.setWidth(2);
			ui_qcustomplot->graph(1)->setPen(pen);
			_t->start(66);
		}

		return ;
	}
	else {
		qDebug() << "open file fail!";
		return ;
	}
}