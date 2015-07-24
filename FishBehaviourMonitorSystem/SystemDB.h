# pragma execution_character_set("utf-8")//解决中文乱码
#pragma once
#include "qobject.h"


#include <QWidget>

#include<QtWidgets\qtableview.h>

#include<QtWidgets\qlayout.h>
#include<QtWidgets\qtableview.h>
#include<QtWidgets\QDateTimeEdit>
#include<QtWidgets\QPushButton>
#include<QtWidgets/qlabel.h>
#include<qtimer.h>

#include<QtSql\QSqlTableModel>
#include<QtSql\qsql.h>
#include<QtSql\qsqldriver.h>
#include<QtSql\qsqldatabase.h> //db
#include<QtSql\qsqlquery.h>
#include<QtSql\qsqlrelationaltablemodel.h>
#include<QtSql\qsqlerror.h>

#include<QMessageBox>

#include"VideoDataDisplayer.h"

class SysDB :
	public QObject
{
	Q_OBJECT
public:
	SysDB();
	~SysDB();
	// 添加一个新的数据库连接
	void AddConnection(const QString &driver, const QString &dbName, const QString &host,
		const QString &user, const QString &passwd, int port);

	//返回当前连接的数据库
	QSqlDatabase CurrentDatabase() const;
	//插入 新的监测信息
	bool InsertNewMonitor(QString id, QString name, int num_fish, QString remark, QString date);
	//插入 新的视频文件信息
	bool InsertNewVideo(int video_id, int monitor_id, QString path, QString name, QString timestamp);
	//删除 监测

	//插入 warning
	bool Insert_warning(int monitor_id, QString video_id, int mode);

	// 2015-06-12 19:48
	// [1]插入新的记录
	bool InsertNewRecord(QString video_id, const QString &path, int num_fish, QString time_begin, const QString &remark);
	// [2]更新结束时间
	bool InsertNewRecord_endtime(QString time_end, QString video_id);

	QString get_del_file_name();

signals:
	void DBChanged(); // 数据库内容发生变化  //只在内部发出

private:
	QString _driverName;
	QString _databaseName;
	QString _hostName;
	QString _userName;
	QString _password;
	QString _port;

	QSqlDatabase *db;
};


class SysDB_view : public QWidget
{
	Q_OBJECT
public:
	static SysDB_view *_instance;//唯一实例 指针
	static SysDB_view *instance(QWidget *parent = 0, SysDB *s = 0)
	{
		if (!_instance)
			_instance = new SysDB_view(parent, s);
		return _instance;
	}

	SysDB_view(QWidget *parent = 0, SysDB *sys_db = 0);
	~SysDB_view();

private:
	SysDB *_sys_db;

	// 2015-06-14
	QTableView *_record_tab;
	QSqlTableModel *_record_model;
	//

	QTableView *_warning_tab;
	QSqlTableModel *_warning_model;

	QDateTimeEdit *_begin_time_Edit;
	QDateTimeEdit *_end_time_Edit;
	QPushButton   *_button;

	QVBoxLayout *_vboxLayout_left;
	QVBoxLayout *_vboxLayout_right;
	QHBoxLayout *_HboxLayout_main;

	QLayout *_hboxLayout_timeedit;

public:

	void updata_DB_view();
	void find_by_time();  //video_tab
	void on_tableView_doubleClicked(const QModelIndex &index);
private:
	void setupUI();
	void setupModel();

	QModelIndex current_index;//
};

