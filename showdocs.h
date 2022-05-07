#ifndef SHOWDOCS_H
#define SHOWDOCS_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <QTableWidget>

namespace Ui {
class ShowDocs;
}

class ShowDocs : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowDocs(QWidget *parent = 0);
    ~ShowDocs();

private slots:
    void on_pushButton_clicked();// ѡ���ļ�

private:
    Ui::ShowDocs *ui;
    QString FilePath;//��¼�ļ���·��
    QMenu *menu;    //�˵�����
    bool OpenSeleteFile();//����ѡ���ļ��Ի���
    void ShowTxtToWindow();//��ʾtxt�ļ�����
};

#endif // SHOWDOCS_H
