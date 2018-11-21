#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
//    qDebug() << "get:" + settings.value("request_url").toString();
    QString request_url = settings.value("request_url").toString();
    this->setWindowIcon(QIcon(":/image/posbrowser.png"));

    view = new QWebEngineView(this);
//    connect(view,SIGNAL(sig_iconChanged(QPixmap)),this,SLOT(slt_iconChanged(QPixmap)));
    view->load(QUrl(request_url));
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    view->resize(this->size());
}

void MainWindow::handleMessage(const QString &topic)
{
    QSettings settings;
    QString request_url = settings.value("request_url").toString();
//    qDebug() << topic;
    if(request_url != topic){
//        view = new QWebEngineView(this);
    //    connect(view,SIGNAL(sig_iconChanged(QPixmap)),this,SLOT(slt_iconChanged(QPixmap)));
        view->load(QUrl(topic));
        view->show();
    }
}
