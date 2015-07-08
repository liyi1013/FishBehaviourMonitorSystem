#pragma once
#include "qwidget.h"

#include<QtWidgets\qwidget.h>
#include<QtWidgets\qlayout.h>
#include<QtWidgets\QLabel>
#include<QtWidgets\QListWidget>
#include<QtWidgets\QListWidgetItem>

/* 用于显示预警信息的控件，*/
class WarningViewWidget : public QWidget
{
public:
	WarningViewWidget(QWidget *parent = 0);
	~WarningViewWidget();

	//*添加一条预警记录，degree等级，mode特征编号，text其他信息（如时间）
	void add_warning_item(size_t degree, int modeindex, QString text);

	void on_btn_Add_clicked();
	void on_btn_Del_clicked();
private:

	QListWidget *warningList;
};

