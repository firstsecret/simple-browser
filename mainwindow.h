#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <webview.h>
#include <QFrame>
#include <QLabel>
#include <QTabWidget>
#include <QMap>
#include "qxtglobalshortcut.h"
#include "maskwidget.h"
#include <QDesktopWidget>
#include <QMouseEvent>

//extern WebView *web_view;
//extern QTabWidget *web_tab_views;
extern QString new_web_view_url; //  新页面的url
extern QString new_web_view_title; //  新页面的标题
extern QTabWidget *tab_widget_obj;
//typedef QMap<QString, Employee> EmployeeMap; //自定义一个map类型，值为EmployeeMap对象
//    EmployeeMap tab_web_view_map;
extern QMap<int, WebView*> tab_web_view_map;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *e);
private:
    Ui::MainWindow *ui;
    WebView* view;
private slots:
    void handleMessage(const QString &topic); // 外部url改变事件
    void on_btnGo_clicked();  // 浏览器 前进事件
    void on_btnRefresh_clicked();  // 浏览器 刷新事件
    void onUrlChanged(QUrl); // 一级url改变事件
    void onLinkHovered(QString); // 一级url 点击事件 (无效)
    void onTabWidgetClose(int); // tab 关闭事件
    void onCurrentChanged(int); // tab change事件

    void on_btnBack_clicked(); // 浏览器 返回事件

public slots:
    void webViewCustomAddTabSlot(WebView*);
};

#endif // MAINWINDOW_H
