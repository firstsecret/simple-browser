#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>

class WebView: public QWebEngineView{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = nullptr);
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType);
};

#endif // WEBVIEW_H
