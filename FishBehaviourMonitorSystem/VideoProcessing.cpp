#include "VideoProcessing.h"
#include "mainwindow.h"
#include"SystemSet.h"

VideoProcessing::VideoProcessing(QObject *parent, SystemSet *set, SysDB* sys_db, ImgProcessSet  *img_p_set)
	:QObject(parent), _fps(15), _codec(CV_FOURCC('D', 'I', 'V', 'X')), _sys_set(set), _sys_db(sys_db), _img_process_set(img_p_set)
{
	_mode_processing = new Speedmode_processing(img_p_set);
	_mode_processing_wp = new WPmode_processing(img_p_set);
	_mode_processing_Cluster = new Clustermode_processing(img_p_set);
}
void VideoProcessing::attach(MainWindow *Object)
{
	_main_window = Object;
}

VideoProcessing::~VideoProcessing()
{
	if (_video_Writer){
		cvReleaseVideoWriter(&_video_Writer);
	}

	_data_writer_1.close();
	_data_writer_2.close();
	_data_writer_3.close();
}

IplImage* VideoProcessing::ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw)
{
	// 图片预处理，转化成灰度图像
	cvCvtColor(src, dst, CV_BGR2GRAY);  // 彩色图像转化成灰度图像
	cvSmooth(dst, dst, CV_GAUSSIAN, 3, 0, 0); //高斯滤波

	// 图片 阈值分割
	cvThreshold(dst, dst, _img_process_set->get_segment_threshold(), 255, CV_THRESH_BINARY); //取阀值把图像转为二值图像

	// 开运算
	cvMorphologyEx(dst, dst, NULL, 0, CV_MOP_OPEN, 1);
	// 闭运算（先膨胀再腐蚀）:效果较好，可以使目标更加完整清晰，但仍就有一些噪声。
	cvMorphologyEx(dst, dst, NULL, 0, CV_MOP_CLOSE, 1);

	// 腐蚀
	cvErode(dst, dst, 0, 1);
	// 膨胀
	cvDilate(dst, dst, 0, 1);
	//颜色反转
	cvNot(dst, dst);

	return dst;
}

bool VideoProcessing::open_camera()
{
	_capture = cvCaptureFromCAM(0);
	if (!_capture){
		//qDebug() << "Can not open the camera.";
		return false;
	} else {
		_frame = cvQueryFrame(_capture);//从CvCapture中获得一帧
		if (!_frame){
			//qDebug() << "Can not get frame from the capture.\n";
			return false;
		}
		CvSize img_size = cvSize(_frame->width, _frame->height);
		//中间存储图像
		_p_temp = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
		return true;
	}
}

bool VideoProcessing::open_file(const char* file_name)
{
	//_video_file_name = file_name;
	_capture = cvCaptureFromFile(file_name);
	if (!_capture){
		//std::cout << "Can not open the file:"<<file_name<<endl;
		return false;
	} else {
		_frame = cvQueryFrame(_capture);
		if (!_frame){
			//std::cout << "Can not get frame from the capture." << std::endl;
			return false;
		}
		CvSize img_size = cvSize(_frame->width, _frame->height);
		_p_temp = cvCreateImage(img_size, IPL_DEPTH_8U, 1);  //轮廓效果前景帧
		ImgProcessing(_frame, _p_temp, _frame);
		this->notify();
		return true;
	}
}

void VideoProcessing::time_out_todo_1()
{
	//从CvCapture中获得一帧
	_frame = cvQueryFrame(_capture);
	if (!_frame){
		//qDebug() << "Can not get frame from the capture.\n";
	} else {
		// 如果开始处理
		if (_isPrecess){
			ImgProcessing(_frame, _p_temp, _frame);
			if (_img_process_set->get_num_fish() == 1){
				double speed = _mode_processing->execute(_p_temp, _frame, _img_process_set->get_min_area());
				double wp = _mode_processing_wp->execute(_p_temp, _frame, _img_process_set->get_min_area());
				send_data(1, speed / (640 / this->_sys_set->get_realLength()));
				send_data(2, wp);
			}
			else if (_img_process_set->get_num_fish() > 1)
			{
				double r = _mode_processing_Cluster->execute(_p_temp, _frame, _img_process_set->get_min_area());
				send_data(3, r);
			}

			// 如果开始记录
			if (_isPrecess && _isRecord)
			{
				if (_video_Writer){
					save_video();//保存视频
				}
				else
				{}
			}
		}
		++_num_of_frames;
		this->notify();
	}
}

void VideoProcessing::notify()// 图像 或 数据 改变了，向观察者mainwindow 发出通知
{
	if (_main_window && _frame)
	{
		_main_window->updata_img(_frame);
	}
}


void VideoProcessing::send_data(size_t modeIndex, double data){
	static double speed = 0;
	static int duration = 4;//异常持续时间
	static int wp_count = 0;
	static int old_data = 0;  //前一个数据

	static int r = 0;
	static int r_count = 0;
	static int is_warning = 0;

	static int speedcount = 0;
	static int wpcount = 0;

	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	switch (modeIndex)
	{
	case 1: // 速度
	{
		if (_num_of_frames % 15 == 0){
			// 数据显示，保存
			_main_window->updata_data(1, data);

			if (_isRecord){
				_data_writer_1 << (data) << " ";	// 数据写入对应文件中

				// 预警检测
				if (data > 2){
					++speedcount;
					if (speedcount > duration && is_warning == 0){
						_main_window->ui_warning_view->add_warning_item(1, 1, "速度异常 " + current_date);
						_sys_db->Insert_warning(1, 0, modeIndex);
						is_warning = 1;
					}
				}
				else {
					speedcount = 0;
					is_warning = 0;
				}
			}
		}
		break;
	}
	case 2: //
	{
		if (_num_of_frames % 15 == 0){
			_main_window->updata_data(2/*modeIndex*/, wp_count);

			if (_isRecord){
				_data_writer_2 << (wp_count) << std::endl;	// 数据写入对应文件中

				if (data > 1.5){
					++wpcount;
					if (wpcount > duration && is_warning == 0){
						_main_window->ui_warning_view->add_warning_item(1, 2, "尾频异常 " + current_date);
						_sys_db->Insert_warning(2, 0, modeIndex);
						is_warning = 1;
					}
				}
				else{
					wpcount = 0;
					is_warning = 0;
				}
			}
			wp_count = 0;
			old_data = data;
		}
		else{
			if ((old_data == 1 && data == 2) || (old_data == 2 && data == 1)){
				++wp_count;
				old_data = data;
			}
		}
		break;
	}
	case 3: //群聚特征，
	{
		if (_num_of_frames % 15 == 0){

			_main_window->updata_data(3, r / 15);
			if (_isRecord){
				_data_writer_3 << (r / 15) << std::endl;	// 数据写入对应文件中

				if (data > 50){
					++r_count;
					if (r_count > duration && is_warning == 0){
						_main_window->ui_warning_view->add_warning_item(1, 3, "半径异常 " + current_date);
						_sys_db->Insert_warning(3, 0, modeIndex);
						is_warning = 1;
					}
				}
				else{
					r_count = 0;
					is_warning = 0;
				}
			}
			r = 0;
		}
		else {
			r += data;
		}
		break;
	}
	default:
		break;
	}
}

bool VideoProcessing::save_video(){
	if (!_video_Writer /*|| _video_save_name.empty()*/){
		puts("set video_save fisrt.\n");
		return false;
	}
	else{
		if (!_frame)
		{
			puts("Can not get frame from the capture.");
			return false;
		}
		if (!cvWriteFrame(_video_Writer, _frame)){

			puts("save video fail\n");
			return false;
		}
		return true;
	}
}

void VideoProcessing::process_end()
{
	if (this->_capture){
		_isPrecess = 0;
		_isRecord = 0;
	}

	_data_writer_1.close();
	_data_writer_2.close();
	_data_writer_3.close();


	if (_video_Writer){
		cvReleaseVideoWriter(&_video_Writer);
	}
	_video_Writer = nullptr;

	_sys_db->InsertNewRecord_endtime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"), _video_id);
}


void VideoProcessing::process_begin()
{
	if (this->_capture)
	{
		_isPrecess = 1;
	}
}

bool VideoProcessing::record(){
	if (_isPrecess){

		_isRecord = 1;
		QString current_date = QDateTime::currentDateTime().toString("yyyyMMddhhmm");

		_video_id = current_date;

		QString new_video_name = _sys_set->get_file_save_path() + '/' + current_date + ".avi";
		QString new_datafile_name = _sys_set->get_file_save_path() + '/' + current_date;

		if (_sys_db->InsertNewRecord(
			_video_id,
			_sys_set->get_file_save_path(),
			_img_process_set->get_num_fish(),
			QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"),
			""/*remark*/))
		{
			_num_of_frames = 1;
			_video_Writer = cvCreateVideoWriter(new_video_name.toStdString().c_str(), _codec, _fps, { _frame->width, _frame->height }, 1);

			if (_img_process_set->get_num_fish() == 1){
				_data_writer_1.open((new_datafile_name + "_1.txt").toStdString());
				_data_writer_2.open((new_datafile_name + "_2.txt").toStdString());
			}
			else if (_img_process_set->get_num_fish() > 1)
			{
				_data_writer_3.open((new_datafile_name + "_3.txt").toStdString());
			}
			return true;
		}
	}
	return false;
}