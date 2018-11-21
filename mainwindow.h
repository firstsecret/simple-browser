#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <webview.h>

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
private:
    Ui::MainWindow *ui;
    WebView* view;
private slots:
    void handleMessage(const QString &topic);
};

#endif // MAINWINDOW_H
