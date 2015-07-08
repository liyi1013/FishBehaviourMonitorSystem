#pragma once
#include <vector>
#include <QtWidgets\qwidget.h>
#include "MyVideoPlayer.h"
#include "plotdata.h"

// 继承MyVideoPlayer,
// 重定义CallBackTrackBarSlide(int i)函数,等函数
// 与类plotData关联起来
class MyVideoPlayer_1 :public MyVideoPlayer
{
public:
	~MyVideoPlayer_1();
	MyVideoPlayer_1(QWidget* parent, QTimer* timer) : MyVideoPlayer(parent, timer)
	{
		connect(_timer, &QTimer::timeout, this, &MyVideoPlayer_1::nextFrame);
	}
private:

	std::vector<plotData*> dataCruves;

public:
	void AddPlotDataWidget(plotData* plotdata)
	{
		dataCruves.push_back(plotdata);
	}

	void CallBackTrackBarSlide(int i)
	{
		for (size_t j = 0; j < dataCruves.size(); ++j)
		{
			dataCruves[j]->goto_x(i/15-1);
		}

		cvSetCaptureProperty(_capture, CV_CAP_PROP_POS_FRAMES, i);
		_Position = i;

		int sec = _Position / 15;
		int min = sec / 60; sec = sec % 60;
		int h = min / 60; min = min % 60;
		if (sec < 10 && min < 10){
			this->_time_label->setText(tr("%0:0%1:0%2").arg(h).arg(min).arg(sec));
		}
		else if (sec < 10){
			this->_time_label->setText(tr("%0:%1:0%2").arg(h).arg(min).arg(sec));
		}
		else if (min < 10){
			this->_time_label->setText(tr("%0:0%1:%2").arg(h).arg(min).arg(sec));
		}
		else{
			this->_time_label->setText(tr("%0:%1:%2").arg(h).arg(min).arg(sec));
		}
		_frame = cvQueryFrame(_capture);
		cvCopy(_frame, _iplImg, 0);
		cvCvtColor(_iplImg, _iplImg, CV_BGR2RGB);
		this->videoplayer->show_img(_qImg);
	}

	void nextFrame(){
		if (_capture)
		{
			_frame = cvQueryFrame(_capture);
		}
		if (_frame){
			cvCopy(_frame, _iplImg, 0);
			cvCvtColor(_iplImg, _iplImg, CV_BGR2RGB);

			this->videoplayer->show_img(_qImg);

			_Position++;
			if (_Position % 15 == 0)
			{
				this->_grogressbar_horizontalSlider->setValue(_Position);
				for (plotData* dataCruve : dataCruves)
				{
					dataCruve->timeouttodo();
				}
			}
		}
		else { //video end
			_timer->stop();
			QMessageBox::information(this, tr("提示"), tr("视频结束！"));
			//this->close();//关闭当前窗口
		}
	}

	bool Open_File(QString filename){
		_file_name = filename;
		char*  ch;
		QByteArray ba = _file_name.toLatin1();
		ch = ba.data();

		this->setWindowTitle(_file_name);

		_capture = cvCaptureFromFile(ch);

		if (_capture){

			_totalFrames = (int)cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_COUNT);
			//qDebug() << _totalFrames;
			this->_grogressbar_horizontalSlider->setRange(0, _totalFrames);

			int sec = _totalFrames / 15;
			int min = sec / 60; sec = sec % 60;
			int h = min / 60; min = min % 60;
			if (sec < 10 && min < 10){
				this->_time_label_2->setText(tr("/ %0:0%1:0%2").arg(h).arg(min).arg(sec));
			}
			else if (sec < 10){
				this->_time_label_2->setText(tr("/ %0:%1:0%2").arg(h).arg(min).arg(sec));
			}
			else if (min < 10){
				this->_time_label_2->setText(tr("/ %0:0%1:%2").arg(h).arg(min).arg(sec));
			}
			else{
				this->_time_label_2->setText(tr("/ %0:%1:%2").arg(h).arg(min).arg(sec));
			}

			_frame = cvQueryFrame(_capture);

			_qImg = new QImage(QSize(_frame->width, _frame->height), QImage::Format_RGB888);
			_iplImg = cvCreateImageHeader(cvSize(_frame->width, _frame->height), 8, 3);
			_iplImg->imageData = (char*)_qImg->bits();

			connect(this->_grogressbar_horizontalSlider, &QSlider::valueChanged, this, &MyVideoPlayer_1::CallBackTrackBarSlide);
			
			_timer->start();
			_play_flag = true;

			cvCopy(_frame, _iplImg, 0);
			cvCvtColor(_iplImg, _iplImg, CV_BGR2RGB);
			this->videoplayer->show_img(_qImg);

			return true;
		}
		else{
			QMessageBox::information(this, tr("提示"), tr("无法打开%0！").arg(filename));
			return false;
		}
	}
};

//*显示记录的视频和数据的控件
class VideoDataDisplayer : public QWidget
{
public:
	VideoDataDisplayer(int num_fish);
	~VideoDataDisplayer();

	bool openFile(QString filename);
	void setTitle(QString filename);

private:
	MyVideoPlayer_1 *ui_videoplayer;
	plotData* ui_plotdata_1;
	plotData* ui_plotdata_2;
	QLabel* ui_currentData;
	QLabel *ui_title;
	int num_fish;
	void setupUI(int num_fish)
	{   
		// [0]
		QVBoxLayout *vLayout_main = new QVBoxLayout(this);
		// [1]
		ui_title = new QLabel(this);
		ui_title->setText(tr("监测编号：01201506061059； 视频编号：201506061100； 起始时间2015-06-06 9：00，结束时间2015-06-06 11：00"));

		// [2]
		QHBoxLayout *hLayout_main = new QHBoxLayout();

		// [2][1]
		QVBoxLayout *vLayout_data_Widgets = new QVBoxLayout();

		// [2][1][1]
		QTabWidget *tabWidget = new QTabWidget(this);

		if (num_fish == 1)
		{
			ui_plotdata_1 = new plotData(this, _timer);
			ui_plotdata_2 = new plotData(this, _timer);
			tabWidget->addTab(ui_plotdata_1, tr("速度"));
			tabWidget->addTab(ui_plotdata_2, tr("尾频"));
		}
		else if (num_fish > 1)
		{
			ui_plotdata_1 = new plotData(this, _timer);
			ui_plotdata_2 = new plotData(this, _timer);
			tabWidget->addTab(ui_plotdata_1, tr("R"));
			tabWidget->addTab(ui_plotdata_2, tr("out of cricle"));
		}

		tabWidget->setFixedHeight(280);

		// [2][1][2]
		QHBoxLayout *hLayout_3 = new QHBoxLayout(this);
		QComboBox* _datastep_comboBox = new QComboBox(this);
		_datastep_comboBox->addItem("1秒");
		_datastep_comboBox->addItem("1分钟");
		_datastep_comboBox->addItem("30分钟");

		hLayout_3->addWidget(new QLabel("间隔单位:"));
		hLayout_3->addWidget(_datastep_comboBox);
		hLayout_3->addSpacing(340);

		// [2][1][add]
		vLayout_data_Widgets->addWidget(tabWidget);
		vLayout_data_Widgets->addLayout(hLayout_3);

		// [2][2]
		QVBoxLayout *vLayout_video_Widgets = new QVBoxLayout();
		ui_videoplayer = new MyVideoPlayer_1(this, _timer);
		ui_videoplayer->AddPlotDataWidget(ui_plotdata_1);
		ui_videoplayer->AddPlotDataWidget(ui_plotdata_2);

		vLayout_video_Widgets->addSpacing(20);
		vLayout_video_Widgets->addWidget(ui_videoplayer);

		// [2][add]
		hLayout_main->addLayout(vLayout_data_Widgets);
		hLayout_main->addLayout(vLayout_video_Widgets);

		// [0][add]
		vLayout_main->addWidget(ui_title);
		vLayout_main->addLayout(hLayout_main);
		//this->setFixedSize(880, 380);
		this->setWindowTitle("视频数据查看");
	}

	QString _file_name;
	QTimer *_timer;
};

