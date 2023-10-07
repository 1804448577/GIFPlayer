#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__


#include <QDir>
#include <QDialog>
#include <QStringList>
#include <QMainWindow>

#include <qlabel.h>
#include <memory>

#include "FunctionStudyWidget.h"

using std::shared_ptr;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

	//初始化菜单
	void initPlay();
	void initSwitch();
	void initInSide();
public slots:
	//模拟操作
	void next();
	void pre();


	///播放gif相关操作

	//开始播放GIF
	void slotStartplay();
	//暂停播放
	void slotStopPlay();
	//添加资源
	void slotAddMovies();
	//更新id
	void slotUpdateId(int id);
	//设置版本信息
	void slotSetVersion();
	//设置资源路劲
	void slotSetPath();

private:
	//菜单栏
	shared_ptr<QMenuBar> menuBar_;
	//图片播放
	shared_ptr<FunctionStudyWidget> gifPlayer_;
	shared_ptr<QMovie> gifSource_;
	shared_ptr<QLabel> label_;

	//GIF播放类
	shared_ptr<FunctionStudyWidget> test_;
};

#endif // !__MAIN_WINDOW_H__
