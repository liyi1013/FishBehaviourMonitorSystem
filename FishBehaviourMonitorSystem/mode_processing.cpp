#include "mode_processing.h"
#include "My_Delaunay.h"

int Distance(CvPoint a, CvPoint b)
{
	int x = a.x > b.x ? (a.x - b.x) : (b.x - a.x);
	int y = a.y > b.y ? (a.y - b.y) : (b.y - a.y);
	return  (x * x + y * y);
}

double cvCos(CvPoint a, CvPoint b)
{
	CvPoint p = { 0, 0 };
	return (Distance(a, p) + Distance(b, p) - Distance(a, b)) /
		(2 * sqrt(double(Distance(a, p)*Distance(b, p))));
}

mode_processing::mode_processing(ImgProcessSet  *img_p_set) :_img_process_set(img_p_set)
{
}


mode_processing::~mode_processing()
{
}


double Speedmode_processing::execute(IplImage *src, IplImage *img_draw, int minContour){

	CvPoint center = this->compute_Contour(src, img_draw, _img_process_set->get_min_area());

	return this->compute_speed(center);
}

CvPoint Speedmode_processing::compute_Contour(IplImage *src, IplImage *img_draw, int minContour){
	CvSeq        *_cont;      //用于存储轮廓的结构
	CvMemStorage *_stor;      //
	_stor = cvCreateMemStorage(0); // 分配内存空间，之后必须释放
	_cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), _stor);

	int Number_All_Object = cvFindContours(src, _stor, &_cont, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	//只提取最外层的轮廓,将所有点由链码形式翻译为点序列形式,偏移为0

	//////////////////////////////////////////////////////////////////////////////////

	// 声明
	int num_cont = 0;      // 轮廓数
	CvPoint   fish_centerpoint;
	// 计算得到每一条鱼的中心， 保持在 _fishCenter 中
	while (_cont)
	{
		double s = cvContourArea(_cont);// 计算整个轮廓的面积 
		if (fabs(s) < _img_process_set->get_min_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		else if (fabs(s) > _img_process_set->get_max_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		cvDrawContours(img_draw, _cont, CV_RGB(0, 255, 0), CV_RGB(255, 0, 0), 0);

		// 以下程序块：计算、保存每个轮廓（鱼）的重心 (输入cont、num_cont，输出 _fishCenter)
		{
			CvMoments moments;   //??
			cvMoments(_cont, &moments, 0);
			// 计算重心坐标  这里的重心指的是一条鱼的重心（轮廓的重心）
			float cx, cy;         // 定义重心坐标
			cx = cvRound(moments.m10 / moments.m00);
			cy = cvRound(moments.m01 / moments.m00);  //不变矩。。质心坐标？
			// 保存各条鱼的重心坐标 到 xy 中	

			fish_centerpoint.x = int(cx);
			fish_centerpoint.y = int(cy);
			//fishCenter.push_back(fish_centerpoint);
		}

		// 指向下一个轮廓
		_cont = _cont->h_next;
		++num_cont;
	}
	cvReleaseMemStorage(&_stor);// 内存泄漏是这里的问题。之前分配空间在Init（）中，没有cvReleaseMemStorage(&_stor);，内存泄漏	
	return fish_centerpoint;
}

double Speedmode_processing::compute_speed(CvPoint fishCenter){
	if (_old_point.x == 0){
		_old_point = fishCenter;
	}
	{

		if (_distence.size() > num_frm_Sec){
			double d2 = std::sqrt(abs((_old_point.x - fishCenter.x) ^ 2 + (_old_point.y - fishCenter.y) ^ 2));
			double d1 = _distence.front();
			_distence.pop_front();
			_distence.push_back(d2);
			speed -= d1;
			speed += d2;
		}

		else{
			double d2 = std::sqrt(abs((_old_point.x - fishCenter.x) ^ 2 + (_old_point.y - fishCenter.y) ^ 2));
			_distence.push_back(d2);
			speed += d2;
		}
		_old_point = fishCenter;
	}
	return speed;//返回的是当前帧前一秒的位移（单位：像素/s）
}

/*******************************************************************************************/

Fish WPmode_processing::compute_Contour(IplImage *src, IplImage *img_draw, int minContour)
{
	CvSeq        *_cont;      //用于存储轮廓的结构
	CvMemStorage *_stor;      //
	_stor = cvCreateMemStorage(0); // 分配内存空间，之后必须释放
	_cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), _stor);

	int Number_All_Object = cvFindContours(src, _stor, &_cont, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

	int num_cont = 0;      // 轮廓数
	Fish fish;
	CvPoint   fish_centerpoint;
	while (_cont)
	{
		double s = cvContourArea(_cont);// 计算整个轮廓的面积 
		if (fabs(s) < _img_process_set->get_min_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		else if (fabs(s) > _img_process_set->get_max_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		fish = Fish(_cont);
		_cont = _cont->h_next;
		++num_cont;
	}
	cvReleaseMemStorage(&_stor);// 内存泄漏是这里的问题。之前分配空间在Init（）中，没有cvReleaseMemStorage(&_stor);，内存泄漏	
	return fish;
}

int  WPmode_processing::compute_WP(Fish fish)
{
	if (_old_fish.center.x == 0 && _old_fish.center.y == 0)
	{
		_old_fish = fish;
		return 0;
	}
	else
	{
		CvPoint tmp1 = { fish.center.x - fish.head.x, fish.center.y - fish.head.y };	//头-重心
		CvPoint tmp2 = { fish.tail.x - fish.center.x, fish.tail.y - fish.center.y };	//重心-尾
		double cos = cvCos(tmp1, tmp2);

		if (cos < 0.99)
		{
			if ((fish.center.y - fish.head.y)*(fish.tail.x - fish.center.x) + (fish.head.x - fish.center.x)*(fish.tail.y - fish.center.y) > 0){

				return 1;
			}
			else{
				return 2;
			}
		}
		else{
			return 0;
		}
	}
}

double WPmode_processing::execute(IplImage *src, IplImage *img_draw, int minContour){

	Fish center = this->compute_Contour(src, img_draw, minContour);

	return this->compute_WP(center);
}
//-----------------------------------------------------------------------------------------------
double Clustermode_processing::execute(IplImage *src, IplImage *img_draw, int minContour)
{
	_fishCenter.clear();
	compute_Contour(src, img_draw, minContour, _fishCenter);
	return compute_R(_fishCenter, img_draw);
}

void Clustermode_processing::compute_Contour(IplImage *src, IplImage *img_draw, int minContour,
	std::vector<CvPoint>& fishCenter)
{
	CvSeq        *_cont;      //用于存储轮廓的结构
	CvMemStorage *_stor;      //
	_stor = cvCreateMemStorage(0); // 分配内存空间，之后必须释放
	_cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), _stor);

	int Number_All_Object = cvFindContours(src, _stor, &_cont, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

	int num_cont = 0;      // 轮廓数
	Fish fish;
	CvPoint   fish_centerpoint;
	while (_cont)
	{
		double s = cvContourArea(_cont);// 计算整个轮廓的面积 
		if (fabs(s) < _img_process_set->get_min_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		else if (fabs(s) > _img_process_set->get_max_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		fish = Fish(_cont);
		fishCenter.push_back(fish.center);
		_cont = _cont->h_next;
		++num_cont;
	}
	cvReleaseMemStorage(&_stor);// 内存泄漏是这里的问题。之前分配空间在Init（）中，没有cvReleaseMemStorage(&_stor);，内存泄漏	
}

int Clustermode_processing::compute_R(std::vector<CvPoint>& fishCenter, IplImage *img_draw)
{
	int _r = 0;
	int num_cont = fishCenter.size();
	CvPoint _all_triangles_center = _Delaunay(fishCenter, img_draw);

	// 计算鱼中心到 Triangles_center 的距离， 并排序，得到第二远的距离作为半径 R。
	std::vector<int> fishDiff;

	//计算每条鱼与鱼群中心坐标的位置，放到fishDiff[i]中.
	for (int i = 0; i < num_cont; ++i)
	{
		fishDiff.push_back(sqrt((_all_triangles_center.x - _fishCenter[i].x)*(_all_triangles_center.x - _fishCenter[i].x)
			+ (_all_triangles_center.y - _fishCenter[i].y)*(_all_triangles_center.y - _fishCenter[i].y)));
	}

	sort(fishDiff.begin(), fishDiff.end());
	if (num_cont >= 2){
		_r = fishDiff[num_cont - 2]; //取第二远作为半径； 对的
	}
	else{
		_r = 30;
	}

	// 绘中心点
	cvCircle(img_draw, _all_triangles_center, 2, cvScalar(0, 255, 255, 0), 2, 8, 0);
	// 绘圆圈
	cvCircle(img_draw, _all_triangles_center, _r, cvScalar(0, 255, 255, 0), 1, 8, 0);
	//_all_triangles_center;
	return _r;
}

CvPoint Clustermode_processing::_Delaunay(std::vector<CvPoint> points, IplImage *dst){
	if (points.size() < 1){
		return{ 0, 0 };
	}
	if (points.size() == 1){
		return points[0];
	}
	if (points.size() == 2){

		cvLine(dst, points[0], points[1], cvScalar(0, 0, 0, 0), 1, 8, 0);
		return{ (points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2 };
	}
	if (points.size() == 3){

		cvLine(dst, points[0], points[1], cvScalar(0, 0, 0, 0), 1, 8, 0);
		cvLine(dst, points[1], points[2], cvScalar(0, 0, 0, 0), 1, 8, 0);
		cvLine(dst, points[0], points[2], cvScalar(0, 0, 0, 0), 1, 8, 0);
		return{ (points[0].x + points[1].x + points[2].x) / 3, (points[0].y + points[1].y + points[2].y) / 3 };
	}
	//points.size() > 3

	My_Delaunay D({ dst->width, dst->height });
	D.set_points(points);
	D.draw_subdiv(dst, { 0, 255, 0 }, 1);

	return D.Get_Center();
}
