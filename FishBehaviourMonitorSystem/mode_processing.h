#pragma once

#include<math.h>
#include<vector>
#include<deque>

#include <cv.h>     //iplimage
#include <highgui.h>//CvCapture

#include"Fish.h"
#include"ImgProcessSet.h"

// 虚类 接口
class mode_processing
{
public:
	mode_processing(ImgProcessSet  *img_p_set);
	~mode_processing();

	/*纯虚函数--接口*/
	virtual double execute( cv::Mat *src,  cv::Mat *img_draw, int minContour) = 0;
	virtual double execute(IplImage *src, IplImage *img_draw, int minContour) = 0; // c++ 风格
	//virtual double execute(IplImage *src, IplImage *img_draw) = 0;
	ImgProcessSet  *_img_process_set;
};



// 接口实现
class Speedmode_processing :public mode_processing
{
public:
	Speedmode_processing(ImgProcessSet  *img_p_set) :mode_processing(img_p_set){};

	/*virtual*/double execute(IplImage *src, IplImage *img_draw, int minContour);
	/*virtual*/double execute(cv::Mat *src, cv::Mat *img_draw, int minContour);// c++ 风格
	/*计算速度，移动窗口法*/
private:
	CvPoint            _old_point;
	std::deque<double> _distence;     /*帧与帧之间 中心点距离*/
	double             speed = 0;
	size_t             num_frm_Sec = 15;/*每秒帧数*/
	//std::vector<CvPoint> fishCenter;

	CvPoint compute_Contour(IplImage *src, IplImage *img_draw, int minContour);
	// 计算一条鱼的重心？？？c++ 风格
	CvPoint compute_Contour(cv::Mat *src, cv::Mat *img_draw, int minContour);

	double compute_speed(CvPoint fishCenter);
};

/**/
class WPmode_processing :public mode_processing
{
public:
	WPmode_processing(ImgProcessSet  *img_p_set) :mode_processing(img_p_set){ _old_fish.center = { 0, 0 }; };
	virtual    double execute(cv::Mat *src, cv::Mat *img_draw, int minContour);
	/*virtual*/double execute(IplImage *src, IplImage *img_draw, int minContour);

	/*计算速度，移动窗口法*/
private:
	CvPoint            _old_point;
	std::deque<double> _distence;     /*帧与帧之间 中心点距离*/
	double             speed = 0;
	size_t             num_frm_Sec = 15;/*每秒帧数*/
	Fish  _this_fish;
	Fish  _old_fish;

	Fish compute_Contour(IplImage *src, IplImage *img_draw, int minContour);

	int  compute_WP(Fish fish);
};

class Clustermode_processing :public mode_processing
{
public:
	Clustermode_processing(ImgProcessSet  *img_p_set) :mode_processing(img_p_set){};
	virtual    double execute(cv::Mat *src, cv::Mat *img_draw, int minContour);
	/*virtual*/double execute(IplImage *src, IplImage *img_draw, int minContour);

private:

	double             speed = 0;
	size_t             num_frm_Sec = 15;/*每秒帧数*/

	std::vector<CvPoint> _fishCenter;//存储每条鱼中心

	void compute_Contour(IplImage *src, IplImage *img_draw, int minContour, std::vector<CvPoint>& fishCenter);

	int compute_R(std::vector<CvPoint>& fishCenter, IplImage *img_draw);

	//int compute_numFishOutCircle(std::vector<CvPoint>& fishCenter, IplImage *img_draw);
	CvPoint _Delaunay(std::vector<CvPoint> points, IplImage *dst);
};