#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include<bits/stdc++.h>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QtGui>
#include <QString>
#include <QDateTime>
#include <QString>
#include <synchapi.h>

MainWindow::MainWindow(int flag,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    QString titlestring = QString::fromLocal8Bit("五子棋小游戏                                                                     By ZZY");
    setWindowTitle(titlestring);
    QIcon icon("D:\\wuziqi\\wuziqi\\src\\wzqicon.png");
    setWindowIcon(icon);
    // 设置窗口大小。
    setFixedSize(MARGIN*2 + BLOCK_SIZE * BOARD_GRAD_SIZE,
                 MARGIN*2 + BLOCK_SIZE * BOARD_GRAD_SIZE);
    playbackFlag = flag;
    initGame(); // 初始化函数
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0,0,width(),height(),QPixmap("D:\\wuziqi\\wuziqi\\src\\bgpic.png"));

    for(int i=0;i<BOARD_GRAD_SIZE + 1;i++){
        // 从左到右，第i+1条竖线
        painter.drawLine(MARGIN + BLOCK_SIZE * i,MARGIN,
                         MARGIN + BLOCK_SIZE * i,
                         size().height() - MARGIN);
        painter.drawLine(MARGIN,MARGIN + BLOCK_SIZE * i,
                         size().width()-MARGIN,
                         MARGIN+BLOCK_SIZE * i);
    }

    //绘制选中的点
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    // 绘制落子标记 防止出框越界
    if(clickPosRow>0 && clickPosRow<BOARD_GRAD_SIZE &&
       clickPosCol>0 && clickPosCol<BOARD_GRAD_SIZE &&
        game->gameMapVec[clickPosRow][clickPosCol]==0){

        if(game->playerFlag){
            brush.setColor(Qt::black);
        }else{
            brush.setColor(Qt::white);
        }
        painter.setBrush(brush);
        painter.drawRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE);
    }
    // 绘制棋子
    for(int i=0;i<BOARD_GRAD_SIZE;i++){
        for(int j=0;j<BOARD_GRAD_SIZE;j++){
            if(game->gameMapVec[i][j]==1){
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,CHESS_RADIUS*2,CHESS_RADIUS*2);


            }else if(game->gameMapVec[i][j]==-1){
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,CHESS_RADIUS*2,CHESS_RADIUS*2);
            }

        }
    }

    // 判断输赢 判断合法+落子位置是否有棋子
    if(clickPosCol>0 && clickPosCol<BOARD_GRAD_SIZE &&
           clickPosRow>0 && clickPosRow<BOARD_GRAD_SIZE &&
           (game->gameMapVec[clickPosRow][clickPosCol]==1||game->gameMapVec[clickPosRow][clickPosCol]==-1)){
            //game->gameMapVec[clickPosRow][clickPosCol]==1||game->gameMapVec[clickPosRow][clickPosCol]==-1，防止5个0(空白)误
                if(game->isWin(clickPosRow,clickPosCol) &&
                        (game->gamestatus == PVE_Playing || game->gamestatus == PVP_Playing || game->gamestatus == EVE_Playing)  ){
                    game->gamestatus = WIN;
                    // ------------------------------------------------------------------------
                    QString str;
                    str = "Do you wanna save document?";
                    QMessageBox::StandardButton ques = QMessageBox::question(this,"GoBang ",str);
                    if(ques==QMessageBox::Yes){
                        //保存这次记录

                        if(game_type == PVE)
                            docu->GType = "PVE";
                        else if(game_type == PVP)
                            docu->GType = "PVP";
                        else if(game_type == EVE)
                            docu->GType = "EVE";

                        docu->winner = game->gameMapVec[clickPosRow][clickPosCol]==1?"Black":"White";
                        QDateTime curDateTime=QDateTime::currentDateTime();
                        uint endtime = curDateTime.toTime_t();
                        uint statime = docu->starttime.toTime_t();
                        docu->running_time = endtime - statime;

                        docu->savemodel();
                        QMessageBox::information(this,"GoBang","Save Successed !!!");
                    }

                    // ------------------------------------------------------------------------
                    str = game->gameMapVec[clickPosRow][clickPosCol]==1?"Black":"White";
                    QMessageBox::StandardButton btnValue = QMessageBox::information(this,"GoBang ",str+" Win!!!");
                    if(btnValue == QMessageBox::Ok){
                        game->startGame(game_type);
                        if(game_type == PVE)
                            game->gamestatus = PVE_Playing;
                        else if(game_type == PVP)
                            game->gamestatus = PVP_Playing;
                        else if(game_type == EVE)
                            game->gamestatus = EVE_Playing;

                    }


                }
                else if(game->isDead() &&
                        (game->gamestatus == PVE_Playing || game->gamestatus == PVP_Playing || game->gamestatus == EVE_Playing)  ){
                    game->gamestatus = Dead;
                    QString str;
                    str = " It's A Draw!!!";
                    QMessageBox::StandardButton btnValue = QMessageBox::information(this,"GoBang ",str);
                    if(btnValue == QMessageBox::Ok){
                        game->startGame(game_type);
                        if(game_type == PVE)
                            game->gamestatus = PVE_Playing;
                        else if(game_type == PVP)
                            game->gamestatus = PVP_Playing;
                        else if(game_type == EVE)
                            game->gamestatus = EVE_Playing;
                    }
                }

    }
}

void MainWindow::initGame()
{
    game = new GameModel;
    docu = new Documents_Chess;
    playbackStep = 0;
    // 判断
    if(playbackFlag>0){
        //回放
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.drawPixmap(0,0,width(),height(),QPixmap("D:\\wuziqi\\wuziqi\\src\\bgpic.png"));
        for(int i=0;i<BOARD_GRAD_SIZE + 1;i++){
            // 从左到右，第i+1条竖线
            painter.drawLine(MARGIN + BLOCK_SIZE * i,MARGIN,
                             MARGIN + BLOCK_SIZE * i,
                             size().height() - MARGIN);
            painter.drawLine(MARGIN,MARGIN + BLOCK_SIZE * i,
                             size().width()-MARGIN,
                             MARGIN+BLOCK_SIZE * i);
        }


        readdocs();
        initAIgame();
        update();
    }
    else{
        docu->allstep = 1;
        initAIgame();
        // 进行一个信息的记录
        QDateTime curDateTime=QDateTime::currentDateTime();
        QString st = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
        std::cout << st.toStdString().data() << std::endl;
        docu->starttime =curDateTime;
        docu->Docs.clear();// clear
    }
}

void MainWindow::initAIgame()
{
    //game_type =PVE;
    if(game_type == PVE){
        game_type=PVE;
        game->gamestatus = PVE_Playing;
        docu->GType = "PVE";
    }
    else if(game_type == PVP){
        game_type=PVP;
        game->gamestatus = PVP_Playing;
        docu->GType = "PVP";
    }
    else if(game_type == EVE){
        game_type=EVE;
           game->gamestatus = EVE_Playing;
           docu->GType = "EVE";
    }

    game->startGame(game_type);
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){

    int x = event->x();
    int y = event->y();

    //棋盘边缘不落子
    if(x>=MARGIN+BLOCK_SIZE/2&&
           x<this->width()-MARGIN-BLOCK_SIZE/2&&
           y>=MARGIN+BLOCK_SIZE/2&&
           y<this->height()-MARGIN-BLOCK_SIZE/2){
        //获取最近的左上角的点
        int col = (x-MARGIN)/BLOCK_SIZE;
        int row = (y-MARGIN)/BLOCK_SIZE;

        int leftTopPosX = MARGIN+BLOCK_SIZE*col;
        int leftTopPosY = MARGIN+BLOCK_SIZE*row;

        //算距离
        clickPosRow = -1; //初始化
        clickPosCol = -1;
        int len = 0;  //计算完取整

        selectPos = false;


        //len：到左上角的距离
        len = sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow = row;
            clickPosCol = col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
        //len：右上
        len = sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow = row;
            clickPosCol = col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
        //len：左下
        len = sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow = row+1;
            clickPosCol = col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
        //len：右下
        len = sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow = row+1;
            clickPosCol = col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
    }
    // 更新
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(selectPos == false){
        return;
    }
    else{
        selectPos = false;
    }
    // PVP 是人 然后 人
    // PVE 是人 然后 机器
    // EVE 是机器 然后 机器
    if(game_type == PVP){

        chessOneByPerson();
    }

    else if(game_type == PVE){
        chessOneByPerson();

        //机器下棋
        QTimer::singleShot(AI_THINKING_TIME,this,SLOT(chessOneByAI()));
    }
    else if(game_type==EVE){

        //QTimer::singleShot(AI_THINKING_TIME,this,SLOT(chessOneByAI()));
        chessOneByAI();
    }

}

void MainWindow::chessOneByPerson(){
    if(clickPosRow!=-1 && clickPosCol!=-1 && game->gameMapVec[clickPosRow][clickPosCol]==0){

        if(playbackFlag == 1){
            if(playbackStep < game->pre_PB.size())
            clickPosRow = game->pre_PB[playbackStep].row;
            clickPosCol = game->pre_PB[playbackStep].col;
            playbackStep++;
        }

        game->actionByPerson(clickPosRow,clickPosCol);
        // 空白为0，黑子为1 白子为-1
        // 第几步、黑子、i、j
        if(game->playerFlag == true){
            everyStep es(docu->allstep,1,clickPosRow,clickPosCol);
            docu->Docs.emplace_back(es);
        }else{
            everyStep es(docu->allstep,-1,clickPosRow,clickPosCol);
            docu->Docs.emplace_back(es);
        }
        docu->allstep++;
        update();
    }
}

void MainWindow::chessOneByAI(){
    game->actionByAI(clickPosRow,clickPosCol);
    // 默认机器是白棋

    everyStep es(docu->allstep,-1,clickPosRow,clickPosCol);
    docu->Docs.emplace_back(es);
    docu->allstep++;
    update();
}

void MainWindow::readdocs(){
    // 读TXT文件
    if(false == OpenSeleteFile())//弹出选择文件对话框 如果成功选择文件，主线程myWidget类就有对象存储了文件路径
    {
        return;
    }
    if(FilePath == "")//如果没有选择文件，即文件路径为空
    {
        return;
    }


    QString fileName = FilePath;

    if(!fileName.isEmpty())
    {
        QFile *file = new QFile;
        file->setFileName(fileName);
        bool ok = file->open(QIODevice::ReadOnly);
        if(ok)
        {
            QTextStream in(file);
            // 读到gamemodel 的 vector数组里面
            int steps; in >> steps;// 读入一共多少步
            playbackNum = steps;
            for(int i=1;i<=steps;i++){
               everyStep es;
               in>>es.step>>es.color>>es.row>>es.col;
               // 先保存在一个 vector里面 等加载出界面然后再draw
               //game->gameMapVec[es.row][es.col] = es.color;
               game->pre_PB.emplace_back(es);
            }

            file->close();
            delete file;
        }
        else
        {
            QMessageBox::information(this,"Error","Open File:" + file->errorString());
            return;
        }
    }
}

bool MainWindow::OpenSeleteFile(){
    QString strPath = QFileDialog::getOpenFileName(NULL,QString::fromUtf8("ShowDocs"),"",QObject::tr("txt(*.txt)"));
    if(strPath == "")
    {
      QMessageBox::information(this,QString::fromUtf8("Tips:"),QString::fromUtf8("Choose File Failed,No Path"),"OK");
      return false;//用户点击的取消按钮
    }
    FilePath = strPath;
    return true;
}

void MainWindow::playback(){
    // 回放

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for(auto es:game->pre_PB){
        game->gameMapVec[es.row][es.col] = es.color;
        clickPosCol = es.col;
        clickPosRow = es.row;
        update();
    }
}
