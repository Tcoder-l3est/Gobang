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
    QString titlestring = QString::fromLocal8Bit("������С��Ϸ                                                                     By ZZY");
    setWindowTitle(titlestring);
    QIcon icon("D:\\wuziqi\\wuziqi\\src\\wzqicon.png");
    setWindowIcon(icon);
    // ���ô��ڴ�С��
    setFixedSize(MARGIN*2 + BLOCK_SIZE * BOARD_GRAD_SIZE,
                 MARGIN*2 + BLOCK_SIZE * BOARD_GRAD_SIZE);
    playbackFlag = flag;
    initGame(); // ��ʼ������
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
        // �����ң���i+1������
        painter.drawLine(MARGIN + BLOCK_SIZE * i,MARGIN,
                         MARGIN + BLOCK_SIZE * i,
                         size().height() - MARGIN);
        painter.drawLine(MARGIN,MARGIN + BLOCK_SIZE * i,
                         size().width()-MARGIN,
                         MARGIN+BLOCK_SIZE * i);
    }

    //����ѡ�еĵ�
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    // �������ӱ�� ��ֹ����Խ��
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
    // ��������
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

    // �ж���Ӯ �жϺϷ�+����λ���Ƿ�������
    if(clickPosCol>0 && clickPosCol<BOARD_GRAD_SIZE &&
           clickPosRow>0 && clickPosRow<BOARD_GRAD_SIZE &&
           (game->gameMapVec[clickPosRow][clickPosCol]==1||game->gameMapVec[clickPosRow][clickPosCol]==-1)){
            //game->gameMapVec[clickPosRow][clickPosCol]==1||game->gameMapVec[clickPosRow][clickPosCol]==-1����ֹ5��0(�հ�)��
                if(game->isWin(clickPosRow,clickPosCol) &&
                        (game->gamestatus == PVE_Playing || game->gamestatus == PVP_Playing || game->gamestatus == EVE_Playing)  ){
                    game->gamestatus = WIN;
                    // ------------------------------------------------------------------------
                    QString str;
                    str = "Do you wanna save document?";
                    QMessageBox::StandardButton ques = QMessageBox::question(this,"GoBang ",str);
                    if(ques==QMessageBox::Yes){
                        //������μ�¼

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
    // �ж�
    if(playbackFlag>0){
        //�ط�
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.drawPixmap(0,0,width(),height(),QPixmap("D:\\wuziqi\\wuziqi\\src\\bgpic.png"));
        for(int i=0;i<BOARD_GRAD_SIZE + 1;i++){
            // �����ң���i+1������
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
        // ����һ����Ϣ�ļ�¼
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

    //���̱�Ե������
    if(x>=MARGIN+BLOCK_SIZE/2&&
           x<this->width()-MARGIN-BLOCK_SIZE/2&&
           y>=MARGIN+BLOCK_SIZE/2&&
           y<this->height()-MARGIN-BLOCK_SIZE/2){
        //��ȡ��������Ͻǵĵ�
        int col = (x-MARGIN)/BLOCK_SIZE;
        int row = (y-MARGIN)/BLOCK_SIZE;

        int leftTopPosX = MARGIN+BLOCK_SIZE*col;
        int leftTopPosY = MARGIN+BLOCK_SIZE*row;

        //�����
        clickPosRow = -1; //��ʼ��
        clickPosCol = -1;
        int len = 0;  //������ȡ��

        selectPos = false;


        //len�������Ͻǵľ���
        len = sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow = row;
            clickPosCol = col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
        //len������
        len = sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow = row;
            clickPosCol = col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
        //len������
        len = sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow = row+1;
            clickPosCol = col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
        //len������
        len = sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow = row+1;
            clickPosCol = col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos = true;
            }
        }
    }
    // ����
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(selectPos == false){
        return;
    }
    else{
        selectPos = false;
    }
    // PVP ���� Ȼ�� ��
    // PVE ���� Ȼ�� ����
    // EVE �ǻ��� Ȼ�� ����
    if(game_type == PVP){

        chessOneByPerson();
    }

    else if(game_type == PVE){
        chessOneByPerson();

        //��������
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
        // �հ�Ϊ0������Ϊ1 ����Ϊ-1
        // �ڼ��������ӡ�i��j
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
    // Ĭ�ϻ����ǰ���

    everyStep es(docu->allstep,-1,clickPosRow,clickPosCol);
    docu->Docs.emplace_back(es);
    docu->allstep++;
    update();
}

void MainWindow::readdocs(){
    // ��TXT�ļ�
    if(false == OpenSeleteFile())//����ѡ���ļ��Ի��� ����ɹ�ѡ���ļ������߳�myWidget����ж���洢���ļ�·��
    {
        return;
    }
    if(FilePath == "")//���û��ѡ���ļ������ļ�·��Ϊ��
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
            // ����gamemodel �� vector��������
            int steps; in >> steps;// ����һ�����ٲ�
            playbackNum = steps;
            for(int i=1;i<=steps;i++){
               everyStep es;
               in>>es.step>>es.color>>es.row>>es.col;
               // �ȱ�����һ�� vector���� �ȼ��س�����Ȼ����draw
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
      return false;//�û������ȡ����ť
    }
    FilePath = strPath;
    return true;
}

void MainWindow::playback(){
    // �ط�

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
