#pragma once
#include "qdialog.h"

#include "qdialog.h"
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QRadioButton>

#include <QSqlQuery>
#include <qdebug.h>


#pragma execution_character_set("utf-8")//解决中文乱码

class NewMonitor_dlg :
	public QDialog
{
public:
	NewMonitor_dlg();
	~NewMonitor_dlg();

public slots:
	bool my_accept();
	void my_cancel();

public:
	QString get_id()  { return ID_Edit->text(); }
	int  get_num_fish(){ return fish_num_SpinBox->text().toInt(); }

private:
	void set_up_ui();

private:
	//* UI
	QGroupBox *packagesGroup;

	QLabel *ID_Label;
	QLineEdit *ID_Edit;

	QLabel *fish_num_Label;
	QSpinBox *fish_num_SpinBox;
	//* UI


private:

	QDateTime current_date_time;
	QString current_date;

	QLabel *date_Label;

	QPushButton *accept_Button;
	QPushButton *cancel_Button;

	QGridLayout *packagesLayout;

	bool _is_camera;

};

