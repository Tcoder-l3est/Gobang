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
    QString titlestring = QString::fromLocal8Bit("������С��Ϸ                                                                     By ZZY");
    setWindowTitle(titlestring);
}

ShowDocs::~ShowDocs()
{
    delete ui;
}

void ShowDocs::on_pushButton_clicked()
{
    if(false == OpenSeleteFile())//����ѡ���ļ��Ի��� ����ɹ�ѡ���ļ������߳�myWidget����ж���洢���ļ�·��
      {
        return;
      }
      if(FilePath == "")//���û��ѡ���ļ������ļ�·��Ϊ��
      {
        return;
      }
      ui->textBrowser->setText(FilePath);//���ѡ�����ļ���setText() ���� insert() �ı����е��ı�,��ô�б༭���оͻ����ѡ�е��ļ���·��
      ShowTxtToWindow();
}
void ShowDocs::ShowTxtToWindow()//��ʾ�ı��ļ��е�����
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


bool ShowDocs::OpenSeleteFile()//����ѡ���ļ��Ի���
{
    /**
   * @brief strPath
   * ����::������->��ʱ�ĺ�����һ����̬���������磺getOpenFileName
   * getOpenFileName()�����ṩ������������
   *  * ��һ������������������ָ��������������Ϲ��캯�������ṩһ��Ĭ�ϵ�ֵ����parent = 0;
   *  * �ڶ�����������������ʾ�İ�ť�ı���
   *  * �����������������ǶԻ�����ʾʱĬ�ϴ򿪵�Ŀ¼
   *  * ���ĸ������������ǶԻ���ĺ�׺�������������˵�����Ҫѡ����ļ�����
   *  * �����������������Ĭ��ѡ��Ĺ�����
   *  * �����������������ǶԻ����һЩ�����趨������Ի���ֻ��ʾ�ļ��е���Ϣ
   *  ** ע�⣺�������еĲ������ǿ��Բ���Ҫѡ��ģ�������������������ǿ���ʡ�Եģ�
   *       ���ǵ�һ�������͵�����������û������Ҫ���ʱ�򣬽���һ��������ΪNULL��
   *       ������������Ϊ���ַ�������"";
   */
  QString strPath = QFileDialog::getOpenFileName(NULL,QString::fromUtf8("ShowDocs"),"",QObject::tr("txt(*.txt)"));
  if(strPath == "")
  {
    QMessageBox::information(this,QString::fromUtf8("Tips:"),QString::fromUtf8("Choose File Failed,No Path"),"OK");
    return false;//�û������ȡ����ť
  }
  FilePath = strPath;
  return true;
}
