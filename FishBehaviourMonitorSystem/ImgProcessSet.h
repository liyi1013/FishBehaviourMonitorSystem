#pragma execution_character_set("utf-8")//解决中文乱码
#pragma once
#include "qwidget.h"
#include <QSettings>
#include<QSpinBox>
#include<qlayout.h>
#include<qlabel.h>

class ImgProcessSet :public QObject
{
	Q_OBJECT
public:

	ImgProcessSet();
	~ImgProcessSet();
	void save_all_data();

	int get_num_fish(){ return _num_fish; }
	int get_segment_threshold(){ return _segment_threshold; }
	int get_min_area(){ return _min_area; }
	int get_max_area(){ return _max_area; }

public slots:
	int set_num_fish(int n){ _num_fish = n; return _num_fish; }
	int set_segment_threshold(int t){ _segment_threshold = t;  return _segment_threshold; }
	int set_min_area(int min){ _min_area = min;  return _min_area; }
	int set_max_area(int max){ _max_area = max;  return _max_area; }

private:
	int _num_fish;
	int _segment_threshold;
	int _min_area;
	int _max_area;
};

class ImgProcessSet_view : public QWidget
{
	Q_OBJECT
public:
	ImgProcessSet_view(ImgProcessSet* ImgP_set);
	~ImgProcessSet_view();

	void set_up_UI();
	void set_up_Data();
	
	QSpinBox* ui_num_fish;
	QSpinBox* ui_segment_threshold;
	QSpinBox* ui_min_area;
	QSpinBox* ui_max_area;
private:
	ImgProcessSet* _img_process_set;
};

