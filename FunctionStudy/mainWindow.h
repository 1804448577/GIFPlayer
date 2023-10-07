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

	//��ʼ���˵�
	void initPlay();
	void initSwitch();
	void initInSide();
public slots:
	//ģ�����
	void next();
	void pre();


	///����gif��ز���

	//��ʼ����GIF
	void slotStartplay();
	//��ͣ����
	void slotStopPlay();
	//�����Դ
	void slotAddMovies();
	//����id
	void slotUpdateId(int id);
	//���ð汾��Ϣ
	void slotSetVersion();
	//������Դ·��
	void slotSetPath();

private:
	//�˵���
	shared_ptr<QMenuBar> menuBar_;
	//ͼƬ����
	shared_ptr<FunctionStudyWidget> gifPlayer_;
	shared_ptr<QMovie> gifSource_;
	shared_ptr<QLabel> label_;

	//GIF������
	shared_ptr<FunctionStudyWidget> test_;
};

#endif // !__MAIN_WINDOW_H__
