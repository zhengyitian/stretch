#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

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
