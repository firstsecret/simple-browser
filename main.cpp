#include "mainwindow.h"
#include <QApplication>
#include <QSharedMemory>
#include <QDebug>
#include <QSettings>
#include <QtSingleApplication>

int main(int argc, char *argv[])
{
    //
    QtSingleApplication a(argc, argv);

    QSettings settings;
//    qDebug() << argv[1];
    QString request_url = "https://cy.tiantianremai.cn/web/index.php?c=index&a=login";

    if (argc >= 2 && (request_url.left(5) == "http:" || request_url.left(6) == "https:")){
        request_url = argv[1];
    }

    settings.setValue("request_url", request_url);

    if (a.isRunning())
    {
        // 传递request_url
        a.sendMessage(request_url);
        return EXIT_SUCCESS;
    }

//    qDebug() << argc;
//    qDebug() << argv[1];

    MainWindow w;
//    w.show();
    a.setActivationWindow(&w);
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)),&w, SLOT(handleMessage(const QString&)));
    w.showMaximized();
    return a.exec();
}
