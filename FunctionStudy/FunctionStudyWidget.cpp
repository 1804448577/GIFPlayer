#include "FunctionStudyWidget.h"


//����չʾͼƬ
int FunctionStudyWidget::playTimes = 0;

//��дpaintEvent����
void FunctionStudyWidget::paintEvent(QPaintEvent* event) 
{
	QPainter painter(this);
	// ���������
	painter.setRenderHint(QPainter::Antialiasing, true);
	// ���û�ˢ��ɫ
	painter.setBrush(QBrush(Qt::white)); 
	// �����Ʊ߿�
	painter.setPen(Qt::NoPen);
	// ����Բ�Ǿ��Σ�Բ�ǵİ뾶Ϊ10px
	painter.drawRoundedRect(this->rect(), 10, 10); 
}

//��дshow����
void FunctionStudyWidget::show()
{
	//���ñ߿�͸��
	perantWidgetForShow->setWindowFlags(Qt::FramelessWindowHint);
	//���ñ���͸��
	perantWidgetForShow->setAttribute(Qt::WA_TranslucentBackground);
	//չʾ
	perantWidgetForShow->show();

	QWidget::show();
}

//��дshow����
void FunctionStudyWidget::close()
{
	perantWidgetForShow->close();
	QWidget::close();
}

//Ĭ�Ϲ��캯��
FunctionStudyWidget::FunctionStudyWidget()
{
	qDebug() << QCoreApplication::applicationDirPath();
	//ֻ��ʼ����Ҫ��Ui����ͳ�Ա���ݣ���������Դ
	perantWidgetForShow.reset(new QWidget());
	this->setParent(perantWidgetForShow.get());
	this->initUi();
}

//������Դ·���Ĺ��캯��
FunctionStudyWidget::FunctionStudyWidget(const QString& moviesPath) : QWidget()
{
	qDebug() << QCoreApplication::applicationDirPath();
	//��ʼ��Ui���沢������Դ
	this->setMoviesPath(moviesPath);
	perantWidgetForShow.reset(new QWidget());
	this->setParent(perantWidgetForShow.get());


	nowShowId_ = showIds_.front();
	playTimes = 0;

	nowShowId_ = *showIds_.begin();
	this->initUi();
	this->loadMovies();
	this->showMovies();
	this->loadMoviesButton();
}

void FunctionStudyWidget::loadQssFile()
{
	//��ȡ��Ӧ��Qss�ļ�����
	QFile file("../src/QssOfFunctionStudy.qss");
	file.open(QFile::ReadOnly);
	QTextStream stream(&file);
	QString styleSheet = stream.readAll();
	this->setStyleSheet(styleSheet);
}
//���õ�ǰչʾ��Id
void FunctionStudyWidget::updateId(int id)
{
	//���Ҹ�Id�Ƿ���Ids�д���
	auto ifIdFind = std::find(showIds_.begin(),showIds_.end(),id);
	
	if (ifIdFind == showIds_.end())
	{//��û�ҵ��������������־������������
		qDebug() << "not find the id:" << id << "in Ids" << showIds_;
		return;
	}
	
	//���������Id��Դ
	nowShowId_ = id;
	playTimes = 0;

	this->loadMovies();
	this->showMovies();
	this->loadMoviesButton();
}

//���ݰ�ťչʾ��Ӧ��ͼƬ
void FunctionStudyWidget::slotButtonPush(int buttonSit)
{
	buttonList_[buttonSit]->setChecked(true);
	for (int buttonCount = 0; buttonCount < buttonList_.size(); ++buttonCount)
	{
		if(buttonSit != buttonCount)
		{
			buttonList_[buttonCount]->setChecked(false);
		}
	}

	if (isClickButton && buttonSit == playTimes)
	{//��֮ǰ�������ť�����ٽ����ظ����
		return;
	}

	
	//����չʾͼƬ����������ʾ
	playTimes = buttonSit;
	showGifLabel_->setMovie(movies_[playTimes].get());
	movies_[playTimes]->start();
	//�ָ�ԭͼƬ״̬
	movies_[preShowGif]->stop();
	preShowGif = playTimes;


	//���õ��״̬������չʾ�¼�Ϊ9��
	isClickButton = true;
	timer_.setInterval(9000);
	timer_.start();
}

//������Դ·��
void FunctionStudyWidget::setMoviesPath(const QString& path)
{
	//������Դ·��
	moviesPath = path;
	//�����·������Դ
	this->loadMoviesPath(); 
}

//������Դ·���е���Դ
void FunctionStudyWidget::loadMoviesPath()
{
	if (!moviesPath.isEmpty())
	{
		//�ͷ�ԭ������Դ
		datas_.clear();
		showIds_.clear();

		//������Դ·��
		QDir dir(moviesPath);
		//��ȡ��Դ·�������е��ļ�����
		QStringList dirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		QMap<int, QStringList> map;
		
		for (const QString& subDirName : dirList) {
			//����ļ����Ƿ�ȫ������
			bool isDirNameNumber = false;
			//��ȡ����·��
			QDir subDir(dir.absoluteFilePath(subDirName));
			//��ȡ����·���µ�����gif��Դ�ļ�
			QStringList gifList = subDir.entryList(QStringList() << "*.gif", QDir::Files);
			//��Ŷ�Ӧ��·������
			Data pathData;
			//�����洢��Ӧ��·������
			for (const QString& gifName : gifList) {
				pathData.path.append(subDir.absoluteFilePath(gifName));
			}
			//���ļ����Ƿ�Ϊȫ����
			int DirNumber = subDirName.toInt(&isDirNameNumber);
			
			if (isDirNameNumber)
			{//��ȫ�����֣�����Ӹ�·���µ���������
				this->addMovies(DirNumber, pathData);
			}
			else
			{//���������������������־
				qDebug() << "error! the dir:" << subDirName << "can not load gifs because of the name is not number";
			}
		}
	}
	else
	{
		qDebug() << "Path is Empty";
	}
}


void FunctionStudyWidget::loadMovies()
{
	//����ͼƬ��Դ����
	for each (auto movie in movies_)
	{
		movie.reset();
	}

	//��ʼ�����г�Ա����
	movies_.clear();

	// ����һ��QMovie�������飬ÿ������ָ��һ��GIFͼƬ��·��
	for (const auto& gif : datas_[nowShowId_].path) {
		std::shared_ptr<QMovie> movie;
		movie.reset(new QMovie(gif));
		movies_.push_back(movie);
	}
}

//��̬���밴ť
void FunctionStudyWidget::loadMoviesButton()
{
	
	qDebug() << "load Movies";
	if (movies_.empty())
	{//��ͼƬ��ԴΪ��
		qDebug() << "Gifs Is empty!";
		return;
	}

	//����ť��Դ����
	for each (auto button  in buttonList_)
	{
		buttinLayout_->removeWidget(button.get());
		button->deleteLater();
		button.reset();
	}

	//��ʼ�����г�Ա����
	buttonList_.clear();
	isClickButton = false;
	
	if (!buttinLayout_)
	{
		buttinLayout_.reset(new QHBoxLayout());
	}

	//�Ƴ�buttinLayout_������Ԫ��
	QLayoutItem* child;
	while ((child = buttinLayout_->takeAt(0)) != 0) {
		if (QWidget* widget = child->widget()) {
			widget->deleteLater();
		}
		delete child;
	}

	if (datas_.isEmpty())
	{//��ͼƬ��ԴΪ�գ���ֱ�ӷ���
		return;
	}

	//��showLayout_���Ƴ�buttonLayout_
	showLayout_->removeItem(buttinLayout_.get());

	//�ڰ�ť���������ӵ���
	buttinLayout_->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	//��̬��Ӱ�ť
	for (int countForList = 0; countForList < datas_[nowShowId_].path.size(); ++countForList)
	{
		std::shared_ptr<QPushButton> button;
		button.reset(new QPushButton(this));
		button->setFixedSize(8, 8);
		button->setCheckable(true); // ���ð�ťΪ��ѡ��״̬
		buttonList_.push_back(button);
		buttinLayout_->addWidget(button.get());
		connect(button.get(), &QPushButton::clicked, [=]()mutable
			{
				slotButtonPush(countForList);

			}
		);
	}
	//Ĭ����ʾ��һ����ťΪ��ѡ��״̬;
	buttonList_[0]->setChecked(true);
	//�ڰ�ť�����Ҳ���ӵ���
	buttinLayout_->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
	//�����º��buttonLayout_��ӵ�showLayout_��
	showLayout_->addLayout(buttinLayout_.get());


}



//��ʼ�����г�Ա����
void FunctionStudyWidget::initUi()
{
	this->setObjectName("FunctionStudyWidget");
	//��ʼ��showLayout_ , mainLayout_
	showLayout_.reset(new QVBoxLayout(this));
	mainLayout_.reset(new QVBoxLayout(this));
	//����QSS�ʹ�С
	loadQssFile();
	showLayout_->setContentsMargins(8, 8, 8, 4);
	//���ò����봰��֮��ļ�϶ֵ
	showLayout_->addStretch(); 
	this->setLayout(showLayout_.get());
}

//��ʼ��չʾ
void FunctionStudyWidget::showMovies()
{
	if (movies_.isEmpty())
	{//��ͼƬ��ԴΪ�գ���ֱ�ӷ���
		return;
	}
	if (!showGifLabel_)
	{
		showGifLabel_.reset(new QLabel(this));
		showGifLabel_->setObjectName("ShowGifLabel");
		//����ͼ��չʾ��׼
		showGifLabel_->setFixedSize(256, 256);
		//����lable�Զ���䣬
		showGifLabel_->setScaledContents(true);
		//���ó�ʼ����ͼƬ
		showGifLabel_->setMovie(movies_[playTimes].get());
		//��layout�������Ҫ�ؼ�
		showLayout_->addWidget(showGifLabel_.get());
	}
	else 
	{
		//���ó�ʼ����ͼƬ
		showGifLabel_->setMovie(movies_[playTimes].get());
	}
	preShowGif = playTimes;
	movies_[playTimes]->start();
	qDebug() << "now show : " << nowShowId_ << " " << playTimes;
	playTimes++;

	//�Ͽ���ʱ��������connect
	timer_.disconnect();

	//���ö�ʱ����ʱʱ��
	timer_.start(GIF_PLAY_TIME);

	//����Gif�Զ���������
	QObject::connect(&timer_, &QTimer::timeout, [&] {
		if (playTimes < movies_.size())
		{
			if(isClickButton)
			{//����Ƿ���������������װ�ʱ��Ϊ3s
				isClickButton = false;
				timer_.setInterval(GIF_PLAY_TIME);
				timer_.start(); 
			}
			//��ͣԭ���Ĳ�����Դ������ȡ��ǰ�Ĳ�����Դ
			showGifLabel_->setMovie(nullptr);
			movies_[preShowGif]->stop();
			preShowGif = playTimes;
			//��ȡ��ǰ�Ĳ�����Դ
			showGifLabel_->setMovie(movies_[playTimes].get());
			movies_[playTimes]->start();
			qDebug() << "now showing : " << nowShowId_ << " " << playTimes;
			for (int buttonCount = 0; buttonCount < buttonList_.size(); ++buttonCount)
			{
				if (playTimes != buttonCount)
				{
					buttonList_[buttonCount]->setChecked(false);
				}
			}
			buttonList_[playTimes]->setChecked(true);
			playTimes++;
		}
		else
		{
			playTimes = 0; 
			showGifLabel_->setMovie(nullptr);
			movies_[preShowGif]->stop();
			preShowGif = playTimes; 
			showGifLabel_->setMovie(movies_[playTimes].get());
			movies_[playTimes]->start();
			qDebug() << "now showing : " << nowShowId_ << " " << playTimes;
			for (int buttonCount = 0; buttonCount < buttonList_.size(); ++buttonCount)
			{
				if (playTimes != buttonCount)
				{
					buttonList_[buttonCount]->setChecked(false);
				}
			}
			buttonList_[playTimes]->setChecked(true);
			playTimes++; 
		}
	}
	);
	

	if (!this->isVisible())
	{
		this->show();
	}
	
}


//���Id�Ͷ�Ӧ����Դ����
void FunctionStudyWidget::addMovies(int id, Data data)
{
	showIds_.push_back(id);
	datas_.insert(id, data);
}

//���ð汾����
void FunctionStudyWidget::setVersion(const versionData& verData)
{
	this->verData_ = verData;
}