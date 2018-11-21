#include "webview.h"

WebView::WebView(QWidget *parent) : QWebEngineView(parent){}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type)
    WebView* pp = new WebView(this);
    pp->resize(this->size());
    pp->show();
    return pp;
//    return this;
}
