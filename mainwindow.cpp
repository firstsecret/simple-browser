#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDebug>
#include <QWebEngineSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
//    qDebug() << "get:" + settings.value("request_url").toString();
    QString request_url = settings.value("request_url").toString();
    this->setWindowIcon(QIcon(":/image/posbrowser.png"));

//    view = new QWebEngineView(this);
    view = new WebView(this);

    view->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
//    connect(view, SIGNAL(urlChanged(QUrl)),
//            this, SLOT(onUrlChanged(QUrl)));
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
//        connect(view,SIGNAL(urlChanged(QPixmap)),this,SLOT(slt_iconChanged(QPixmap)));
        view->load(QUrl(topic));
        view->show();
    }
}
