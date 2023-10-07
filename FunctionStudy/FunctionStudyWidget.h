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

	//Ĭ�Ϲ��캯��
	FunctionStudyWidget();
	//������Դ·���Ĺ��캯��
	FunctionStudyWidget(const QString& moviesPath);
	//���캯��������ʱ����ָ����id�Ͷ�Ӧ����Դ��Ϣ
	FunctionStudyWidget(const int& id, const QStringList& data);
	//���µ�ǰչʾ��id��չʾ��Ӧ��ͼƬ��Դ
	void updateId(int id);
	//���ð汾����
	void setVersion(const versionData& verData);
	//������Դ·����������Դ
	void setMoviesPath(const QString& path);

	//��дshow����
	void show();
	//��дclose����
	void close();

	~FunctionStudyWidget() = default;
private:

	//��ť�������
	void slotButtonPush(int buttonSit);
	//���id��Ӧ����Դ·��
	void addMovies(int id, Data data);
	//��ʼ�����г�Ա����
	void initUi();
	//չʾGifͼƬ	
	void showMovies();
	//�����Ӧ��ͼƬ��Դ
	void loadMovies();
	//�����Ӧ��ͼƬ��ť
	void loadMoviesButton();
	//������Դ·���е���Դ
	void loadMoviesPath();
	//�����ӦUi��Qss�ļ�
	void loadQssFile();

protected:
	//��дpaint����
	void paintEvent(QPaintEvent* event) override;
private:
	
	//���ര��
	std::shared_ptr<QWidget> perantWidgetForShow;
	//��Դ�ļ�
	QMap<int, Data> datas_;
	//���IDS
	std::list<int> showIds_;
	//��Ŷ����б�
	QVector<std::shared_ptr<QMovie>> movies_;
	//Ҫչʾ�İ汾����
	versionData verData_;
	//��Դ����·��
	QString moviesPath;
	
	//���ڴ�ֱ����
	std::shared_ptr<QVBoxLayout> showLayout_;
	//��ťˮƽ����
	std::shared_ptr<QHBoxLayout> buttinLayout_;
	//�����ܴ�ֱ����
	std::shared_ptr<QVBoxLayout> mainLayout_;
	//�洢��ť�ؼ�
	QVector<std::shared_ptr<QPushButton>> buttonList_;
	//��ŵ�ǰgifͼƬչʾ
	std::shared_ptr<QLabel> showGifLabel_;

	//����չʾ��ǰͼƬ
	static int playTimes;
	int preShowGif;
	//��ǰչʾ��id
	int nowShowId_;
	//�ж��Ƿ�����
	bool isClickButton;
	//��ʱ��
	QTimer timer_;
};

#endif // !__FUNCTION_STUDY_WIDGET_H__
