#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include<bits/stdc++.h>
#include<QDateTime>
#include<QFile>
#include<QTextStream>
#include<QIODevice>
#include<cstring>
#include<QMessageBox>

using namespace std;
// 人人 人机 机机对战
enum GameType{
    PVP, // 人人
    PVE, //人机
    EVE // 机机对战
};

// 游戏状态
enum GameStatus{
    PVP_Playing,
    PVE_Playing,
    EVE_Playing,
    WIN,
    Dead
};

// 记录每一步的行为 包括落子颜色、行列坐标、是否结束
struct everyStep
{
    int step; // 第几步
    int color;// 这次的落子颜色
    int col;// 列
    int row;// 行
    //int endGame;
    everyStep() {
        step = 0;
        color = 0;
        col = 0;
        row = 0;
    }
    everyStep(int s,int colo,int ro,int co) {
        step = s;
        color = colo;
        col = co;
        row = ro;
        //endGame = eg;
    }
};

// 记录一次对局的信息 分为很多步具体以及剩余部分
class Documents_Chess
{
public:
    vector<everyStep> Docs;// 记录的一个结构体vector
    QDateTime starttime;// 游戏开始的时间
    int running_time; // 游戏进行的时间
    int allstep;// 一共多少步
    QString GType; //游戏类型
    QString winner;//谁赢了
    QString filepath_index;
    QString filepath_this; // 这次记录的文件名称采用 时间.txt 命名 可以唯一标识~
    Documents_Chess(){
        filepath_index = "D:\\wuziqi\\build-wuziqi-Desktop_Qt_5_9_3_MinGW_32bit-Debug\\datas\\Index.TXT";
        filepath_this ="D:\\wuziqi\\build-wuziqi-Desktop_Qt_5_9_3_MinGW_32bit-Debug\\datas\\";

        running_time = 0;
        allstep = 1;
        GType = "";
        winner = "";
    }
    Documents_Chess(QDateTime gt,int rt,int as,QString gtype,QString win){
        starttime= gt;
        running_time = rt;
        allstep = as;
        GType = gtype;
        winner = win;
        // 保存索引的文件
        filepath_index = "D:\\wuziqi\\build-wuziqi-Desktop_Qt_5_9_3_MinGW_32bit-Debug\\datas\\Index.TXT";
        filepath_this  = "datas\\";
    }

    void savemodel(){
        //保存这次记录

        // 首先是Index
        QString fname = starttime.toString("yyyy-MM-dd hh:mm:ss");
        QFile *file = new QFile(filepath_index);//此路径下没有就会自己创建一个
        while(!file->open(QIODevice::ReadWrite | QIODevice::Append));//以读写切追加写入的方式操作文本
        QTextStream txtOutput(file);
        // 先写
        txtOutput << fname<< "      ";
        txtOutput << running_time<< "s           ";
        txtOutput << allstep-1<< "          ";
        txtOutput << GType <<"        ";
        txtOutput << winner<<"\n";
        file->close();

        // 写单个文件
        //fname = "123";
        //2021-12-18 15:55:26  处理一下
        QString ff = "";
        for(auto c:fname){
            if(c >= '0' && c<='9')
                 ff+=c;
        }
        std::cout<<ff.toStdString()<<endl;
        QFile *file1 = new QFile(filepath_this+ ff+".txt");//此路径下没有就会自己创建一个

        if(! file1->open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            std::cout<<"OpenFile Failed!!"<<std::endl;
            return;
        }


        QTextStream txtOutput1(file1);
        // 只写everyStep
        // 先写入 一共多少步
        txtOutput1 <<allstep-1<<"\n";
        for(auto s:Docs){
            txtOutput1 << s.step<< " " << s.color<<" "<<s.row<<" "<<s.col<<"\n";
        }

        file1->close();
        Docs.clear();
    }
};


const int BOARD_GRAD_SIZE = 15;
const int MARGIN = 30; //棋子边缘空隙
const int CHESS_RADIUS = 15; //棋子半徑
const int MARK_SIZE = 6; //落子标记边长
const int BLOCK_SIZE = 40;//格子大小
const int POS_OFFSET =BLOCK_SIZE*0.4; // 鼠标点击的模糊距离上线
const int AI_THINKING_TIME = 700;//ms

class GameModel
{
public:
    GameModel(){}

    // 储存当前游戏棋盘和棋子的情况，空白为0，黑子为1 白子为-1
    vector<vector<int> >gameMapVec;

    // 存储各个点位的评分情况，作为AI下棋的依据
    vector<vector<int> > scoreMapVec;

    // 标识下棋 true 黑棋下 flase 白棋下
    bool playerFlag;

    // 游戏模式: PVP PVE EVE
    GameType gameType;
    GameStatus gamestatus;
    vector<everyStep> pre_PB; // 存将要回放的数据

    void startGame(GameType Type);//开始游戏
    void calculateScore();// 计算评分 为了AI
    void actionByPerson(int row,int col); // 人下棋
    void actionByAI(int &clickRow,int &clickCol);// 机器下棋
    void UpdateGameMap(int row,int col); //每次骡子之后更新棋盘
    bool isWin(int row,int col);
    bool isDead();//和棋
};




#endif // GAMEMODEL_H
