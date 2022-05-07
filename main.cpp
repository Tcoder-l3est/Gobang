#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "welcome.h"
#include "showdocs.h"
#include <QObject>
#include <QTextCodec>

void SetCodec(QApplication &app)
{
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");//���ñ����ʽΪUTF-8
   // QTextCodec::setCodecForTr(codec);//����QT�ַ���ʹ��Ӣ���ַ����ã�������������������ô���tr����ʱ��Ĭ���ִ�����
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
   // QTextCodec::setCodecForLocale(codec);//���������Ҫ�������úͶԱ����ļ�ϵͳ��дʱ���Ĭ�ϱ����ʽ��
    //QTextCodec::setCodecForCStrings(codec);//���������Ҫ�������ַ���������QByteArray����QString����ʱʹ�õ�Ĭ�ϱ��뷽ʽ

}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //SetCodec(a);
    welcome w1;
    w1.show();
    //MainWindow w;
    //QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    //w.setWindowTitle("������С��Ϸ")
    //w.show();

//    ShowDocs ss;
//    ss.show();

    return a.exec();
}
