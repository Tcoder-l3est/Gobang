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
    void on_pushButton_clicked();// 选择文件

private:
    Ui::ShowDocs *ui;
    QString FilePath;//记录文件的路径
    QMenu *menu;    //菜单对象
    bool OpenSeleteFile();//弹出选择文件对话框
    void ShowTxtToWindow();//显示txt文件内容
};

#endif // SHOWDOCS_H
