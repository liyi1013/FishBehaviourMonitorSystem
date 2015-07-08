#include "NewMonitor_dlg.h"

NewMonitor_dlg::NewMonitor_dlg()
{
	this->setWindowTitle(tr("新建监测记录"));

	this->set_up_ui();

	QSqlQuery query;
	if (query.exec("select max(monitor_ID) from monitor_tab")){
		while (query.next()){
			ID_Edit->setText(QString::number(query.value(0).toInt() + 1));
		}
	}
	else{
		ID_Edit->setText(QString::number(1));
	}

	QString unit_id;
	if (query.exec("select unit_ID from unit_info")){
		while (query.next()){
			unit_id = query.value(0).toString();
		}
	}
	QString name = current_date_time.toString("yyyyMMddhhmm");

	connect(this->accept_Button, &QPushButton::clicked, this, &NewMonitor_dlg::my_accept);
	connect(this->cancel_Button, &QPushButton::clicked, this, &NewMonitor_dlg::my_cancel);
}


NewMonitor_dlg::~NewMonitor_dlg()
{
}

void NewMonitor_dlg::set_up_ui(){

	packagesGroup = new QGroupBox(this);
	packagesGroup->setTitle(tr("新建监测记录"));
	packagesGroup->move(10, 10);
	packagesGroup->setFixedWidth(280);

	ID_Label = new QLabel(tr("<b>视频编号：</b>"));
	ID_Edit = new QLineEdit;

	fish_num_Label = new QLabel(tr("<b>鱼条数：</b>"));
	fish_num_SpinBox = new QSpinBox;
	fish_num_SpinBox->setMinimum(1);
	fish_num_SpinBox->setMaximum(20);
	fish_num_SpinBox->setSingleStep(1);

	current_date_time = QDateTime::currentDateTime();
	current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

	date_Label = new QLabel(current_date);

	accept_Button = new QPushButton(this); accept_Button->setText("确定");
	cancel_Button = new QPushButton(this); cancel_Button->setText("取消");
	accept_Button->setFixedWidth(70);
	cancel_Button->setFixedWidth(70);
	accept_Button->move(140, 120);
	cancel_Button->move(220, 120);

	packagesLayout = new QGridLayout;

	QLabel *data_label_text = new QLabel(this);
	data_label_text->setText(tr("<b>记录起始时间：</b>"));

	packagesLayout->addWidget(ID_Label, 1, 0);
	packagesLayout->addWidget(ID_Edit, 1, 1);

	packagesLayout->addWidget(fish_num_Label, 3, 0);
	packagesLayout->addWidget(fish_num_SpinBox, 3, 1);

	packagesLayout->addWidget(data_label_text, 0, 0);
	packagesLayout->addWidget(date_Label, 0, 1);

	packagesGroup->setLayout(packagesLayout);

	this->setFixedSize(300, 160);
}

bool NewMonitor_dlg::my_accept(){
	QSqlQuery query;
	if (query.exec(tr("select * from monitor_tab where monitor_ID=%1").arg(ID_Edit->text())))
	{
		if (query.next()){
			QString id = ID_Edit->text();
			ID_Edit->setText(id + tr(" 编号已经存在！"));
		}
		else{
			this->accept();
			return 1;
		}
	}
	return false;
}

void NewMonitor_dlg::my_cancel(){
	this->reject();
}