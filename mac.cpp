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
#include "QSettings"







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

            if ( lx -conGx*conL/2+shx<= conGx/2+mar and lx  -conGx*conL/2+shx>= -(conGx/2+mar)
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
                conGx+=4;
            if(keyEvent->key()==83)
                conGy+=4;
            if(keyEvent->key()==65 and conGx>4)
                conGx-=4;
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
            if(keyEvent->key()==Qt::Key_P)
                dealS=!dealS;
        }
    }
    return QWidget::eventFilter( obj, e );
}


void MainWindow::pp(int v,int i,int j, int nn,int sx,int ex,int sy,int ey,int ax,int ay,QImage* im)
{
    int n;
    int tn;
    n = int(v/255.0*(conL/3)*conL);
    tn = 0;
    for(int lx= i*conL+(nn-1)*int(conL/3);lx<i*conL+(nn)*int(conL/3);lx++)
    {
        for (int ly =j*conL;ly<j*conL+conL;ly++)
        {
            tn += 1;
            if(lx+ax>=sx && lx+ax<ex && ly+ay >=sy && ly+ay<ey)
            {
if (ly>500)
    int kk =9;
                if (tn>=n)
                {im->setPixelColor(lx-sx+ax,ly-sy+ay,Qt::black);continue;}
                if(nn==1)
                    im->setPixelColor(lx-sx+ax,ly-sy+ay,Qt::red);
                if(nn==2)
                    im->setPixelColor(lx-sx+ax,ly-sy+ay,Qt::green);
                if(nn==3)
                    im->setPixelColor(lx-sx+ax,ly-sy+ay,Qt::blue);
            }
        }
    }



}

void MainWindow::doOne(int sx1,int sy1,int ex1,int ey1,int ax,int ay,int orix,int oriy,QImage * img,QPainter * painter)
{
    QPixmap * imp1=0;
        QImage *im1=0;


    if (ex1>sx1 && ey1>sy1)
    {
        imp1 = new QPixmap(QSize(ex1-sx1,ey1-sy1));
        im1 = new QImage(imp1->toImage().convertToFormat(QImage::Format_RGB32));
    }

    for(int i=0;i<conGx;i++)
    {
        for (int j=0;j<conGy;j++)
        {
            auto p = img->pixel(i,j);
            auto r=qRed(p);
            auto g=qGreen(p);
            auto b =  qBlue(p);
            pp(r,i,j,1,sx1,ex1,sy1,ey1,ax,ay,im1);
          pp(g,i,j,2,sx1,ex1,sy1,ey1,ax,ay,im1);
        pp(b,i,j,3,sx1,ex1,sy1,ey1,ax,ay,im1);

        }
    }


    if(im1)
    {
        painter->drawImage(QRect(sx1-ax,sy1-ay,ex1-sx1,ey1-sy1),*im1);
        delete  imp1;
        delete  im1;
    }
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QScreen *screen  =  QGuiApplication::primaryScreen();

    auto pos = QCursor::pos();
    auto orix = pos.x();
    auto oriy = pos.y();

      QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
int sw = screenGeometry.width();
int sh = screenGeometry.height();

    auto ax = pos.x()-int(conL*conGx/2);
    auto ay =  pos.y()-int(conL*conGy/2);
    if(refre){
        auto img=screen->grabWindow(0, pos.x()-int(conGx/2), pos.y()-int(conGy/2), conGx, conGy).toImage();

        setGeometry(pos.x()+conGx+3,pos.y()+conGy+3, 10, 10);

        auto im = QPixmap(QSize(10,10)).toImage();
        im.fill(Qt::black);
        painter.drawImage(QRect(0,0,10,10),im);
        return;}
    setAttribute(Qt::WA_TranslucentBackground);


    if (ax<0)
        ax = 0;
    if (ax+ conL*conGx>screenGeometry.width())
        ax = screenGeometry.width()-conL*conGx;
    if (ay<30)
        ay = 30;
    if (ay+ conL*conGy>screenGeometry.height()-70)
        ay = screenGeometry.height()-70-conL*conGy;

    setGeometry(ax,ay, conL*conGx, conL*conGy);

    auto img=screen->grabWindow(0, pos.x()-int(conGx/2), pos.y()-int(conGy/2), conGx, conGy).toImage();


   // int sx1= ax;
   // int sy1 = ay;
   // int ex1 = ax+conGx*conL;
   // int ey1 = oriy-conGy/2;
doOne(ax,ay,ax+conGx*conL,oriy-conGy/2,ax,ay,orix,oriy,&img,&painter);

doOne(ax,oriy+conGy/2,ax+conGx*conL,ay+conGy*conL,ax,ay,orix,oriy,&img,&painter);
doOne(ax,oriy-conGy/2, orix-conGx/2,oriy+conGy/2,ax,ay,orix,oriy,&img,&painter);
doOne(orix+conGx/2,oriy-conGy/2,ax+conGx*conL,oriy+conGy/2,ax,ay,orix,oriy,&img,&painter);
}
void MainWindow::onT()
{
    repaint();
}


MainWindow::~MainWindow()
{

}
