#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>

namespace Ui {
class welcome;
}

class welcome : public QMainWindow
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = 0);
    ~welcome();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::welcome *ui;
};

#endif // WELCOME_H
