#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "trem.h"
#include "track.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void updateInterface(int,int,int);

private slots:
    // void on_pushButton_clicked();
    // void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    //Cria os objetos TREM's
    std::vector<Trem*> trems;
};

#endif // MAINWINDOW_H
