#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
class My_Delaunay
{
public:
	My_Delaunay();
	My_Delaunay(CvSize);
	~My_Delaunay();

	void set_points(vector<CvPoint> &);
	int draw_subdiv(IplImage *, Scalar, int);
	CvPoint Get_Center();
private:

	vector<CvPoint> _Triangles_center;
	CvPoint _all_Triangles_center;
	Rect *_rect;
	Subdiv2D *_subdiv;
	CvSize _size;
};

