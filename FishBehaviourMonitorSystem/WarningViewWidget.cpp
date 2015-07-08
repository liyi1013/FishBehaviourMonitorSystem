#include "WarningViewWidget.h"


WarningViewWidget::WarningViewWidget(QWidget *parent) : QWidget(parent)
{
	warningList = new QListWidget(this);
	QVBoxLayout * ui_layout_main = new QVBoxLayout(this);
	ui_layout_main->setContentsMargins(5, 5, 5, 5);
	ui_layout_main->addWidget(warningList);

	this->setLayout(ui_layout_main);

	warningList->setDragEnabled(true);
	this->setMaximumWidth(320 * 1.5 + 10);

	/*
	for (int i = 0; i < 150; ++i)
	{
	add_warning_item(i % 3, i % 3 , QString(tr("2015-5-15 %1").arg(i)));
	}
	*/
}


WarningViewWidget::~WarningViewWidget()
{
}


void WarningViewWidget::on_btn_Add_clicked()
{
	warningList->addItem(tr("new item : "));

	//多选
	//    const QList<QListWidgetItem *>   items = ui->lvWid_Source->selectedItems();
	//    foreach (QListWidgetItem * item, items) {
	//        item->text();
	//        item->setTextColor(Qt::blue);
	//    }
}

void  WarningViewWidget::on_btn_Del_clicked()
{
	if (warningList->currentItem() != Q_NULLPTR)
	{
		QListWidgetItem * item = warningList->takeItem(warningList->currentRow());
		delete item;
	}
}

void WarningViewWidget::add_warning_item(size_t degree, int modeindex, QString text){
	QIcon *icon;
	switch (degree)
	{
	case 1:
		icon = new QIcon("yellowdot.bmp");
		break;
	case 2:
		icon = new QIcon("reddot.ico");
		break;
	default:
		icon = new QIcon("reddot.ico");
		break;
	}
	QString mode;
	switch (modeindex)
	{
	case 1:
		mode = tr("速度异常! ");
		break;
	case 2:
		mode = tr("尾频异常! ");
		break;
	default:
		mode = tr("其他异常! ");
		break;
	}
	mode += text;
	QListWidgetItem *newtiem = new QListWidgetItem(*icon, mode, 0, 0);

	warningList->insertItem(0, newtiem);

	if (warningList->count() > 10)//如果warningList中的item数量超过10
	{
		warningList->takeItem(10);
		QListWidgetItem * item = warningList->takeItem(10);//脱离关系
		delete item;    //删除warningList中最后一项
	}
	delete icon;
}