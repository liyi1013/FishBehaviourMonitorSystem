#pragma once
#include<QtWidgets\qwidget.h>
#include<QtWidgets\qlabel.h>

#include<QtCore\qfile.h>
#include<QtCore\qtextstream.h>

#include <cv.h>     //iplimage
#include <highgui.h>//CvCapture


//  基本类
//  这个类用于显示一个图片QImage *src 到 QLabel   *_img_label 上
//  可以作为其他控件的子控件。
//  void show_img(const QImage *src); 显示图片
// *void set_size(const QSize size);  改变大小
class ImgShowWidget : public QWidget
{
public:
	ImgShowWidget(QWidget *parent);
	~ImgShowWidget();
	void show_img(const QImage *src);//显示图片
	void set_size(const QSize& size); //改变大小

protected:
	QLabel* _img_label;
	QSize   _size;
	void setupUi();
};

//  继承类img_show_widget
//  这个类用于显示一个图片QImage *src 到 QLabel   *_img_label 上
//  可以作为其他控件的子控件。
//  void show_img(const QImage *src); 显示图片
//  void set_size(const QSize size);  改变大小
// *void updata_img(IplImage *src);   显示opencv的iplimage图像
class ImgShowWidget_opencv :public ImgShowWidget
{
public:
	ImgShowWidget_opencv(QWidget *parent);
	~ImgShowWidget_opencv();

	void updata_img(const IplImage *src);
private:
	IplImage *_iplImg;
	QImage   *_qimg;
};