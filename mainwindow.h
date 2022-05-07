#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gamemodel.h"
#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int flag,QWidget *parent = 0);
    ~MainWindow();
    GameModel* game; //游戏指针
    Documents_Chess *docu;// 存储类型
    GameType game_type; //存储游戏类型
    int playbackFlag; // True 就是回放 光间隔时间涂色就行了
    int playbackNum; // 总共多少次回放
    int playbackStep; // 到第几次了

    void readdocs(); // 选择一个文件
    void playback(); // 回放
    bool OpenSeleteFile();
    void initGame();
    void initAIgame();
private:
    Ui::MainWindow *ui;

    // 绘制
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int clickPosRow,clickPosCol;
    bool selectPos = false;


    // 用来回放的文件
    QString FilePath;


    void chessOneByPerson();

private slots:
    void chessOneByAI();
};

#endif // MAINWINDOW_H
