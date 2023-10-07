#pragma once

#ifndef __FUNCTION_STUDY_WIDGET_H__
#define __FUNCTION_STUDY_WIDGET_H__



#include <qwidget.h>
#include <qstring.h>
#include <qtimer.h>
#include <QPainter>
#include <qmovie.h>
#include <qlabel.h>
#include <QVBoxLayout>
#include <vector>
#include <qpushbutton.h>
#include <QLinkedList>
#include <QDir>
#include <QCoreApplication>

#include <map>
#include <vector>
#include <memory>
#include <filesystem>

#define GIF_PLAY_TIME 3500

class FunctionStudyWidget : public QWidget
{
	Q_OBJECT
public:
	typedef struct
	{
		QStringList path;
	}Data;


	typedef struct
	{
		QString version;
		QString orderId;
		QString orderNumber;
		QString designOrganization;

	}versionData;

	//默认构造函数
	FunctionStudyWidget();
	//传入资源路径的构造函数
	FunctionStudyWidget(const QString& moviesPath);
	//构造函数，构造时载入指定的id和对应的资源信息
	FunctionStudyWidget(const int& id, const QStringList& data);
	//更新当前展示的id并展示对应的图片资源
	void updateId(int id);
	//设置版本数据
	void setVersion(const versionData& verData);
	//设置资源路径并载入资源
	void setMoviesPath(const QString& path);

	//重写show函数
	void show();
	//重写close函数
	void close();

	~FunctionStudyWidget() = default;
private:

	//按钮点击函数
	void slotButtonPush(int buttonSit);
	//添加id对应的资源路径
	void addMovies(int id, Data data);
	//初始化所有成员数据
	void initUi();
	//展示Gif图片	
	void showMovies();
	//载入对应的图片资源
	void loadMovies();
	//载入对应的图片按钮
	void loadMoviesButton();
	//载入资源路径中的资源
	void loadMoviesPath();
	//载入对应Ui的Qss文件
	void loadQssFile();

protected:
	//重写paint函数
	void paintEvent(QPaintEvent* event) override;
private:
	
	//父类窗口
	std::shared_ptr<QWidget> perantWidgetForShow;
	//资源文件
	QMap<int, Data> datas_;
	//存放IDS
	std::list<int> showIds_;
	//存放动画列表
	QVector<std::shared_ptr<QMovie>> movies_;
	//要展示的版本数据
	versionData verData_;
	//资源数据路径
	QString moviesPath;
	
	//窗口垂直布局
	std::shared_ptr<QVBoxLayout> showLayout_;
	//按钮水平布局
	std::shared_ptr<QHBoxLayout> buttinLayout_;
	//设置总垂直布局
	std::shared_ptr<QVBoxLayout> mainLayout_;
	//存储按钮控件
	QVector<std::shared_ptr<QPushButton>> buttonList_;
	//存放当前gif图片展示
	std::shared_ptr<QLabel> showGifLabel_;

	//用于展示当前图片
	static int playTimes;
	int preShowGif;
	//当前展示的id
	int nowShowId_;
	//判断是否点击过
	bool isClickButton;
	//计时器
	QTimer timer_;
};

#endif // !__FUNCTION_STUDY_WIDGET_H__
