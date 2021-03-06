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
    this->setWindowTitle("天天热卖智慧收银浏览器");
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
    // default fullscreen
//    ui->tabWidget->setWindowFlags (Qt::Window);
//    ui->tabWidget->showFullScreen();
    ui->tabWidget->showNormal();
//    ui->tabWidget->setWindowFlags(Qt::FramelessWindowHint);//无边框
    // full screen
//    this->setCentralWidget(ui->tabWidget);
//    ui->tabWidget->resize(this->size());
//    ui->tabWidget->setContentsMargins(0,0,0,0);

    connect(view,SIGNAL(titleChanged(QString)),view,SLOT(sltTitleChange(QString))); // 标题改变事件
    connect(view, SIGNAL(urlChanged(QUrl)),this, SLOT(onUrlChanged(QUrl)));  // 浏览器页面一级url change 事件
    connect(view, SIGNAL(loadProgress(int)), this, SLOT(onViewProgress(int)));

    tab_web_view_map.insert(0, view);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabWidgetClose(int))); // tab 关闭事件
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(onCurrentChanged(int))); // tab 改变事件

    // hot key
    QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(this);
    if(shortcut->setShortcut(QKeySequence("Alt+q")))
    {
       connect(shortcut, &QxtGlobalShortcut::activated,
           [=]() {
           if (ui->tabWidget->isFullScreen()){
               ui->tabWidget->setWindowFlags (Qt::SubWindow);
               ui->tabWidget->showNormal ();
           } else {
               ui->tabWidget->setWindowFlags (Qt::Window);
               ui->tabWidget->showFullScreen ();
           }
       });
    } else
    {
        qDebug()<<"alt + Q 快捷键已占用";
    }

//    if(shortcut->setShortcut(QKeySequence("Esc")))
//    {
//        connect(shortcut, &QxtGlobalShortcut::activated,
//            [=]() {
//            if (ui->tabWidget->isFullScreen()){
//                ui->tabWidget->setWindowFlags (Qt::SubWindow);
//                ui->tabWidget->showNormal ();
//            } else {
//                ui->tabWidget->setWindowFlags (Qt::Window);
//                ui->tabWidget->showFullScreen ();
//            }
//        });
//    }

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect clientRect = desktopWidget->availableGeometry();    //用户可用窗口大小
//    QRect applicationRect = desktopWidget->screenGeometry();  //应用程序可用窗口大小
//    qDebug() << clientRect;
//    qDebug() << applicationRect;
//    this->cmaskMake(clientRect);
    // add icon
    //新建QSystemTrayIcon对象
    phpts_TrayIcon = new QSystemTrayIcon(this); //新建托盘要显示的icon
    QIcon icon = QIcon(":/image/posbrowser.png"); //将icon设到QSystemTrayIcon对象中
    phpts_TrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    phpts_TrayIcon->setToolTip(QObject::trUtf8("PosBrowser"));
    //在系统托盘显示此对象
    phpts_TrayIcon->show();

    phpts_TrayRestoreAction = new QAction("打开PosBrowser", this);
    connect(phpts_TrayRestoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    phpts_TrayQuitAction = new QAction(tr("关闭PosBrowser"), this);
    connect(phpts_TrayQuitAction, SIGNAL(triggered()), qApp, SLOT(quit())); //关闭应用，qApp对应的是程序全局唯一指针

    phpts_TrayIconMenu = new QMenu(this);
    phpts_TrayIconMenu->addAction(phpts_TrayRestoreAction);
    phpts_TrayIconMenu->addSeparator();
    phpts_TrayIconMenu->addAction(phpts_TrayQuitAction);
    phpts_TrayIcon->setContextMenu(phpts_TrayIconMenu);

    connect(phpts_TrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(phpts_IconActivated(QSystemTrayIcon::ActivationReason)));

//    QLabel *per1 = new QLabel("Ready1", this);
//    this->statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
//    this->statusBar()->addPermanentWidget(per1); //现实永久信息
//    connect(view->page(),&QWebEnginePage::linkHovered, this, &WebView::linkHovered));
}

void MainWindow::cmaskMake(QRect clientRect)
{
    //! 添加遮罩
    CMaskWidget* maskWidget = CMaskWidget::GetInstance();
    maskWidget->setParent(this);
//    qDebug() << this->geometry();
//    maskWidget->SetMainWidget(this);
//    maskWidget->SetCustomMainWidget(QRect(0, clientRect.height() - 84,  clientRect.width(), 84));
//    int buttonOrTopHeight = clientRect.height(); // on buttom
    int buttonOrTopHeight = 0;
    maskWidget->SetCustomMainWidget(QRect(0, buttonOrTopHeight,  clientRect.width(), 36));
    maskWidget->SetDialogNames(QStringList() << "CBonusForNewUserDlg");

//    QJsonArray browserImages = maskWidget->getPostionImages();

    // match
    this->mathPosition(clientRect.width(), clientRect.height());


    this->setMouseTracking (true); // mouse click
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onViewProgress(int progress)
{
//    qDebug() << progress;

}

void MainWindow::phpts_IconActivated(QSystemTrayIcon::ActivationReason phpts_Reason)
{
    switch (phpts_Reason)
    {
    case QSystemTrayIcon::Trigger:
        //鼠标单击
        this->showNormal();
        //phpts_TrayIcon->showMessage("title","你单击了"); //后面两个默认参数
        break;
    case QSystemTrayIcon::DoubleClick:
        //鼠标双击
        this->showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        //鼠标中键
        this->showNormal();
        break;
    default:
        this->showNormal();
        break;
    }
}

void MainWindow::mathPosition(int rwidth, int rheight)
{
    QJsonArray bottomImages;
    bottomImages.append(":/image/shouye.png");
    bottomImages.append(":/image/houtui.png");
    bottomImages.append(":/image/zuixiao.png");
    bottomImages.append(":/image/go.png");
    bottomImages.append(":/image/shuaxin.png");
    int imageLen = bottomImages.count();
    int catSpace = rwidth * 0.2;
    int leftSpace = rwidth - catSpace;
    int middleSpace = (leftSpace - 26 * imageLen) / imageLen;
    // onbuttom use this
//    for(int i = 0; i < imageLen; i ++)
//    {
//       int mX = catSpace + middleSpace * i;
//       int mXMax = mX + 26;
//       QString smX = QString::number(mX);
//       smX.append("," + QString::number(mXMax));
//       int mY = rheight - 84 + 8; // - black maskwidget + top
//       int mYMax = mY + 26;
//       smX.append("," + QString::number(mY));
//       smX.append("," + QString::number(mYMax));
////         qDebug() << smX;
//       this->positionImages.append(smX);
//    }
    // on top
    for(int i = 0; i < imageLen; i ++)
    {
       int mX = catSpace + middleSpace * i;
       int mXMax = mX + 26;
       QString smX = QString::number(mX);
       smX.append("," + QString::number(mXMax));
       int mY = 0; // - black maskwidget + top
       int mYMax = 36;
       smX.append("," + QString::number(mY));
       smX.append("," + QString::number(mYMax));
//         qDebug() << smX;
       this->positionImages.append(smX);
    }
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{

//    QString str = "(" + QString::number (e->x ()) + ", " + QString::number (e->y ()) + ")";
    int len =  this->positionImages.count();
    int getX = e->x();
    int getY = e->y();
    for(int i=0; i < len; i++)
    {
        QJsonValue str = this->positionImages.at(i);
        QStringList list = str.toString().split(",");
        int minX = list[0].toInt() - 5; // bigger
        int maxX = list[1].toInt() + 5; // bigger
        int minY = list[2].toInt();
        int maxY = list[3].toInt() + 5; // bigger
//        qDebug() << "ttt";
//        qDebug() << minX;
//        qDebug() << maxX;
//        qDebug() << minY;
//        qDebug() << maxY;
//        qDebug() << "end";
//        qDebug() << getX;
//        qDebug() << getY;
        if (getX >= minX && getX <= maxX && getY >= minY && getY <= maxY)
        {
            // match
            if (i == 0)
            {
                   //http://cy.tiantianremai.cn
                QString url =  "http://cy.tiantianremai.cn";
                int tab_index = ui->tabWidget->currentIndex();
            //    qDebug() << "返回";
                view = tab_web_view_map.value(tab_index);
            //    qDebug() << "刷新";
                view->load(url);
            }else if (i == 1)
            {
                this->on_btnBack_clicked();

            }else if (i == 2)
            {
                this->hide();

            }else if (i == 3)
            {
                this->on_btnGo_clicked();
            }else if (i == 4)
            {

                this->on_btnRefresh_clicked();
            }
            break;
        }
    }


//    if(e->button () == Qt::LeftButton)
//    {
//        this->on_btnBack_clicked();
//        qDebug() << str;
//    }
//    else if(e->button () == Qt::RightButton)
//    {



//    }
//    else if(e->button () == Qt::MidButton)
//    {


//    }

}



void MainWindow::onCurrentChanged(int index)
{
//    qDebug() << index;
    QString tab_label = ui->tabWidget->tabText(index);
//    this->setWindowTitle(tab_label);
}

void MainWindow::onLinkHovered(QString url)
{
    qDebug() << "url link change";
    qDebug() << url;
}

void MainWindow::onTabWidgetClose(int index)
{
    ui->tabWidget->removeTab(index);
    // 内存释放
    delete tab_web_view_map.value(index);
    tab_web_view_map.remove(index);
//    qDebug() << "get index:" + index;
}

void MainWindow::onUrlChanged(QUrl url)
{
//     qDebug() << "url change";
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
        // 新增一个view
        view = new WebView(this);

        view->load(QUrl(topic));
        int get_index = ui->tabWidget->addTab(view,topic);
        view->tab_index = get_index;
        view->show();
//        view->showFullScreen();

        connect(view,SIGNAL(titleChanged(QString)),view,SLOT(sltTitleChange(QString))); // 标题改变事件

        tab_web_view_map.insert(get_index, view);
        ui->tabWidget->setCurrentIndex(get_index);
    }
}

void MainWindow::webViewCustomAddTabSlot(WebView* view)
{
    int get_index = ui->tabWidget->addTab(view,new_web_view_url);
    view->tab_index = get_index;
    view->show();
    connect(view,SIGNAL(titleChanged(QString)),view,SLOT(sltTitleChange(QString))); // 标题改变事件
    connect(view, SIGNAL(WebViewUpdateSignal(WebView*)),this,SLOT(webViewCustomAddTabSlot(WebView*)));

    QStringList qList = view->url().query().split('&');

    QList<QString>::Iterator it = qList.begin(),itend = qList.end();
    int match_times = 0;
    for (int i = 0;it != itend; it++,i++){
         QString text = *it;
         QStringList kv = text.split("=");
//         qDebug() << kv;
//         qDebug() << kv[0];
         if((kv[0] == "c" && kv[1] == "site") ||
                 (kv[0] == "a" && kv[1] == "onefive") ||
                 (kv[0] == "op" && kv[1] == "display") ||
                 (kv[0] == "do" && kv[1] == "intelligence_pospage") ||
                 (kv[0] == "m" && kv[1] == "weisrc_dish") ){
            match_times++;
         }
     }

    if(match_times == 5){
        // fullscreen
        ui->tabWidget->setWindowFlags (Qt::Window);
        ui->tabWidget->showFullScreen ();
    }

    tab_web_view_map.insert(get_index, view);
//    qDebug() << get_index + 1;
    ui->tabWidget->setCurrentIndex(get_index);
//    qDebug() << "出发自定义信号";
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

void MainWindow::on_btnRefresh_clicked()
{
    int tab_index = ui->tabWidget->currentIndex();
//    qDebug() << "返回";
    view = tab_web_view_map.value(tab_index);
//    qDebug() << "刷新";
    view->reload();
}

void MainWindow::on_btnBack_clicked()
{
    int tab_index = ui->tabWidget->currentIndex();
//    qDebug() << "返回";
    view = tab_web_view_map.value(tab_index);

    view->back();
}

void MainWindow::on_pushButton_clicked()
{
    this->on_btnBack_clicked();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->on_btnGo_clicked();
}

void MainWindow::on_pushButton_3_clicked()
{
    this->on_btnRefresh_clicked();
}
