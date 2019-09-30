#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <qt_windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    public:
    void keyDown(DWORD key);
       void keyUp(DWORD key);

       static void pressKey(DWORD vkKeyCode);
   private slots:
       void doMultimedia(DWORD vkKeyCode);
   private:
       Ui::MainWindow *ui;
       HHOOK hhkLowLevelKybd;
       bool bWinKey;
       bool dealS=true;

public:
    int keyCo=0;
    int conG=50;
    int conGy=50;
    int conL=12;
    bool refre = false;
    explicit MainWindow(QWidget *parent = nullptr);
    void dd(int v,int i,int j,int nn, QImage*,int,int);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
bool eventFilter(QObject *obj, QEvent *e);
  public slots:
    void onT();


};

#endif // MAINWINDOW_H
