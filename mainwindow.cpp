#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDebug>
#include <QWebEngineSettings>
#include <QStackedLayout>

//WebView *web_view; // 浏览器当前页面
//QTabWidget *web_tab_views; // 浏览器当前所有的页面
QString new_web_view_url; // 新页面的url
QString new_web_view_title; // 新页面的标题
QTabWidget *tab_widget_obj; // 浏览器的tab容器
QMap<int, WebView*> tab_web_view_map; // tab 与 view 的映射

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
    // 浏览器tab 新增事件
    connect(view, SIGNAL(WebViewUpdateSignal(WebView*)),this,SLOT(webViewCustomAddTabSlot(WebView*)));
    tab_widget_obj = ui->tabWidget;
//    qDebug() << "window朱汉书";
//    view->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
//    connect(view, SIGNAL(urlChanged(QUrl)),
//            this, SLOT(onUrlChanged(QUrl)));
//    QStackedLayout* layout = new QStackedLayout(ui->frame);
//    ui->frame->setLayout(layout);
//    layout->addWidget(view);
    // 删除 默认
    ui->tabWidget->removeTab(0);
    view->load(QUrl(request_url));
    view->show();
    view->tab_index = 0;
//    qDebug() << view->url();
    ui->tabWidget->addTab(view,request_url);
    connect(view,SIGNAL(titleChanged(QString)),view,SLOT(sltTitleChange(QString))); // 标题改变事件
    connect(view, SIGNAL(urlChanged(QUrl)),this, SLOT(onUrlChanged(QUrl)));  // 浏览器页面一级url change 事件

    tab_web_view_map.insert(0, view);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabWidgetClose(int))); // tab 关闭事件
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(onCurrentChanged(int))); // tab 改变事件
//    connect(view->page(),&QWebEnginePage::linkHovered, this, &WebView::linkHovered));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCurrentChanged(int index)
{
//    qDebug() << index;
    QString tab_label = ui->tabWidget->tabText(index);
    this->setWindowTitle(tab_label);
}

void MainWindow::onLinkHovered(QString url)
{
    qDebug() << "url link change";
    qDebug() << url;
}

void MainWindow::onTabWidgetClose(int index)
{
    ui->tabWidget->removeTab(index);
//    qDebug() << "get index:" + index;
}

void MainWindow::onUrlChanged(QUrl url)
{
//    qDebug() << "url change";
//     qDebug() << url;
     QUrl new_web_view_url = url;
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

void MainWindow::webViewCustomAddTabSlot(WebView* view)
{
//    QUrl request_url = *new_web_view_url;
//    qDebug() << new_web_view_url;
//    qDebug() << view->url();
    view->load(QUrl(new_web_view_url));
    int get_index = ui->tabWidget->addTab(view,new_web_view_url);
    view->tab_index = get_index;
    view->show();
//    connect(view, SIGNAL())
    connect(view,SIGNAL(titleChanged(QString)),view,SLOT(sltTitleChange(QString))); // 标题改变事件
    tab_web_view_map.insert(get_index, view);
//    qDebug() << get_index + 1;
    ui->tabWidget->setCurrentIndex(get_index);
//    qDebug() << "出发自定义信号";
//    qDebug() << view;
}

void MainWindow::on_btnGo_clicked()
{
    int tab_index = ui->tabWidget->currentIndex();
//    qDebug() << "前进";
//    WebView* view = ui->tabWidget->currentIndex();
//    qDebug() << view;
    view = tab_web_view_map.value(tab_index);
//    QMapIterator<int, WebView> *tab_web_view_map;

//    while(tab_web_view_map.)
//    if(tab_web_view_map.contains(tab_index)){
//        WebView::iterator it = tab_web_view_map.find(tab_index);
//        view = it.data();
//    }
    view->forward();
}

void MainWindow::on_btnBack_clicked()
{
    int tab_index = ui->tabWidget->currentIndex();
//    qDebug() << "返回";
    view = tab_web_view_map.value(tab_index);

    view->back();
}

void MainWindow::on_btnRefresh_clicked()
{
    int tab_index = ui->tabWidget->currentIndex();
//    qDebug() << "返回";
    view = tab_web_view_map.value(tab_index);
//    qDebug() << "刷新";
     view->reload();
}
