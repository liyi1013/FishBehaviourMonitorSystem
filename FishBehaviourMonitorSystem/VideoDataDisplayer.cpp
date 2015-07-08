#include "VideoDataDisplayer.h"


VideoDataDisplayer::VideoDataDisplayer(int num_fish)
{
	_timer = new QTimer(this);
	_timer->setInterval(66);
	setupUI(num_fish);
	this->num_fish = num_fish;
}


VideoDataDisplayer::~VideoDataDisplayer()
{
}


bool VideoDataDisplayer::openFile(QString filename)
{
	if (this->ui_videoplayer->Open_File(filename+".avi"))
	{
		if (num_fish == 1){
			this->ui_plotdata_1->openfile(filename + "_1.txt");
			this->ui_plotdata_2->openfile(filename + "_2.txt");
		}else if (num_fish > 1){
			this->ui_plotdata_1->openfile(filename + "_3.txt");
			this->ui_plotdata_2->openfile(filename + "_4.txt");
		}
		_timer->stop();
		return true;
	}else{
		return false;
	}
}

void VideoDataDisplayer::setTitle(QString filename)
{
	ui_title->setText(filename);
	this->update();
}
//--------------------------------------------------------------------------------------------

MyVideoPlayer_1::~MyVideoPlayer_1()
{
}