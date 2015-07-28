#pragma once

#include<QtCore\qobject.h>
#include<QtCore\qthread.h>
#include<QtCore\qtimer.h>

#include"mainwindow.h"
#include"VideoProcessing.h"
#include"SystemSet.h"
#include"SystemDB.h"
#include"ImgProcessSet.h"
#include"sendwatertakingsignal.h"

class App :	public QObject
{
public:
	App();
	~App();
private:
	QTimer*          _t;
	MainWindow*      _main_window;
	VideoProcessing* _video_processing;
	QThread*         _thread_videoprocessing;//视频处理线程

	ImgProcessSet* _imgp_set;
	SystemSet*     _sys_set;
	SysDB*         _sys_db;

	SendWaterTakingSignal* _water_taking_siganl_sender;
};


/*
数据字典：

数字 - 特征  modeIndex
单条
1  - 速度
2  - 尾频率
多条
3  - 群聚半径
4  - 指定半径外鱼条数
5  - 群簇数量
统一
8  - 半死亡数
9  - 死亡条数

存储文件文件名：节点标号+起始时间              (01_201506061100)
视频文件：     节点标号+起始时间.avi          (01_201506061100.avi)
数据文件：     节点标号+起始时间+特征index.txt (01_201506061100_01.txt)
*/

/*
操作流程：
打开软件->
打开摄像头->点击开始处理->
调整分割参数（使得分割效果最好）->
开始记录(记录的过程中不能更改系统设置)->
结束记录（同时停止图像处理）->
关闭摄像头（停止timer，可选）->
结束
*/