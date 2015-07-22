#pragma once
#include <cv.h>
#include <math.h>
#include <iostream>
#include <vector>


class Fish
{
public:
	CvPoint center;
	CvPoint head;
	CvPoint tail;
	//int TF;			//Tail frequency,Î²Æµ£¬µ¥Î»/s

	Fish()
	{
		center.x = 0;
		center.y = 0;
		head.x = 0;
		head.y = 0;
		tail.x = 0;
		tail.y = 0;
	}

	Fish(CvSeq *cont);

	Fish(const std::vector<cv::Point> &contours);

	Fish(const Fish &fish)
	{
		center.x = fish.center.x;
		center.y = fish.center.y;
		head.x = fish.head.x;
		head.y = fish.head.y;
		tail.x = fish.tail.x;
		tail.y = fish.tail.y;
		//TF = fish.TF;
	}

	~Fish();
	//void CreatFish(CvSeq *cont);
private:

	int Distance(CvPoint a, CvPoint b)//¾àÀëµÄÆ½·½
	{
		int x = a.x > b.x ? (a.x - b.x) : (b.x - a.x);
		int y = a.y > b.y ? (a.y - b.y) : (b.y - a.y);
		return  (x * x + y * y);
	}
};

