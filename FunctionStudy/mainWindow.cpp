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

//Ĭ�Ϲ��캯������ʼ���������г�Ա
MainWindow::MainWindow() 
{

	
	//����һ��widget
	QWidget* centralWidget = new QWidget(this);
	//���ö�Ӧ��layout
	QVBoxLayout* layout = new QVBoxLayout(centralWidget);
	//���ö�Ӧ��lineEdit
	QLineEdit* lineEdit = new QLineEdit(centralWidget);
	
	//��lineEdit����layout��
	layout->addWidget(lineEdit);
	//����button
	QPushButton* button = new QPushButton("get text", centralWidget);
	layout->addWidget(button);
	//���òۺ���
	connect(button, &QPushButton::clicked, this, [lineEdit,this] {
		QMessageBox::information(nullptr, "info", lineEdit->text());
		bool isTextNumber = false;
		int Id = lineEdit->text().toInt(&isTextNumber);
		if (isTextNumber)
		{//������ȫ�����֣���ֱ�Ӹ��¶�Ӧ��Id
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

//Ĭ����������
MainWindow::~MainWindow()
{

}




//ģ���л�����һ������
void MainWindow::next()
{
	test_->updateId(300);
}

//ģ���л�����һ������
void MainWindow::pre()
{
	test_->updateId(200);
}

//��ʼ����
void MainWindow::slotStartplay()
{

	qDebug() << "now init TestWidget";
	if (test_ && !test_->isVisible())
	{//����չʾ
		qDebug() << "now reStart TestWidget";
		test_->show();
	}
	else if(!test_)
	{//��ʼ��չʾ
		qDebug() <<"now init TestWidget";
		test_.reset(new FunctionStudyWidget());
		test_->setMoviesPath("D:/src");
		test_->updateId(100);
		test_->show();
	}
}

//ֹͣ����
void MainWindow::slotStopPlay()
{
	if (test_ && test_->isVisible() )
	{//��ͣ����
		test_->close();
	}
}

//������Դ
void MainWindow::slotAddMovies()
{
	
}

//����Id
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

//����չʾ�汾��ϸ
void MainWindow::slotSetVersion()
{
	QString version = "2023.9.22.01";
	QString orderId = "774";
	QString orderNumber = "ZH Denium_R_02";
	QString designUnit = "[L] ��˿�̶� ȫ�� 36";
	FunctionStudyWidget::versionData versionData = {
		version,orderId,orderNumber,designUnit
	};
	test_->setVersion(versionData);
}

//������Դ·��
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

//��ʼ���˵���
void MainWindow::initPlay()
{
	QMenu* fileMenu = menuBar()->addMenu("Play");

	// ���gif��ʼ����
	QAction* startPlay = new QAction("StartPlay", this);
	fileMenu->addAction(startPlay);
	connect(startPlay, SIGNAL(triggered()), SLOT(slotStartplay()));
	
	// ���gifֹͣ����
	QAction* stopPlay = new QAction("StopPlay", this);
	fileMenu->addAction(stopPlay);
	connect(stopPlay, SIGNAL(triggered()), SLOT(slotStopPlay()));

}


//��ʼ���˵���
void MainWindow::initSwitch()
{
	QMenu* fileMenu = menuBar()->addMenu("Action");

	// ����һ����������ӵ��˵�
	QAction* nextAction = new QAction("Next", this);
	fileMenu->addAction(nextAction);
	connect(nextAction, SIGNAL(triggered()),this, SLOT(next()));


	// ����һ����������ӵ��˵�
	QAction* preAction = new QAction("Pre", this);
	fileMenu->addAction(preAction);
	connect(preAction, SIGNAL(triggered()), this, SLOT(pre()));
}


//��ʼ���˵���
void MainWindow::initInSide()
{

	QMenu* fileMenu = menuBar()->addMenu("SetInfo");

	// ���������Դ·��
	QAction* addMovies = new QAction("LoadMovies", this);
	fileMenu->addAction(addMovies);
	connect(addMovies, SIGNAL(triggered()), SLOT(slotAddMovies()));

	// ��Ӹ���չʾId
	QAction* updataShowId = new QAction("UpdataId", this);
	fileMenu->addAction(updataShowId);
	connect(updataShowId, SIGNAL(triggered()), SLOT(slotUpdateId()));

	// ������ð汾��Ϣ
	QAction* setVersion = new QAction("SetVersion", this);
	fileMenu->addAction(setVersion);
	connect(setVersion, SIGNAL(triggered()), SLOT(slotSetVersion()));

	//������Դ·��
	QAction* setMoviesPath = new QAction("SetMoviesPath", this);
	fileMenu->addAction(setMoviesPath);
	connect(setMoviesPath, SIGNAL(triggered()), SLOT(slotSetPath()));
}