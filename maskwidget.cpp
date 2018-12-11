#include "maskwidget.h"
#include <QMutex>
#include <QEvent>
#include <QPainter>
#include <QApplication>

CMaskWidget* CMaskWidget::m_pMask = nullptr;


CMaskWidget* CMaskWidget::GetInstance()
{
    static QMutex s_Mutex;
    if (nullptr == m_pMask)
    {
        QMutexLocker locker(&s_Mutex);
        if (nullptr == m_pMask)
        {
            m_pMask = new CMaskWidget;
            m_pMask->setAttribute(Qt::WA_DeleteOnClose);
        }
    }

    return m_pMask;
}

void CMaskWidget::UnInstance()
{
    static QMutex s_Mutex;
    if (nullptr != m_pMask)
    {
        QMutexLocker locker(&s_Mutex);
        if (nullptr != m_pMask)
        {
            delete m_pMask;
            m_pMask = nullptr;
        }
    }
}

void CMaskWidget::SetMainWidget(QWidget *pWidgetMasked)
{
    if (m_pDlgToBeMasked != pWidgetMasked)
    {
        m_pDlgToBeMasked = pWidgetMasked;
        setGeometry(m_pDlgToBeMasked->geometry());
    }
}

void CMaskWidget::SetCustomMainWidget(QRect deskRect)
{
    setGeometry(deskRect);
}

void CMaskWidget::SetDialogNames(const QStringList &lstWidgetNames)
{
    if (m_lstNamesForDlgPrompt != lstWidgetNames)
    {
        m_lstNamesForDlgPrompt = lstWidgetNames;
    }
}

void CMaskWidget::SetOpacity(float fOpacity)
{
    setWindowOpacity(fOpacity);
}

void CMaskWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(QColor(0, 0, 0, 255)));
    painter.setPen(Qt::transparent);

    QRect rect = this->rect();

    rect.setWidth(rect.width());
    rect.setHeight(rect.height());
//    qDebug() << "maskwidget width";
//    qDebug() << rect.width();
    QPainterPath painterPath;
    painterPath.addRoundedRect(rect, 0, 0);
    painter.drawPath(painterPath);

    QJsonArray bottomImages;
    bottomImages.append(":/image/shouye.png");
    bottomImages.append(":/image/houtui.png");
    bottomImages.append(":/image/zuixiao.png");
    bottomImages.append(":/image/go.png");
    bottomImages.append(":/image/shuaxin.png");
    int imageLen = bottomImages.count();
    int catSpace = rect.width() * 0.2;
    int leftSpace = rect.width() - catSpace;
    int middleSpace = (leftSpace - 26 * imageLen) / imageLen;

    for(int i =0; i < imageLen; i ++)
    {
        // image
        int mX = catSpace + middleSpace * i;
        QRect target(mX, 5.0, 26.0, 26.0); //建立目标矩形，该区域是显示图像的目的地

        QRect source(0.0, 0.0, 26.0, 26.0); //建立源矩形，用来划定来自外部的源图像文件中需要显示的区域
        QJsonValue uri = bottomImages.at(i);
        QImage image(uri.toString()); //建立QImage类对象
        painter.drawImage(target, image, source);
    }
    // image
//    QRect target(20.0,5.0, 26.0, 26.0); //建立目标矩形，该区域是显示图像的目的地
//    QRect source(0.0, 0.0, 26.0, 26.0); //建立源矩形，用来划定来自外部的源图像文件中需要显示的区域
//    QImage image(":/image/shouye.png"); //建立QImage类对象
//    painter.drawImage(target, image, source);
    QWidget::paintEvent(event);
}

void CMaskWidget::showEvent(QShowEvent* event)
{
    if (nullptr != m_pDlgToBeMasked)
    {
        //! 获取被遮盖的窗口的宽高
        QRect rectDlgMasked = m_pDlgToBeMasked->geometry();
        int nWidth = rectDlgMasked.width();
        int nHeight = rectDlgMasked.height();
        setGeometry(0, 0, nWidth, nHeight);
    }
    QWidget::showEvent(event);
}

bool CMaskWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (QEvent::Show == event->type())
    {
        if (m_lstNamesForDlgPrompt.contains(obj->objectName()))
        {
            this->setVisible(true);
        }
    }
    else if (QEvent::Hide == event->type())
    {
        if (m_lstNamesForDlgPrompt.contains(obj->objectName()))
        {
            this->setVisible(false);
        }
    }
    return QWidget::eventFilter(obj, event);
}

CMaskWidget::CMaskWidget(QWidget* wgt /* = nullptr */)
{
    setWindowFlags(windowFlags()
        | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

CMaskWidget::~CMaskWidget()
{
    m_lstNamesForDlgPrompt.clear();
}
