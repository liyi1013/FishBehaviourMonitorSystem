#include "ImgProcessSet.h"

ImgProcessSet::ImgProcessSet()
{
	QSettings settings("Resources/ImgProcessSet.ini", QSettings::IniFormat); // 当前目录的INI文件

	// 节点设置
	_segment_threshold = settings.value("ImgProcessSet/segment_threshold").toInt();
	_min_area = settings.value("ImgProcessSet/min_area").toInt();
	_max_area = settings.value("ImgProcessSet/max_area").toInt();

	this->_num_fish = 1;
}
ImgProcessSet::~ImgProcessSet(){
	save_all_data();
}
void ImgProcessSet::save_all_data(){
QSettings settings("Resources/ImgProcessSet.ini", QSettings::IniFormat); // 当前目录的INI文件
	settings.beginGroup("ImgProcessSet");
	settings.setValue("segment_threshold", _segment_threshold);
	settings.setValue("min_area", _min_area);
	settings.setValue("max_area", _max_area);
	settings.endGroup();
}


//`----------------------------------------------------------------------------------------

ImgProcessSet_view::ImgProcessSet_view(ImgProcessSet* ImgP_set) : _img_process_set(ImgP_set)
{
	set_up_UI();
	set_up_Data();
	connect(ui_num_fish, SIGNAL(valueChanged(int)), _img_process_set, SLOT(set_num_fish(int)));
	connect(ui_segment_threshold, SIGNAL(valueChanged(int)), _img_process_set, SLOT(set_segment_threshold(int)));
	connect(ui_min_area, SIGNAL(valueChanged(int)), _img_process_set, SLOT(set_min_area(int)));
	connect(ui_max_area, SIGNAL(valueChanged(int)), _img_process_set, SLOT(set_max_area(int)));
	this->setMaximumSize(300, 200);
}


ImgProcessSet_view::~ImgProcessSet_view()
{
}

void ImgProcessSet_view::set_up_UI()
{
	ui_num_fish = new QSpinBox(this);
	QLabel* ui_num_fish_label = new QLabel(this);
	ui_num_fish_label->setText(tr("鱼条数"));


	ui_segment_threshold = new QSpinBox(this);
	QLabel* ui_segment_threshold_label = new QLabel(this);
	ui_segment_threshold_label->setText(tr("分割阈值"));
	ui_segment_threshold->setRange(1, 255);       //设置变化范围  


	ui_min_area = new QSpinBox(this);
	ui_min_area->setRange(0, 1000);       //设置变化范围  
	ui_min_area->setSuffix("像素");       //设置输出显示前缀 
	QLabel* ui_min_area_label = new QLabel(this);
	ui_min_area_label->setText(tr("最小面积"));

	ui_max_area = new QSpinBox(this);
	ui_max_area->setRange(0, 1000);       //设置变化范围  
	ui_max_area->setSuffix("像素");       //设置输出显示前缀 
	QLabel* ui_max_area_label = new QLabel(this);
	ui_max_area_label->setText(tr("最大面积"));

	QGridLayout* ui_grid_layout_main = new QGridLayout(this);
	ui_grid_layout_main->addWidget(ui_num_fish_label, 0, 0);
	ui_grid_layout_main->addWidget(ui_num_fish, 0, 1);
	ui_grid_layout_main->addWidget(ui_segment_threshold_label, 1, 0);
	ui_grid_layout_main->addWidget(ui_segment_threshold, 1, 1);
	ui_grid_layout_main->addWidget(ui_min_area_label, 2, 0);
	ui_grid_layout_main->addWidget(ui_min_area, 2, 1);
	ui_grid_layout_main->addWidget(ui_max_area_label,3, 0);
	ui_grid_layout_main->addWidget(ui_max_area,3, 1);
}

void ImgProcessSet_view::set_up_Data()
{
	ui_segment_threshold->setValue(_img_process_set->get_segment_threshold());
	ui_min_area->setValue(_img_process_set->get_min_area());
	ui_max_area->setValue(_img_process_set->get_max_area());
	ui_num_fish->setValue(_img_process_set->get_num_fish());
}