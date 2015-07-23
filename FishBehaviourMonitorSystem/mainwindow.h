#pragma execution_character_set("utf-8")//解决中文乱码
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets\QMainWindow>
#include <QtWidgets\qaction.h>
#include <QtWidgets\qdockwidget.h>
#include <QtWidgets\qstatusbar.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qtoolbar.h>

class QMessageBox;

#include"VideoProcessing.h"

#include"ImgShowWidget.h"
#include"DataShowWidget.h"
#include"WarningViewWidget.h"

#include"SystemSet.h"
#include"SystemDB.h"

#include"NewMonitor_dlg.h"

#include"ImgProcessSet.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, VideoProcessing *vp = 0, QTimer *t = 0, SystemSet* sys_set = 0, SysDB* sys_db = 0, ImgProcessSet* imgset=0);
	~MainWindow();

private:
	// Action
	void createActions();
	//QAction *newMonitor;
	QAction *opencamera;  // 1 打开摄像头
	QAction *openfile;    // 1 打开文件
	QAction *background_pickup_Act;    // 1-2 提取背景
	QAction *startAct;    // 2 开始处理
	QAction *recodeAct;   // 3 开始记录
	QAction *endAct;      // 4 结束处理
	QAction *setviewdefaultAct;
	QAction *setAct;
	QAction *DB_manage_Act;
	QAction *aboutAct;
	QAction *exitAct;

	// Menu
	void createMenus();
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *DBMenu;
	QMenu *helpMenu;
	QMenu *ViewSetMenu;
	QMenu *SetMenu;

	// ToolBar
	void setupToolBar();
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *setToolBar;

	// StatusBar
	void createStatusBar();

private:
	void setupUi();
	//ImgShowWidget_opencv *ui_img_view;
	ImgShowWidget_Mat *ui_img_view;

public:
	WarningViewWidget *ui_warning_view;

private:	
	QTabWidget *tabWidget;//data show 
	QWidget *data_show_1;
	QWidget *data_show_2;
	QWidget *data_show_3;

	DataShowWidget *ui_data_view_1;//速度
	DataShowWidget *ui_data_view_2;//尾频
	DataShowWidget *ui_data_view_3;//群聚半径
	DataShowWidget *ui_data_view_4;//指定半径外鱼条数
	DataShowWidget *ui_data_view_5;//群簇数量
	//其他特征
	DataShowWidget *ui_data_view_8;//半死亡数
	DataShowWidget *ui_data_view_9;//死亡条数

	QDockWidget *dock_set;
	QDockWidget *dock_img_process_set;

public:	//给 video_processing 调用
	void updata_img(IplImage *src);//观察vp的变化->显示图像
	void updata_img(cv::Mat &mat); //观察vp的变化->显示图像
	void updata_data(size_t modeIndex, double data); // 

public slots:
	void open_camera();
	void open_file();
	void background_pickup();
	void process_start();
	void process_end();
	void record();

	void DB_manage();

	void system_set();
	//void new_monitor();
	void set_view_default();
	void about();
	

private:
	VideoProcessing* const _video_processing;
	      SystemSet* const _sys_set;
	          SysDB* const _sys_db;

	       QTimer* _timer=nullptr;
	ImgProcessSet* _img_process_set;
};

#endif // MAINWINDOW_H
