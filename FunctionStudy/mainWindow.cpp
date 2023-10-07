#include "mainWindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QMessageBox>

//默认构造函数，初始化类中所有成员
MainWindow::MainWindow() 
{

	
	//创建一个widget
	QWidget* centralWidget = new QWidget(this);
	//设置对应的layout
	QVBoxLayout* layout = new QVBoxLayout(centralWidget);
	//设置对应的lineEdit
	QLineEdit* lineEdit = new QLineEdit(centralWidget);
	
	//将lineEdit置入layout中
	layout->addWidget(lineEdit);
	//设置button
	QPushButton* button = new QPushButton("get text", centralWidget);
	layout->addWidget(button);
	//设置槽函数
	connect(button, &QPushButton::clicked, this, [lineEdit,this] {
		QMessageBox::information(nullptr, "info", lineEdit->text());
		bool isTextNumber = false;
		int Id = lineEdit->text().toInt(&isTextNumber);
		if (isTextNumber)
		{//若输入全是数字，则直接更新对应的Id
			this->slotUpdateId(Id);
		}
		else
		{
			qDebug() << "input is illegal" << lineEdit->text();
		}

		qDebug() << lineEdit->text();
	});

	setCentralWidget(centralWidget);

	initPlay();
	initSwitch();
	initInSide();

}

//默认析构函数
MainWindow::~MainWindow()
{

}




//模拟切换到下一个场景
void MainWindow::next()
{
	test_->updateId(300);
}

//模拟切换到下一个场景
void MainWindow::pre()
{
	test_->updateId(200);
}

//开始播放
void MainWindow::slotStartplay()
{

	qDebug() << "now init TestWidget";
	if (test_ && !test_->isVisible())
	{//重新展示
		qDebug() << "now reStart TestWidget";
		test_->show();
	}
	else if(!test_)
	{//初始化展示
		qDebug() <<"now init TestWidget";
		test_.reset(new FunctionStudyWidget());
		test_->setMoviesPath("D:/src");
		test_->updateId(100);
		test_->show();
	}
}

//停止播放
void MainWindow::slotStopPlay()
{
	if (test_ && test_->isVisible() )
	{//暂停播放
		test_->close();
	}
}

//载入资源
void MainWindow::slotAddMovies()
{
	
}

//更新Id
void MainWindow::slotUpdateId(int id)
{
	if (test_)
	{
		test_->updateId(id);
	}
	else
	{
		test_.reset(new FunctionStudyWidget("D:/src"));
	}
}

//设置展示版本详细
void MainWindow::slotSetVersion()
{
	QString version = "2023.9.22.01";
	QString orderId = "774";
	QString orderNumber = "ZH Denium_R_02";
	QString designUnit = "[L] 螺丝固定 全冠 36";
	FunctionStudyWidget::versionData versionData = {
		version,orderId,orderNumber,designUnit
	};
	test_->setVersion(versionData);
}

//载入资源路径
void  MainWindow::slotSetPath()
{
	if (test_)
	{
		test_->setMoviesPath("D:/src");
	}
	else
	{
		QString a = "D:/src";
		test_.reset(new FunctionStudyWidget(a));
	}
}

//初始化菜单栏
void MainWindow::initPlay()
{
	QMenu* fileMenu = menuBar()->addMenu("Play");

	// 添加gif开始播放
	QAction* startPlay = new QAction("StartPlay", this);
	fileMenu->addAction(startPlay);
	connect(startPlay, SIGNAL(triggered()), SLOT(slotStartplay()));
	
	// 添加gif停止播放
	QAction* stopPlay = new QAction("StopPlay", this);
	fileMenu->addAction(stopPlay);
	connect(stopPlay, SIGNAL(triggered()), SLOT(slotStopPlay()));

}


//初始化菜单栏
void MainWindow::initSwitch()
{
	QMenu* fileMenu = menuBar()->addMenu("Action");

	// 创建一个动作并添加到菜单
	QAction* nextAction = new QAction("Next", this);
	fileMenu->addAction(nextAction);
	connect(nextAction, SIGNAL(triggered()),this, SLOT(next()));


	// 创建一个动作并添加到菜单
	QAction* preAction = new QAction("Pre", this);
	fileMenu->addAction(preAction);
	connect(preAction, SIGNAL(triggered()), this, SLOT(pre()));
}


//初始化菜单栏
void MainWindow::initInSide()
{

	QMenu* fileMenu = menuBar()->addMenu("SetInfo");

	// 添加载入资源路径
	QAction* addMovies = new QAction("LoadMovies", this);
	fileMenu->addAction(addMovies);
	connect(addMovies, SIGNAL(triggered()), SLOT(slotAddMovies()));

	// 添加更新展示Id
	QAction* updataShowId = new QAction("UpdataId", this);
	fileMenu->addAction(updataShowId);
	connect(updataShowId, SIGNAL(triggered()), SLOT(slotUpdateId()));

	// 添加设置版本信息
	QAction* setVersion = new QAction("SetVersion", this);
	fileMenu->addAction(setVersion);
	connect(setVersion, SIGNAL(triggered()), SLOT(slotSetVersion()));

	//载入资源路径
	QAction* setMoviesPath = new QAction("SetMoviesPath", this);
	fileMenu->addAction(setMoviesPath);
	connect(setMoviesPath, SIGNAL(triggered()), SLOT(slotSetPath()));
}