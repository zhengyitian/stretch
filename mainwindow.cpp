#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtimer.h"
#include "qdebug.h"
#include "qdatetime.h"
#include "qpixmap.h"
#include "qscreen.h"
#include "QPaintEvent"
#include "qpainter.h"
#include "qcursor.h"
#include "QDesktopWidget"
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onT()));
    timer->start(50);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QCoreApplication::instance()->installEventFilter(this);
}
void MainWindow::dd(int v,int i,int j,int nn, QImage* im,int shx,int shy)
{
    auto n = int(v/255.0*(conL/3)*conL);
    int tn = 0;
    int mar=1;
    for(int lx= i*conL+(nn-1)*int(conL/3);lx<i*conL+nn*int(conL/3);lx++)
    { for (int ly =j*conL;ly<j*conL+conL;ly++)
        {
            tn += 1;

            if ( lx -conG*conL/2+shx<= conG/2+mar and lx  -conG*conL/2+shx>= -(conG/2+mar)
                 and ly -conGy*conL/2+shy<= conGy/2+mar and ly -conGy*conL/2+shy>= -(conGy/2+mar))
            {            continue;}
            if (tn>=n)
            {im->setPixelColor(lx,ly,Qt::black);continue;}

            if(nn==1)
                im->setPixelColor(lx,ly,Qt::red);
            if(nn==2)
                im->setPixelColor(lx,ly,Qt::green);
            if(nn==3)
                im->setPixelColor(lx,ly,Qt::blue);
        }
    }
}
bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type()==QEvent::KeyRelease )
    {
        keyCo++;
        if (keyCo==2)
        {keyCo =0;
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
            if(keyEvent->key()==68)
                conG+=4;
            if(keyEvent->key()==83)
                conGy+=4;
            if(keyEvent->key()==65 and conG>4)
                conG-=4;
            if(keyEvent->key()==87 and conGy>4)
                conGy-=4;
            if(keyEvent->key()==69)
                conL+=3;
            if(keyEvent->key()==81 and conL>3)
                conL-=3;
            if(keyEvent->key()==Qt::Key_Escape)
                QApplication::quit();
            if(keyEvent->key()==Qt::Key_F)
                refre=!refre;
        }
    }
    return QWidget::eventFilter( obj, e );
}
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QScreen *screen  =  QGuiApplication::primaryScreen();

    auto pos = QCursor::pos();
    auto ax = pos.x()-int(conL*conG/2);
    auto ay =  pos.y()-int(conL*conGy/2);
    if(refre){
        auto img=screen->grabWindow(0, pos.x()-int(conG/2), pos.y()-int(conGy/2), conG, conGy).toImage();

        setGeometry(pos.x()+conG+3,pos.y()+conGy+3, 10, 10);

        auto im = QPixmap(QSize(10,10)).toImage();
        im.fill(Qt::black);
        painter.drawImage(QRect(0,0,10,10),im);
        return;}

    auto imp = new QPixmap(QSize(conG*conL,conGy*conL));
    auto im = new QImage(imp->toImage().convertToFormat(QImage::Format_RGB32));

    setAttribute(Qt::WA_TranslucentBackground);
    QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    if (ax<0)
        ax = 0;
    if (ax+ conL*conG>screenGeometry.width())
        ax = screenGeometry.width()-conL*conG;
    if (ay<0)
        ay = 0;
    if (ay+ conL*conGy>screenGeometry.height())
        ay = screenGeometry.height()-conL*conGy;
    auto shx = ax-(pos.x()-int(conL*conG/2));
    auto shy = ay- (pos.y()-int(conL*conGy/2));
    setGeometry(ax,ay, conL*conG, conL*conGy);

    auto img=screen->grabWindow(0, pos.x()-int(conG/2), pos.y()-int(conGy/2), conG, conGy).toImage();


    for(int i=0;i<conG;i++)
    {for (int j=0;j<conGy;j++)
        {
            auto p = img.pixel(i,j);
            auto r=qRed(p);
            auto g=qGreen(p);
            auto b =  qBlue(p);

            dd(r,i,j,1,im,shx,shy);
            dd(g,i,j,2,im,shx,shy);
            dd(b,i,j,3,im,shx,shy);
        }}
    painter.drawImage(QRect(0,0,conG*conL,conGy*conL),*im);
    qDebug()<<"hehe"<<QDateTime::currentDateTime();
    delete  imp;
    delete  im;
}
void MainWindow::onT()
{

    repaint();

    //qDebug()<<"hehe"<<QDateTime::currentDateTime();


}
MainWindow::~MainWindow()
{

}
