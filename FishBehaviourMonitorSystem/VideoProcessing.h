#pragma once

#include <string>
#include <vector>
#include <math.h>
#include <fstream>

#include <QtCore\qobject.h>
#include <QMessageBox>

#include <cv.h>     //iplimage
#include <highgui.h>//CvCapture
#include <opencv2\opencv.hpp> //cv::VideoCapture
#include <opencv2\core\core.hpp>

#include"mode_processing.h"
#include"ImgProcessSet.h"

//class QMessageBox;
class MainWindow;
class SystemSet;
class SysDB;

class VideoProcessing : public QObject
{
public:
	VideoProcessing(QObject *parent = 0, SystemSet *set = 0, SysDB* sys_db = 0,ImgProcessSet  *img_p_set=0);
	~VideoProcessing();

	void attach(MainWindow *Object);//将MainWindow 与 VideoProcessing 关联 //观察者调用

private:
	void notify();// 图像 或 数据 改变了，向观察者mainwindow 发出通知 
	void send_data(size_t modeIndex, double data);

private:
	// 图片预处理 函数
	IplImage *ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw);
	cv::Mat   ImgProcessing(cv::Mat &src, cv::Mat &dst, cv::Mat &img_draw);
	bool      save_video();

public:
	bool open_file(const char* file_name);//0
	bool open_camera();                   //0 打开摄像头，一直获取原始图像，直到开始处理
	cv::Mat background_pickup();  //提取背景
	void time_out_todo_1();   //设置定时器，时间一到就执行相应函数

public:

	void process_end();
	void process_begin();
	bool record();

	//flags
	int _isPrecess = 0;    // 是否正在处理：1是，0否
	int _isRecord = 0;     // 是否进行记录

private:
	MainWindow    *_main_window;
	SysDB         *_sys_db;
	SystemSet     *_sys_set;
	ImgProcessSet *_img_process_set;

	//imgs
	CvCapture    *_capture;   //1视频流捕捉器
	cv::VideoCapture  _cap;   //
	//IplImage     *_frame;     //2捕捉到的一帧图像 | frame = cvQueryFrame(capture);if (frame)
	cv::Mat      _frame;
	cv::Mat      _p_temp;
	IplImage     *_target_Img;//4储存处理后的图像
	//IplImage     *_p_temp;    //3中间存储图片

	//背景图片，灰度
	cv::Mat _background;

	int _num_of_frames = 0;
	unsigned int _num_of_frames_recoded = 0;

	// mode_processings
	mode_processing* _mode_processing;
	mode_processing* _mode_processing_wp;
	mode_processing* _mode_processing_Cluster;

	//std::ofstream _data_writer;    // 存储数据到本地
	std::ofstream _data_writer_1;
	std::ofstream _data_writer_2;
	std::ofstream _data_writer_3;
	// CvVideoWriter*_video_Writer; 
	//CvVideoWriter*_video_Writer=nullptr;   // 存储视频到本地
	cv::VideoWriter _video_Writer;   // 存储视频到本地
	QString   _video_name;         //   视频存储——文件名，标识视频名称，以及对应的数据名称如“001_201505200951”
	const int _fps;                //   视频存储——帧率
	const int _codec;

	QString _video_id;
};

