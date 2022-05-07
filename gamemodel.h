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
// ���� �˻� ������ս
enum GameType{
    PVP, // ����
    PVE, //�˻�
    EVE // ������ս
};

// ��Ϸ״̬
enum GameStatus{
    PVP_Playing,
    PVE_Playing,
    EVE_Playing,
    WIN,
    Dead
};

// ��¼ÿһ������Ϊ ����������ɫ���������ꡢ�Ƿ����
struct everyStep
{
    int step; // �ڼ���
    int color;// ��ε�������ɫ
    int col;// ��
    int row;// ��
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

// ��¼һ�ζԾֵ���Ϣ ��Ϊ�ܶಽ�����Լ�ʣ�ಿ��
class Documents_Chess
{
public:
    vector<everyStep> Docs;// ��¼��һ���ṹ��vector
    QDateTime starttime;// ��Ϸ��ʼ��ʱ��
    int running_time; // ��Ϸ���е�ʱ��
    int allstep;// һ�����ٲ�
    QString GType; //��Ϸ����
    QString winner;//˭Ӯ��
    QString filepath_index;
    QString filepath_this; // ��μ�¼���ļ����Ʋ��� ʱ��.txt ���� ����Ψһ��ʶ~
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
        // �����������ļ�
        filepath_index = "D:\\wuziqi\\build-wuziqi-Desktop_Qt_5_9_3_MinGW_32bit-Debug\\datas\\Index.TXT";
        filepath_this  = "datas\\";
    }

    void savemodel(){
        //������μ�¼

        // ������Index
        QString fname = starttime.toString("yyyy-MM-dd hh:mm:ss");
        QFile *file = new QFile(filepath_index);//��·����û�оͻ��Լ�����һ��
        while(!file->open(QIODevice::ReadWrite | QIODevice::Append));//�Զ�д��׷��д��ķ�ʽ�����ı�
        QTextStream txtOutput(file);
        // ��д
        txtOutput << fname<< "      ";
        txtOutput << running_time<< "s           ";
        txtOutput << allstep-1<< "          ";
        txtOutput << GType <<"        ";
        txtOutput << winner<<"\n";
        file->close();

        // д�����ļ�
        //fname = "123";
        //2021-12-18 15:55:26  ����һ��
        QString ff = "";
        for(auto c:fname){
            if(c >= '0' && c<='9')
                 ff+=c;
        }
        std::cout<<ff.toStdString()<<endl;
        QFile *file1 = new QFile(filepath_this+ ff+".txt");//��·����û�оͻ��Լ�����һ��

        if(! file1->open(QIODevice::Append|QIODevice::Text))  //append׷�ӣ����Ḳ��֮ǰ���ļ�
        {
            std::cout<<"OpenFile Failed!!"<<std::endl;
            return;
        }


        QTextStream txtOutput1(file1);
        // ֻдeveryStep
        // ��д�� һ�����ٲ�
        txtOutput1 <<allstep-1<<"\n";
        for(auto s:Docs){
            txtOutput1 << s.step<< " " << s.color<<" "<<s.row<<" "<<s.col<<"\n";
        }

        file1->close();
        Docs.clear();
    }
};


const int BOARD_GRAD_SIZE = 15;
const int MARGIN = 30; //���ӱ�Ե��϶
const int CHESS_RADIUS = 15; //���Ӱ돽
const int MARK_SIZE = 6; //���ӱ�Ǳ߳�
const int BLOCK_SIZE = 40;//���Ӵ�С
const int POS_OFFSET =BLOCK_SIZE*0.4; // �������ģ����������
const int AI_THINKING_TIME = 700;//ms

class GameModel
{
public:
    GameModel(){}

    // ���浱ǰ��Ϸ���̺����ӵ�������հ�Ϊ0������Ϊ1 ����Ϊ-1
    vector<vector<int> >gameMapVec;

    // �洢������λ�������������ΪAI���������
    vector<vector<int> > scoreMapVec;

    // ��ʶ���� true ������ flase ������
    bool playerFlag;

    // ��Ϸģʽ: PVP PVE EVE
    GameType gameType;
    GameStatus gamestatus;
    vector<everyStep> pre_PB; // �潫Ҫ�طŵ�����

    void startGame(GameType Type);//��ʼ��Ϸ
    void calculateScore();// �������� Ϊ��AI
    void actionByPerson(int row,int col); // ������
    void actionByAI(int &clickRow,int &clickCol);// ��������
    void UpdateGameMap(int row,int col); //ÿ������֮���������
    bool isWin(int row,int col);
    bool isDead();//����
};




#endif // GAMEMODEL_H
