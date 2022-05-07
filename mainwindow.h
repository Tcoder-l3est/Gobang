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
    GameModel* game; //��Ϸָ��
    Documents_Chess *docu;// �洢����
    GameType game_type; //�洢��Ϸ����
    int playbackFlag; // True ���ǻط� ����ʱ��Ϳɫ������
    int playbackNum; // �ܹ����ٴλط�
    int playbackStep; // ���ڼ�����

    void readdocs(); // ѡ��һ���ļ�
    void playback(); // �ط�
    bool OpenSeleteFile();
    void initGame();
    void initAIgame();
private:
    Ui::MainWindow *ui;

    // ����
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int clickPosRow,clickPosCol;
    bool selectPos = false;


    // �����طŵ��ļ�
    QString FilePath;


    void chessOneByPerson();

private slots:
    void chessOneByAI();
};

#endif // MAINWINDOW_H
