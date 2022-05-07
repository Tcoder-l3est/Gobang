#include "welcome.h"
#include "ui_welcome.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include<bits/stdc++.h>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QtGui>
#include <QString>
#include "choose.h"
#include "showdocs.h"


welcome::welcome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);
    QIcon icon("D:\\wuziqi\\wuziqi\\src\\wzqicon.png");
    setWindowIcon(icon);
    QString titlestring = QString::fromLocal8Bit("五子棋小游戏                                                                     By ZZY");
    setWindowTitle(titlestring);

}

void welcome::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0,0,width(),height(),QPixmap("D:\\wuziqi\\wuziqi\\src\\welbg.png"));
}

welcome::~welcome()
{
    delete ui;
}

void welcome::on_pushButton_2_clicked()
{
    exit(0);
}

void welcome::on_pushButton_clicked()
{
    this->close();
    choose *m = new choose;
    m->show();

//    MainWindow *m = new MainWindow;
//    m->game_type = PVE;
//    m->show();


}
