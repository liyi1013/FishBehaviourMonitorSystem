#include "Fish.h"

Fish::Fish(CvSeq *cont)
{
	if (!cont || cont->total <= 0)
	{
		return;
	}
	int tempH = 0;
	int tempT = 0;
	center.x = 0;
	center.y = 0;
	head.x = 0;
	head.y = 0;
	tail.x = 0;
	tail.y = 0;
	CvPoint* p = NULL;
	CvPoint* p1 = NULL;
	CvPoint* p2 = NULL;
	int Pnum = 0;

	for (int i = 0; i < cont->total; ++i)
	{
		p1 = (CvPoint*)cvGetSeqElem(cont, i);
		for (int j = 0; j < cont->total; ++j)
		{
			p2 = (CvPoint*)cvGetSeqElem(cont, j);
			if ((p2->x - p1->x) > 0 && p2->y == p1->y)//Ö»È¡ÓÒ±ßµÄµã
			{
				for (int k = p1->x; k <= p2->x; ++k)
				{
					++Pnum;
					center.x += k;
					center.y += p1->y;
				}
			}
		}
	}

	center.x /= Pnum;
	center.y /= Pnum;

	for (int i = 0; i < cont->total; ++i)
	{
		p = (CvPoint*)cvGetSeqElem(cont, i);
		int temp = Distance(center, *p);
		if (temp > tempT)
		{
			tempT = temp;
			tail.x = p->x;
			tail.y = p->y;
		}
	}

	for (int i = 0; i < cont->total; ++i)
	{
		p = (CvPoint*)cvGetSeqElem(cont, i);
		int temp = Distance(center, *p);
		if ((Distance(*p, tail) >= (Distance(center, *p) + Distance(center, tail))) && (temp > tempH))
		{
			tempH = temp;
			head.x = p->x;
			head.y = p->y;
		}
	}
}
Fish::Fish(const std::vector<cv::Point> &contour){
	if (contour.size()<=0 )
	{
		return;
	}
	int tempH = 0;
	int tempT = 0;
	center.x = 0;
	center.y = 0;
	head.x = 0;
	head.y = 0;
	tail.x = 0;
	tail.y = 0;
	CvPoint* p = NULL;
	CvPoint* p2 = NULL;
	int Pnum = 0;

	cv::Moments m = cv::moments(contour);

	center.x = m.m10 / m.m00;
	center.y = m.m01 / m.m00;

	for (int i = 0; i < contour.size(); ++i)
	{
		cv::Point p = contour[i];
		int temp = Distance(center, p);
		if (temp > tempT)
		{
			tempT = temp;
			tail.x = p.x;
			tail.y = p.y;
		}
	}

	for (int i = 0; i < contour.size(); ++i)
	{
		cv::Point p = contour[i];
		int temp = Distance(center,p);
		if ((Distance(p, tail) >= (Distance(center, p) + Distance(center, tail))) && (temp > tempH))
		{
			tempH = temp;
			head.x = p.x;
			head.y = p.y;
		}
	}
}
Fish::~Fish()
{
}