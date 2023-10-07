#include <QApplication>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <memory>

#include "FunctionStudyWidget.h"
#include "mainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow* mainWindow = new MainWindow();

    mainWindow->resize(600, 600);
    mainWindow->setWindowTitle("controller");
    mainWindow->show();

    return a.exec();
}
