#include "My_Delaunay.h"


My_Delaunay::My_Delaunay()
{
	_rect = new Rect(0, 0, 600, 600);
	_subdiv = new Subdiv2D(*_rect);
	_size = { 600, 600 };
	_all_Triangles_center = { 0.0 };
}
My_Delaunay::My_Delaunay(CvSize imgSize) :_size(imgSize)
{
	_rect = new Rect(0, 0, _size.width, _size.height);
	_subdiv = new Subdiv2D(*_rect);
	_all_Triangles_center = { 0.0 };
}

My_Delaunay::~My_Delaunay()
{
	delete _subdiv;
	delete _rect;
}

void My_Delaunay::set_points(vector<CvPoint> &point_set){

	// get points from point_set
	for each (CvPoint var in point_set)
	{
		_subdiv->insert(var);
	}

	// get Triangle List
	vector<Vec6f> triangleList;
	_subdiv->getTriangleList(triangleList);

	vector<CvPoint> pt(3);

	// put Triangle center into _Triangles_center(vector<CvPoint>);
	for (size_t i = 0; i < triangleList.size(); i++)
	{

		Vec6f t = triangleList[i];
		CvPoint p1 = { int(t[0]), int(t[1]) }; pt.push_back(p1);
		CvPoint p2 = { int(t[2]), int(t[3]) }; pt.push_back(p2);
		CvPoint p3 = { int(t[4]), int(t[5]) }; pt.push_back(p3);
		// determines wherther the point in image
		bool draw = true;
		for (int i = 0; i<3; i++){
			if (pt[i].x>_size.width || pt[i].y > _size.height || pt[i].x <= 0 || pt[i].y <= 0)
				draw = false;
		}
		if (draw)
		{
			CvPoint c = { (pt[0].x + pt[1].x + pt[2].x) / 3, (pt[0].y + pt[1].y + pt[2].y) / 3 };
			_Triangles_center.push_back(c);
		}
		pt.clear();
	}
}

void My_Delaunay::set_points(vector<cv::Point> &point_set){
	// get points from point_set
	for each (CvPoint var in point_set)
	{
		_subdiv->insert(var);
	}

	// get Triangle List
	vector<Vec6f> triangleList;
	_subdiv->getTriangleList(triangleList);

	vector<CvPoint> pt(3);

	// put Triangle center into _Triangles_center(vector<CvPoint>);
	for (size_t i = 0; i < triangleList.size(); i++)
	{

		Vec6f t = triangleList[i];
		CvPoint p1 = { int(t[0]), int(t[1]) }; pt.push_back(p1);
		CvPoint p2 = { int(t[2]), int(t[3]) }; pt.push_back(p2);
		CvPoint p3 = { int(t[4]), int(t[5]) }; pt.push_back(p3);
		// determines wherther the point in image
		bool draw = true;
		for (int i = 0; i<3; i++){
			if (pt[i].x>_size.width || pt[i].y > _size.height || pt[i].x <= 0 || pt[i].y <= 0)
				draw = false;
		}
		if (draw)
		{
			CvPoint c = { (pt[0].x + pt[1].x + pt[2].x) / 3, (pt[0].y + pt[1].y + pt[2].y) / 3 };
			_Triangles_center.push_back(c);
		}
		pt.clear();
	}
}

int My_Delaunay::draw_subdiv(IplImage *img, Scalar delaunay_color, int line_thickness = 1){
	if (_Triangles_center.empty()){
		std::cout << "no input! \"set_points()\" first." << std::endl;
		return 0;
	}
	else{
		vector<Vec6f> triangleList;
		_subdiv->getTriangleList(triangleList);
		vector<CvPoint> pt(3);
		for (size_t i = 0; i < triangleList.size(); i++)
		{
			Vec6f t = triangleList[i];
			CvPoint p1 = { int(t[0]), int(t[1]) }; pt.push_back(p1);
			CvPoint p2 = { int(t[2]), int(t[3]) }; pt.push_back(p2);
			CvPoint p3 = { int(t[4]), int(t[5]) }; pt.push_back(p3);

			// determines wherther the point in image
			bool draw = true;
			for (int i = 0; i<3; i++){
				if (pt[i].x>_size.width || pt[i].y > _size.height || pt[i].x <= 0 || pt[i].y <= 0)
					draw = false;
			}
			if (draw)
			{
				cvLine(img, pt[0], pt[1], delaunay_color, line_thickness, 8, 0);
				cvLine(img, pt[1], pt[2], delaunay_color, line_thickness, 8, 0);
				cvLine(img, pt[2], pt[0], delaunay_color, line_thickness, 8, 0);
			}
			pt.clear();
		}
		return 1;
	}
}

int My_Delaunay::draw_subdiv(cv::Mat &img, Scalar delaunay_color, int line_thickness = 1){
	if (_Triangles_center.empty()){
		std::cout << "no input! \"set_points()\" first." << std::endl;
		return 0;
	}
	else{
		vector<Vec6f> triangleList;
		_subdiv->getTriangleList(triangleList);
		vector<CvPoint> pt(3);
		for (size_t i = 0; i < triangleList.size(); i++)
		{
			Vec6f t = triangleList[i];
			CvPoint p1 = { int(t[0]), int(t[1]) }; pt.push_back(p1);
			CvPoint p2 = { int(t[2]), int(t[3]) }; pt.push_back(p2);
			CvPoint p3 = { int(t[4]), int(t[5]) }; pt.push_back(p3);

			// determines wherther the point in image
			bool draw = true;
			for (int i = 0; i<3; i++){
				if (pt[i].x>_size.width || pt[i].y > _size.height || pt[i].x <= 0 || pt[i].y <= 0)
					draw = false;
			}
			if (draw)
			{
				line(img, pt[0], pt[1], delaunay_color, line_thickness, 8, 0);
				line(img, pt[1], pt[2], delaunay_color, line_thickness, 8, 0);
				line(img, pt[2], pt[0], delaunay_color, line_thickness, 8, 0);
			}
			pt.clear();
		}
		return 1;
	}
}
CvPoint My_Delaunay::Get_Center(){
	if (_Triangles_center.empty()){
		std::cout << "no input! \"set_points()\" first." << std::endl;
		return{ 0, 0 };
	}
	else{
		int num_triangles = 0;
		for each (CvPoint var in _Triangles_center){
			_all_Triangles_center.x += var.x;
			_all_Triangles_center.y += var.y;
			++num_triangles;
		}
		_all_Triangles_center.x /= num_triangles;
		_all_Triangles_center.y /= num_triangles;

		return _all_Triangles_center;
	}
}