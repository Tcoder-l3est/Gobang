#include "choose.h"
#include "ui_choose.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include<bits/stdc++.h>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QtGui>
#include <QString>
#include "gamemodel.h"
#include "showdocs.h"


choose::choose(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::choose)
{
    ui->setupUi(this);
    QIcon icon("D:\\wuziqi\\wuziqi\\src\\wzqicon.png");
    setWindowIcon(icon);
    QString titlestring = QString::fromLocal8Bit("五子棋小游戏                                                                     By ZZY");
    setWindowTitle(titlestring);
}

void choose::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0,0,width(),height(),QPixmap("D:\\wuziqi\\wuziqi\\src\\csbg.png"));
}

choose::~choose()
{
    delete ui;
}

void choose::on_pushButton_3_clicked()
{
    //this->close();
    MainWindow *m = new MainWindow(0);
    m->game_type = EVE;
    m->playbackFlag = 0;
    m->show();
}
void choose::on_pushButton_2_clicked()
{// 人机
    MainWindow *m = new MainWindow(0);
    m->game_type = PVE;
    m->playbackFlag = 0;
    m->show();
}


void choose::on_pushButton_clicked()
{
    MainWindow *m = new MainWindow(0);
    m->game_type = PVP;
    m->playbackFlag = 0;
    m->show();
}

void choose::on_pushButton_4_clicked()
{// 转到读文件的界面
    ShowDocs *ss = new ShowDocs;

    ss->show();
}

void choose::on_pushButton_5_clicked()
{
    MainWindow *m = new MainWindow(1);
    m->playbackFlag = 1; // 回放
    m->game_type = PVP;
    m->show();
    //m->readdocs();// 读文件
    //m->playback();// 回放
}
