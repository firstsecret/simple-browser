#include "webview.h"
#include <QDebug>
#include "mainwindow.h"
#include <QWebEnginePage>

WebView::WebView(QWidget *parent) : QWebEngineView(parent){
    connect(this->page(),&QWebEnginePage::linkHovered,this,&WebView::linkHovered); // 页面的a link
    connect(this->page(),&QWebEnginePage::titleChanged, this, &WebView::titleChanged); // 页面标题 改变事件
//    qDebug() << "页面槽功能";
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType)
{
//    Q_UNUSED(type)
//    qDebug() << "初始化浏览器";
    if(now_url.isEmpty()){
        return 0;
    }
    WebView* pp = new WebView(this);
    // 新的页面需要 监听
    pp->load(now_url);
    now_url = "";
//    pp->resize(this->size());
//    pp->show();
//    qDebug() << pp->url();
//    this->sendWebViewUpdate();
    // 触
//    qDebug() << "触发新页面";
    emit WebViewUpdateSignal(pp);
//    return pp;
    return 0;
//    return this;
}

void sendWebViewUpdate(WebView*)
{
//    emit WebViewUpdate();
}

void WebView::linkHovered(const QString &url)
{

    if(!url.isEmpty()){
        new_web_view_url = url;
        now_url=url;
    }

//    qDebug() << "link url dddd:";
//    qDebug() << url;
}

void WebView::titleChanged(const QString &title)
{

    new_web_view_title = title;
//    qDebug() << "get page title:" + title;
}

void WebView::sltTitleChange(const QString &title)
{
//    ui->frame;
    tab_widget_obj->setTabText(this->tab_index,title);
//    qDebug() << this->tab_index;
//    QString windowTitle = title;
//    this->setWindowTitle(windowTitle);
    // 将对应的tab title 设置
//    qDebug() << "webview 事件出发";
}
