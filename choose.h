#ifndef CHOOSE_H
#define CHOOSE_H

#include <QMainWindow>

namespace Ui {
class choose;
}

class choose : public QMainWindow
{
    Q_OBJECT

public:
    explicit choose(QWidget *parent = 0);
    ~choose();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::choose *ui;
};

#endif // CHOOSE_H
