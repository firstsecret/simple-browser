#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>

class WebView: public QWebEngineView{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = nullptr);
    int tab_index;
public slots:
    void linkHovered(const QString &url);
    void titleChanged(const QString &title);
    void sltTitleChange(const QString &title);
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType);
    void sendWebViewUpdate(WebView*);
signals:
    void explains();
    void WebViewUpdateSignal(WebView*); // 添加新的tab信号
};

#endif // WEBVIEW_H
