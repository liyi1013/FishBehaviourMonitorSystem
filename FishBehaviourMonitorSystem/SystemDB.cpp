#include"SystemDB.h"


SysDB::SysDB()
{
}

SysDB::~SysDB()
{
	db->close();
}

void SysDB::AddConnection(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port)
{
	_driverName = driver;
	_databaseName = dbName;
	_hostName = host;
	_userName = user;
	_password = passwd;
	_port = port;

	db = new QSqlDatabase(QSqlDatabase::addDatabase(driver));
	db->setDatabaseName(dbName);
	db->setHostName(host);
	db->setPort(port);

	if (!db->open(user, passwd))
	{
		QMessageBox::critical(0, QObject::tr("Database Error"),
			db->lastError().text());
	}
	else
	{
	}
}


QSqlDatabase SysDB::CurrentDatabase() const//返回当前的数据库
{
	return *db;
}


bool SysDB::InsertNewMonitor(QString id, QString name, int num_fish, QString remark, QString date){
	QString unit_id = "001";
	QSqlQuery query;
	if (query.exec(tr("INSERT INTO monitor_tab VALUES ('%1','%2','%3','%4','%5','%6')")
		.arg(id)
		.arg(name)
		.arg(unit_id)
		.arg(num_fish)
		.arg(remark)
		.arg(date)
		)){
		emit DBChanged();
		return true;
	}
	else{
		QMessageBox::critical(0, QObject::tr("Database Error"),
			query.lastError().text());
		return false;
	}
}

bool SysDB::InsertNewVideo(int video_id, int monitor_id, QString path, QString name, QString timestamp){
	QSqlQuery query;
	if (query.exec(tr("INSERT INTO video_tab VALUES ('%1','%2','%3','%4','%5')")
		.arg(video_id)
		.arg(monitor_id)
		.arg(path)
		.arg(name)
		.arg(timestamp)
		)){
		emit DBChanged();
		return true;
	}
	else{
		QMessageBox::critical(0, QObject::tr("Database Error"),
			query.lastError().text());
		return false;
	}
}

bool SysDB::Insert_warning(int monitor_id, QString video_id, int mode){

	int warning_id = 0;
	QSqlQuery query;
	if (query.exec("select max(warning_ID) from warning_tab")){
		while (query.next()){ warning_id = query.value(0).toInt() + 1; }
	}

	if (query.exec(tr("INSERT INTO warning_tab VALUES ('%1','%2','%3','%4','%5','%6')")
		.arg(warning_id)
		.arg(monitor_id)
		.arg(video_id)
		.arg(mode)
		.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"))
		.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"))
		)){
		return true;
	}
	else{
		//QMessageBox::critical(0, QObject::tr("Database Error"),query.lastError().text());
		return false;
	}
}

// 2015-06-12 20:10
bool SysDB::InsertNewRecord(QString video_id, const QString &path, int num_fish, QString time_begin, const QString &remark){
	QSqlQuery query;
	if (query.exec(tr("INSERT INTO record_tab VALUES ('%1','%2','%3','%4','%5','%6','%7')")
		.arg(video_id)
		.arg(path)
		.arg(num_fish)
		.arg(time_begin)
		.arg(time_begin)
		.arg(remark)
		.arg(0)
		)){
		emit DBChanged();
		return true;
	}
	else{
		QMessageBox::critical(0, QObject::tr("Database Error"),
			query.lastError().text());
		return false;
	}
}

bool SysDB::InsertNewRecord_endtime(QString time_end, QString video_id)
{
	QSqlQuery query;
	if (query.exec(tr("UPDATE record_tab SET time_end = '%1' WHERE video_id = '%2'")
		.arg(time_end)
		.arg(video_id)
		)){
		emit DBChanged();
		return true;
	}
	else{
		QMessageBox::critical(0, QObject::tr("Database Error"),
			query.lastError().text());
		return false;
	}
}

// 有问题，得到的是“.avi”
QString SysDB::get_del_file_name(){
	QSqlQuery query;
	QString path;
	QString name;
	if (query.exec(tr("select path,video_id from record_tab where flag>=0 order by video_id"))){
		while (query.next()){
			path = query.value(0).toString();
			name = query.value(1).toString();
			query.exec(tr("update record_tab set flag = -1 where video_id = '%1'").arg(name));
			name += ".avi";
			break;
		}
	}
	return path + name;
}

/***************************************************************************/

SysDB_view* SysDB_view::_instance = nullptr;
SysDB_view::SysDB_view(QWidget *parent, SysDB *sys_db) : QWidget(parent)
{
	_sys_db = sys_db;
	if (_sys_db){
		connect(_sys_db, &SysDB::DBChanged, this, &SysDB_view::updata_DB_view);
	}

	this->setupUI();
	connect(_button, &QPushButton::clicked, this, &SysDB_view::find_by_time);
	connect(_record_tab, &QTableView::doubleClicked, this, &SysDB_view::on_tableView_doubleClicked);
	this->setupModel();

	this->setWindowTitle(tr("数据库查看"));
}

SysDB_view::~SysDB_view()
{

}


void SysDB_view::setupUI()
{
	/*
	// GUI 初始化
	_monitor_tab = new QTableView();
	_monitor_tab->setSelectionBehavior(QAbstractItemView::SelectRows);//设定选择行为，按行选择
	_monitor_tab->setEditTriggers(QTableView::NoEditTriggers);//非编辑模式
	_monitor_tab->resizeColumnsToContents();//由内容调整列
	//_monitor_tab->verticalHeader()->setVisible(false);   //隐藏列表头

	_video_tab = new QTableView();
	_video_tab->setSelectionBehavior(QAbstractItemView::SelectRows);//设定选择行为，按行选择
	_video_tab->setEditTriggers(QTableView::NoEditTriggers);//非编辑模式
	//_video_tab->verticalHeader()->setVisible(false);   //隐藏列表头
	*/

	//2015-06-14
	_record_tab = new QTableView(this);
	_record_tab->setSelectionBehavior(QAbstractItemView::SelectRows);//设定选择行为，按行选择
	_record_tab->setEditTriggers(QTableView::NoEditTriggers);//非编辑模式
	//_record_tab->verticalHeader()->setVisible(false);   //隐藏列表头
	//

	_warning_tab = new QTableView(this);
	_warning_tab->setSelectionBehavior(QAbstractItemView::SelectRows);//设定选择行为，按行选择
	_warning_tab->setEditTriggers(QTableView::NoEditTriggers);//非编辑模式
	//_warning_tab->verticalHeader()->setVisible(false);   //隐藏列表头

	//* 按时间选择 控件
	_hboxLayout_timeedit = new QHBoxLayout;

	_begin_time_Edit = new QDateTimeEdit(QDateTime::currentDateTime());
	_end_time_Edit = new QDateTimeEdit(QDateTime::currentDateTime());
	_begin_time_Edit->setDisplayFormat("yyyy-MM-dd hh:mm");
	_end_time_Edit->setDisplayFormat("yyyy-MM-dd hh:mm");

	_button = new QPushButton("查找");
	_button->setFixedWidth(60);

	_hboxLayout_timeedit->addWidget(new QLabel("起始时间："));
	_hboxLayout_timeedit->addWidget(_begin_time_Edit);
	_hboxLayout_timeedit->addWidget(new QLabel("终止时间："));
	_hboxLayout_timeedit->addWidget(_end_time_Edit);
	_hboxLayout_timeedit->addWidget(_button);
	//* 按时间选择 控件 end

	//* layout

	_vboxLayout_left = new QVBoxLayout();
	_vboxLayout_right = new QVBoxLayout();
	_HboxLayout_main = new QHBoxLayout(this);

	/*
	_vboxLayout_left->addWidget(new QLabel(tr("监测列表")));
	_vboxLayout_left->addWidget(_monitor_tab);
	_vboxLayout_left->addWidget(new QLabel(tr("视频列表")));
	_vboxLayout_left->addWidget(_video_tab);
	_vboxLayout_left->addLayout(_hboxLayout_timeedit);
	*/
	_vboxLayout_left->addWidget(new QLabel(tr("记录列表")));
	_vboxLayout_left->addWidget(_record_tab);
	_vboxLayout_left->addLayout(_hboxLayout_timeedit);


	QLabel *ui_warning_label = new QLabel(this);
	ui_warning_label->setText(tr("警报列表"));
	_vboxLayout_right->addWidget(ui_warning_label);
	_vboxLayout_right->addWidget(_warning_tab);

	_HboxLayout_main->addLayout(_vboxLayout_left);
	_HboxLayout_main->addLayout(_vboxLayout_right);

	this->setLayout(_HboxLayout_main);
	this->setMinimumSize(640, 480);
}

void SysDB_view::setupModel()
{
	/*
	// setup model
	_monitor_model = new QSqlTableModel(_monitor_tab, _sys_db->CurrentDatabase());

	_monitor_model->setEditStrategy(QSqlTableModel::OnRowChange);
	_monitor_model->setTable("monitor_tab");
	_monitor_model->select();
	//set header name
	_monitor_model->setHeaderData(_monitor_model->fieldIndex("monitor_ID"), Qt::Horizontal, tr("监测编号"));
	_monitor_model->setHeaderData(_monitor_model->fieldIndex("unit_ID"), Qt::Horizontal, tr("监测点"));
	_monitor_model->setHeaderData(_monitor_model->fieldIndex("name"), Qt::Horizontal, tr("监测名"));
	_monitor_model->setHeaderData(_monitor_model->fieldIndex("num_fish"), Qt::Horizontal, tr("鱼条数"));
	_monitor_model->setHeaderData(_monitor_model->fieldIndex("remark"), Qt::Horizontal, tr("备注"));
	_monitor_model->setHeaderData(_monitor_model->fieldIndex("timestamp"), Qt::Horizontal, tr("视频拍摄起始时间"));

	//_customerTab->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);


	_monitor_tab->setModel(_monitor_model);


	_video_model = new QSqlTableModel(_video_tab, _sys_db->CurrentDatabase());
	_video_model->setTable("video_tab");
	_video_model->select();
	_video_model->setHeaderData(_video_model->fieldIndex("timestamp"), Qt::Horizontal, tr("视频拍摄起始时间"));
	_video_model->setHeaderData(_video_model->fieldIndex("video_ID"), Qt::Horizontal, tr("视频编号"));
	_video_model->setHeaderData(_video_model->fieldIndex("video_name"), Qt::Horizontal, tr("文件名"));
	_video_model->setHeaderData(_video_model->fieldIndex("monitor_ID"), Qt::Horizontal, tr("监测编号"));
	_video_model->setHeaderData(_video_model->fieldIndex("path"), Qt::Horizontal, tr("视频路径"));

	_video_tab->setModel(_video_model);
	*/

	_warning_model = new QSqlTableModel(_warning_tab, _sys_db->CurrentDatabase());
	_warning_model->setTable("warning_tab");
	_warning_model->select();
	_warning_model->setHeaderData(_warning_model->fieldIndex("time_begin"), Qt::Horizontal, tr("异常起始时间"));
	_warning_model->setHeaderData(_warning_model->fieldIndex("video_ID"), Qt::Horizontal, tr("视频编号"));
	_warning_model->setHeaderData(_warning_model->fieldIndex("video_name"), Qt::Horizontal, tr("文件名"));
	_warning_model->setHeaderData(_warning_model->fieldIndex("monitor_ID"), Qt::Horizontal, tr("监测编号"));
	_warning_model->setHeaderData(_warning_model->fieldIndex("mode"), Qt::Horizontal, tr("特征"));
	_warning_tab->setModel(_warning_model);


	_record_model = new QSqlTableModel(_record_tab, _sys_db->CurrentDatabase());
	_record_model->setTable("record_tab");
	_record_model->select();
	_record_model->setHeaderData(_record_model->fieldIndex("time_begin"), Qt::Horizontal, tr("起始时间"));
	_record_model->setHeaderData(_record_model->fieldIndex("time_end"), Qt::Horizontal, tr("结束时间"));
	_record_model->setHeaderData(_record_model->fieldIndex("video_ID"), Qt::Horizontal, tr("视频编号"));
	_record_model->setHeaderData(_record_model->fieldIndex("video_name"), Qt::Horizontal, tr("文件名"));
	_record_model->setHeaderData(_record_model->fieldIndex("monitor_ID"), Qt::Horizontal, tr("监测编号"));
	_record_model->setHeaderData(_record_model->fieldIndex("num_fish"), Qt::Horizontal, tr("鱼条数"));
	_record_tab->setModel(_record_model);
}

void SysDB_view::updata_DB_view()
{
	/*
	_monitor_model->select();
	_video_model->select();
	*/
	_record_model->select();
}

void SysDB_view::find_by_time()
{
	QString Filter{ "time_begin > '" + _begin_time_Edit->text() + "' and time_begin < '" + _end_time_Edit->text() + "'" };
	_record_model->setFilter(Filter);

	//select data
	_record_model->select();
	_record_tab->setModel(_record_model);
	_record_tab->resizeColumnsToContents();//由内容调整列
}

void SysDB_view::on_tableView_doubleClicked(const QModelIndex &index){
	current_index = index;
	//cmenu->exec(QCursor::pos());//在当前鼠标位置显示;

	QModelIndex i = current_index.sibling(current_index.row(), 1); //path
	QModelIndex j = current_index.sibling(current_index.row(), 0);
	QModelIndex videotime = current_index.sibling(current_index.row(), 3);
	QModelIndex videotime_end = current_index.sibling(current_index.row(), 4);
	QString file_path_name = i.data().toString() + "/" + j.data().toString();

	QModelIndex fish_num = current_index.sibling(current_index.row(), 2);
	int num_fish = fish_num.data().toInt();

	VideoDataDisplayer *vd_player = new VideoDataDisplayer(num_fish);

	vd_player->setAttribute(Qt::WA_DeleteOnClose, true);//自动释放内存
	vd_player->show();
	vd_player->setTitle(tr("<b>视频文件:</b>%1.avi  <b>起始时间:</b>%2   <b>结束时间:</b>%3")
		.arg(j.data().toString())
		.arg(videotime.data().toString())
		.arg(videotime_end.data().toString())
		);
	if (vd_player->openFile(file_path_name)){

	}
	else{
		delete vd_player;
	}
	
}