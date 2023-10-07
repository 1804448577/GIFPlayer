#include "FunctionStudyWidget.h"


//用于展示图片
int FunctionStudyWidget::playTimes = 0;

//重写paintEvent函数
void FunctionStudyWidget::paintEvent(QPaintEvent* event) 
{
	QPainter painter(this);
	// 开启抗锯齿
	painter.setRenderHint(QPainter::Antialiasing, true);
	// 设置画刷颜色
	painter.setBrush(QBrush(Qt::white)); 
	// 不绘制边框
	painter.setPen(Qt::NoPen);
	// 绘制圆角矩形，圆角的半径为10px
	painter.drawRoundedRect(this->rect(), 10, 10); 
}

//重写show函数
void FunctionStudyWidget::show()
{
	//设置边框透明
	perantWidgetForShow->setWindowFlags(Qt::FramelessWindowHint);
	//设置背景透明
	perantWidgetForShow->setAttribute(Qt::WA_TranslucentBackground);
	//展示
	perantWidgetForShow->show();

	QWidget::show();
}

//重写show函数
void FunctionStudyWidget::close()
{
	perantWidgetForShow->close();
	QWidget::close();
}

//默认构造函数
FunctionStudyWidget::FunctionStudyWidget()
{
	qDebug() << QCoreApplication::applicationDirPath();
	//只初始化必要的Ui界面和成员数据，不载入资源
	perantWidgetForShow.reset(new QWidget());
	this->setParent(perantWidgetForShow.get());
	this->initUi();
}

//传入资源路径的构造函数
FunctionStudyWidget::FunctionStudyWidget(const QString& moviesPath) : QWidget()
{
	qDebug() << QCoreApplication::applicationDirPath();
	//初始化Ui界面并载入资源
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
	//读取对应的Qss文件内容
	QFile file("../src/QssOfFunctionStudy.qss");
	file.open(QFile::ReadOnly);
	QTextStream stream(&file);
	QString styleSheet = stream.readAll();
	this->setStyleSheet(styleSheet);
}
//设置当前展示的Id
void FunctionStudyWidget::updateId(int id)
{
	//查找该Id是否在Ids中存在
	auto ifIdFind = std::find(showIds_.begin(),showIds_.end(),id);
	
	if (ifIdFind == showIds_.end())
	{//若没找到，则输出错误日志，并不做操作
		qDebug() << "not find the id:" << id << "in Ids" << showIds_;
		return;
	}
	
	//否则载入该Id资源
	nowShowId_ = id;
	playTimes = 0;

	this->loadMovies();
	this->showMovies();
	this->loadMoviesButton();
}

//根据按钮展示对应的图片
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
	{//若之前点击过按钮，则不再进行重复点击
		return;
	}

	
	//设置展示图片，并重新演示
	playTimes = buttonSit;
	showGifLabel_->setMovie(movies_[playTimes].get());
	movies_[playTimes]->start();
	//恢复原图片状态
	movies_[preShowGif]->stop();
	preShowGif = playTimes;


	//设置点击状态，设置展示事件为9秒
	isClickButton = true;
	timer_.setInterval(9000);
	timer_.start();
}

//载入资源路径
void FunctionStudyWidget::setMoviesPath(const QString& path)
{
	//设置资源路径
	moviesPath = path;
	//载入该路径的资源
	this->loadMoviesPath(); 
}

//载入资源路径中的资源
void FunctionStudyWidget::loadMoviesPath()
{
	if (!moviesPath.isEmpty())
	{
		//释放原来的资源
		datas_.clear();
		showIds_.clear();

		//设置资源路径
		QDir dir(moviesPath);
		//获取资源路径下所有的文件名称
		QStringList dirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		QMap<int, QStringList> map;
		
		for (const QString& subDirName : dirList) {
			//检测文件名是否全是数字
			bool isDirNameNumber = false;
			//获取绝对路径
			QDir subDir(dir.absoluteFilePath(subDirName));
			//获取绝对路径下的所有gif资源文件
			QStringList gifList = subDir.entryList(QStringList() << "*.gif", QDir::Files);
			//存放对应的路劲数据
			Data pathData;
			//遍历存储对应的路劲数据
			for (const QString& gifName : gifList) {
				pathData.path.append(subDir.absoluteFilePath(gifName));
			}
			//看文件名是否为全数字
			int DirNumber = subDirName.toInt(&isDirNameNumber);
			
			if (isDirNameNumber)
			{//若全是数字，则添加该路径下的所有数据
				this->addMovies(DirNumber, pathData);
			}
			else
			{//否则不做处理，并输出错误日志
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
	//清理图片资源数据
	for each (auto movie in movies_)
	{
		movie.reset();
	}

	//初始化所有成员数据
	movies_.clear();

	// 创建一个QMovie对象数组，每个对象指定一个GIF图片的路径
	for (const auto& gif : datas_[nowShowId_].path) {
		std::shared_ptr<QMovie> movie;
		movie.reset(new QMovie(gif));
		movies_.push_back(movie);
	}
}

//动态载入按钮
void FunctionStudyWidget::loadMoviesButton()
{
	
	qDebug() << "load Movies";
	if (movies_.empty())
	{//若图片资源为空
		qDebug() << "Gifs Is empty!";
		return;
	}

	//清理按钮资源数据
	for each (auto button  in buttonList_)
	{
		buttinLayout_->removeWidget(button.get());
		button->deleteLater();
		button.reset();
	}

	//初始化所有成员数据
	buttonList_.clear();
	isClickButton = false;
	
	if (!buttinLayout_)
	{
		buttinLayout_.reset(new QHBoxLayout());
	}

	//移除buttinLayout_中所有元素
	QLayoutItem* child;
	while ((child = buttinLayout_->takeAt(0)) != 0) {
		if (QWidget* widget = child->widget()) {
			widget->deleteLater();
		}
		delete child;
	}

	if (datas_.isEmpty())
	{//若图片资源为空，则直接返回
		return;
	}

	//从showLayout_中移除buttonLayout_
	showLayout_->removeItem(buttinLayout_.get());

	//在按钮部件左侧添加弹簧
	buttinLayout_->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	//动态添加按钮
	for (int countForList = 0; countForList < datas_[nowShowId_].path.size(); ++countForList)
	{
		std::shared_ptr<QPushButton> button;
		button.reset(new QPushButton(this));
		button->setFixedSize(8, 8);
		button->setCheckable(true); // 设置按钮为可选中状态
		buttonList_.push_back(button);
		buttinLayout_->addWidget(button.get());
		connect(button.get(), &QPushButton::clicked, [=]()mutable
			{
				slotButtonPush(countForList);

			}
		);
	}
	//默认显示第一个按钮为可选中状态;
	buttonList_[0]->setChecked(true);
	//在按钮部件右侧添加弹簧
	buttinLayout_->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
	//将更新后的buttonLayout_添加到showLayout_中
	showLayout_->addLayout(buttinLayout_.get());


}



//初始化所有成员数据
void FunctionStudyWidget::initUi()
{
	this->setObjectName("FunctionStudyWidget");
	//初始化showLayout_ , mainLayout_
	showLayout_.reset(new QVBoxLayout(this));
	mainLayout_.reset(new QVBoxLayout(this));
	//设置QSS和大小
	loadQssFile();
	showLayout_->setContentsMargins(8, 8, 8, 4);
	//设置布局与窗口之间的间隙值
	showLayout_->addStretch(); 
	this->setLayout(showLayout_.get());
}

//初始化展示
void FunctionStudyWidget::showMovies()
{
	if (movies_.isEmpty())
	{//若图片资源为空，则直接返回
		return;
	}
	if (!showGifLabel_)
	{
		showGifLabel_.reset(new QLabel(this));
		showGifLabel_->setObjectName("ShowGifLabel");
		//设置图像展示标准
		showGifLabel_->setFixedSize(256, 256);
		//设置lable自动填充，
		showGifLabel_->setScaledContents(true);
		//设置初始播放图片
		showGifLabel_->setMovie(movies_[playTimes].get());
		//给layout，添加主要控键
		showLayout_->addWidget(showGifLabel_.get());
	}
	else 
	{
		//设置初始播放图片
		showGifLabel_->setMovie(movies_[playTimes].get());
	}
	preShowGif = playTimes;
	movies_[playTimes]->start();
	qDebug() << "now show : " << nowShowId_ << " " << playTimes;
	playTimes++;

	//断开定时器的所有connect
	timer_.disconnect();

	//设置定时器定时时间
	timer_.start(GIF_PLAY_TIME);

	//设置Gif自动滚动播放
	QObject::connect(&timer_, &QTimer::timeout, [&] {
		if (playTimes < movies_.size())
		{
			if(isClickButton)
			{//检测是否点击过，是则重新装填定时器为3s
				isClickButton = false;
				timer_.setInterval(GIF_PLAY_TIME);
				timer_.start(); 
			}
			//暂停原来的播放资源，并获取当前的播放资源
			showGifLabel_->setMovie(nullptr);
			movies_[preShowGif]->stop();
			preShowGif = playTimes;
			//获取当前的播放资源
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


//添加Id和对应的资源数据
void FunctionStudyWidget::addMovies(int id, Data data)
{
	showIds_.push_back(id);
	datas_.insert(id, data);
}

//设置版本数据
void FunctionStudyWidget::setVersion(const versionData& verData)
{
	this->verData_ = verData;
}