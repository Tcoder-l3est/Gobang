#include "showdocs.h"
#include "ui_showdocs.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QtableWidget>
#include <QListWidget>
#include <QDebug>
#include <QVariant>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QModelIndex>
#include <QHeaderView>

ShowDocs::ShowDocs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowDocs)
{
    ui->setupUi(this);
    QIcon icon("D:\\wuziqi\\wuziqi\\src\\wzqicon.png");
    setWindowIcon(icon);
    QString titlestring = QString::fromLocal8Bit("五子棋小游戏                                                                     By ZZY");
    setWindowTitle(titlestring);
}

ShowDocs::~ShowDocs()
{
    delete ui;
}

void ShowDocs::on_pushButton_clicked()
{
    if(false == OpenSeleteFile())//弹出选择文件对话框 如果成功选择文件，主线程myWidget类就有对象存储了文件路径
      {
        return;
      }
      if(FilePath == "")//如果没有选择文件，即文件路径为空
      {
        return;
      }
      ui->textBrowser->setText(FilePath);//如果选中了文件，setText() 或者 insert() 改变其中的文本,那么行编辑器中就会出现选中的文件的路径
      ShowTxtToWindow();
}
void ShowDocs::ShowTxtToWindow()//显示文本文件中的内容
{
    QString fileName = FilePath;

    if(!fileName.isEmpty())
    {
        QFile *file = new QFile;
        file->setFileName(fileName);
        bool ok = file->open(QIODevice::ReadOnly);
        if(ok)
        {
            QTextStream in(file);
            ui->textEdit->setText(in.readAll());
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


bool ShowDocs::OpenSeleteFile()//弹出选择文件对话框
{
    /**
   * @brief strPath
   * 类名::函数名->此时的函数是一个静态函数。例如：getOpenFileName
   * getOpenFileName()函数提供了六个参数：
   *  * 第一个参数的作用是用于指定父组件，基本上构造函数都会提供一个默认的值，即parent = 0;
   *  * 第二个参数的作用是显示的按钮的标题
   *  * 第三个参数的作用是对话框显示时默认打开的目录
   *  * 第四个参数的作用是对话框的后缀名过滤器，过滤掉不需要选择的文件名称
   *  * 第五个参数的作用是默认选择的过滤器
   *  * 第六个参数的作用是对话框的一些参数设定，例如对话框只显示文件夹等信息
   *  ** 注意：还函数有的参数这是可以不需要选择的，例如最后两个参数都是可以省略的，
   *       但是第一个参数和第三个参数当没有特殊要求的时候，将第一个参数置为NULL，
   *       第三个参数置为空字符串。即"";
   */
  QString strPath = QFileDialog::getOpenFileName(NULL,QString::fromUtf8("ShowDocs"),"",QObject::tr("txt(*.txt)"));
  if(strPath == "")
  {
    QMessageBox::information(this,QString::fromUtf8("Tips:"),QString::fromUtf8("Choose File Failed,No Path"),"OK");
    return false;//用户点击的取消按钮
  }
  FilePath = strPath;
  return true;
}
