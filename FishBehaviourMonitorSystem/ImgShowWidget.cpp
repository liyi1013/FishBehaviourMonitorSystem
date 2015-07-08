#include "ImgShowWidget.h"


ImgShowWidget::ImgShowWidget(QWidget *parent) : QWidget(parent), _size(320, 240)
{

	setupUi();
	this->setFixedSize(_size);

	// set QSS
	QFile file("Resources/qss.qss");
	file.open(QFile::ReadOnly);
	QTextStream filetext(&file);
	QString stylesheet = filetext.readAll();
	this->setStyleSheet(stylesheet);
}


ImgShowWidget::~ImgShowWidget()
{
}

void ImgShowWidget::setupUi()
{
	_img_label = new QLabel(this);
	_img_label->setFixedSize(_size);
	_img_label->setFrameShape(QFrame::Box);
	_img_label->setScaledContents(true);
}

void ImgShowWidget::show_img(const QImage *src)
{
	if (src){
		this->_img_label->setPixmap(QPixmap::fromImage(*src));
	}
}

void ImgShowWidget::set_size(const QSize& size)
{
	_size = size;
	this->setFixedSize(_size);
	this->_img_label->setFixedSize(_size);
}

//----------------------------------------------------------------


ImgShowWidget_opencv::ImgShowWidget_opencv(QWidget *parent)
	:ImgShowWidget(parent)
{
	_qimg = nullptr;
	_iplImg = nullptr;
}

ImgShowWidget_opencv::~ImgShowWidget_opencv()
{
	if (_qimg){
		delete _qimg;
	}
	if (_iplImg){
		cvReleaseImageHeader(&_iplImg);
	}
}

void ImgShowWidget_opencv::update_img(const IplImage *src)
{
	//观察vp的变化->显示图像
	/*如果没有qimg*/
	if ((!this->_qimg) || (!this->_iplImg)){
		_qimg = new QImage(QSize(src->width, src->height), QImage::Format_RGB888);
		_iplImg = cvCreateImageHeader(cvSize(src->width, src->height), 8, 3);
		_iplImg->imageData = (char*)_qimg->bits();
	}
	if (src)
	{
		cvCvtColor(src, _iplImg, CV_BGR2RGB);
		//cvCopy(src, _iplImg);
		if (_qimg){
			this->show_img(_qimg);
		}
	}
}


//----------------------------------------------------------------


ImgShowWidget_Mat::ImgShowWidget_Mat(QWidget *parent)
	:ImgShowWidget(parent)
{
	_qimg = nullptr;
	_iplImg = nullptr;
}

ImgShowWidget_Mat::~ImgShowWidget_Mat()
{
	if (_qimg){
		delete _qimg;
	}
	if (_iplImg){
		cvReleaseImageHeader(&_iplImg);
	}
}

void ImgShowWidget_Mat::update_img(cv::Mat &mat)
{
	QImage &&qimg = Mat2QImage(mat);
	this->show_img(&qimg);
}
