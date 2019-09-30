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
MainWindow * mwReference;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
            // Pass KeyDown/KeyUp messages for Qt class to logicize
            case WM_KEYDOWN:
                mwReference->keyDown(PKBDLLHOOKSTRUCT(lParam)->vkCode);
            break;
            case WM_KEYUP:
                mwReference->keyUp(PKBDLLHOOKSTRUCT(lParam)->vkCode);
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}



void MainWindow::keyDown(DWORD key)
{
    if(key == VK_LSHIFT || key == VK_RSHIFT)
    {
        bWinKey = true;
        doMultimedia(key);
    }
    if(key == 0x5a || key == 0x53 || key == 0x44 || key == 0x57 || key == 0x41 || key == 0x51 || key == 0x45)
        doMultimedia(key);

}

void MainWindow::keyUp(DWORD key)
{
    if(key == VK_LSHIFT || key == VK_RSHIFT)
        bWinKey = false;
}

void MainWindow::pressKey(DWORD vkKeyCode)
{
    INPUT Input;
    // Set up a generic keyboard event.
    Input.type = INPUT_KEYBOARD;
    Input.ki.wScan = 0;
    Input.ki.time = 0;
    Input.ki.dwExtraInfo = 0;
    Input.ki.dwFlags = 0;
    Input.ki.wVk = vkKeyCode;
    SendInput(1, &Input, sizeof(INPUT));
}

void MainWindow::doMultimedia(DWORD vkKeyCode)
{

    if(!bWinKey or !dealS) return;
    if (vkKeyCode == 0x5a)
    refre = !refre;

    if(vkKeyCode==0x44)
        conG+=4;
    if(vkKeyCode==0x53)
        conGy+=4;
    if(vkKeyCode==0x41 and conG>4)
        conG-=4;
    if(vkKeyCode==0x57 and conGy>4)
        conGy-=4;
    if(vkKeyCode==0x45)
        conL+=3;
    if(vkKeyCode==0x51 and conL>3)
        conL-=3;
}


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onT()));
    timer->start(50);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QCoreApplication::instance()->installEventFilter(this);
      mwReference = this;
         bWinKey     = false;

         hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
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
            if(keyEvent->key()==Qt::Key_P)
                dealS=!dealS;
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
    //qDebug()<<"hehe"<<QDateTime::currentDateTime();
    delete  imp;
    delete  im;
}
void MainWindow::onT()
{
    repaint();
}


MainWindow::~MainWindow()
{
    UnhookWindowsHookEx(hhkLowLevelKybd);
}
