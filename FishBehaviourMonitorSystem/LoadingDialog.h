#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QtWidgets\qdialog.h>
#include <QtWidgets\QLabel>
#include <QtWidgets\qlayout.h>

#include <QPainter>
#include <QMovie>

class LoadingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoadingDialog(QDialog *parent = 0, QString text = "");
	~LoadingDialog();

private:

	QMovie *movie;
	QLabel *label;
	QLabel *label_2;
};

#endif // LOADINGDIALOG_H
