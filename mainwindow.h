#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<Point.h>
#include<User.h>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QPushButton>
#include <vector>
#include <QTimer>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toMove();
    void checkWin();
    void relocateButtons();
    void on_reButton_clicked();
    void updateTime();
    void on_trophyButton_clicked();
    void on_entryButton_clicked();
    void on_okButton_clicked();
    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QPropertyAnimation* animation;
    QTimer* timer;
    double gameTime = 0;
    bool swapPage = true;
    QSqlDatabase db;
    std::vector<Point>Points;
    QList<QPushButton*>Buttons;

};
#endif // MAINWINDOW_H
