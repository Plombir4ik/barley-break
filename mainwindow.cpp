#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     this->setWindowTitle("Barley-break");

     db = QSqlDatabase::addDatabase("QMYSQL");
     db.setHostName("remotemysql.com");
     db.setUserName("PzRKtu4vpT");
     db.setPassword("Gx2bmH9TzK");
     db.setDatabaseName("PzRKtu4vpT");
     db.setPort(3306);
     db.open();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));

    Buttons = ui->groupBox->findChildren<QPushButton *>();
    foreach(QPushButton* butt, Buttons)
    {
        connect(butt,&QPushButton::clicked,this,&MainWindow::toMove);
    }
    int startX = 10;
    int startY = 10;
    int count = 1;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            Points.push_back(Point(count,startX,startY,false));
            count++;
            startX += 75;
        }
        startX = 10;
        startY += 75;
    }

    Points[static_cast<int>(Points.size())-1].changeStatus(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(75);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::toMove()
{
    QPushButton* but = qobject_cast<QPushButton*>(sender());
    Point neighbourButton;
    Point emptyPoint;
    for(int i = 0; i < static_cast<int>(Points.size()); i++)
    {
        if(Points[i].getX() == but->x() && Points[i].getY() == but->y())
        {
            neighbourButton = Points[i];
        }
        if(Points[i].getStatus() == true)
        {
            emptyPoint = Points[i];
        }
    }

    if((emptyPoint.getX() - neighbourButton.getX() == 75 && emptyPoint.getY() == neighbourButton.getY() )
        || (emptyPoint.getY() - neighbourButton.getY() == 75 && emptyPoint.getX() == neighbourButton.getX())
        || (emptyPoint.getY() - neighbourButton.getY() == -75 && emptyPoint.getX() == neighbourButton.getX())
        || (emptyPoint.getX() - neighbourButton.getX() == -75 && emptyPoint.getY() == neighbourButton.getY()))
    {

        animation = new QPropertyAnimation(but,"geometry");
        animation->setDuration(175);
        animation->setStartValue(but->geometry());
        animation->setEndValue(QRect(Points[emptyPoint.getId()-1].getX(),Points[emptyPoint.getId()-1].getY(),70,70));
        animation->start();
        Points[emptyPoint.getId()-1].changeStatus(false);
        Points[neighbourButton.getId()-1].changeStatus(true);
        connect(animation, &QVariantAnimation::finished, this, &MainWindow::checkWin);
    }

}

void MainWindow::checkWin()
{
    Point rightPoint;
    bool isWin = true;
    foreach(QPushButton* but, Buttons)
    {
        for(int i = 0; i < static_cast<int>(Points.size()); i++)
        {
            if(Points[i].getX() == but->x() && Points[i].getY() == but->y())
            {
                rightPoint = Points[i];
            }
        }
        if(!(rightPoint.getId() == but->text().toInt()))
        {
            isWin = false;
            break;
        }
    }

    if(isWin == true)
    {
        timer->stop();

        QSqlQuery query = QSqlQuery(db);
        User user(ui->nickName->text(),ui->time->text().toDouble());
        query.exec("SELECT * from barley_break where nickname = '"+ user.getNick() +"'");
        if(query.size() > 0)
        {
            query.first();
            if(query.value("seconds").toDouble() > user.getSeconds())
            {
                QSqlQuery query2 = QSqlQuery(db);
                query2.exec("update barley_break set seconds = '"+ QString::number(user.getSeconds()) +"' where nickname = '"+ user.getNick() +"'");
            }
        }
        else
        {
            QSqlQuery query3;
            query3.prepare("INSERT INTO barley_break (nickname, seconds) VALUES (:name, :seconds)");
            query3.bindValue(":name", user.getNick());
            query3.bindValue(":seconds", user.getSeconds());
            query3.exec();
        }
        ui->stackedWidget->setCurrentIndex(3);
        ui->trophyButton->setEnabled(false);
        ui->reButton->setEnabled(false);
    }
}


void MainWindow::on_reButton_clicked()
{
    ui->time->setText("0");
    gameTime = 0;
    timer->stop();
    ui->groupBox->setEnabled(false);
    ui->reButton->setEnabled(false);
    for(int i = 0; i < 75; i++)
    {
        delay();
        relocateButtons();
    }
    timer->start(100);
    ui->groupBox->setEnabled(true);
    ui->reButton->setEnabled(true);
}

void MainWindow::relocateButtons()
{
    std::vector<QPushButton*>tempButtons;
    Point emptyPoint;

    for(int i = 0; i < static_cast<int>(Points.size()); i++)
    {

        if(Points[i].getStatus() == true)
        {
            emptyPoint = Points[i];
        }
    }
    foreach(QPushButton* butt, Buttons)
    {
        if((emptyPoint.getX() - butt->x() == 75 && emptyPoint.getY() == butt->y())
            || (emptyPoint.getY() - butt->y() == 75 && emptyPoint.getX() == butt->x())
            || (emptyPoint.getY() - butt->y() == -75 && emptyPoint.getX()== butt->x())
            || (emptyPoint.getX() - butt->x() == -75 && emptyPoint.getY() == butt->y())
           )
        {
            tempButtons.push_back(butt);
        }
    }
        srand ( time(NULL) );
        tempButtons[rand() % tempButtons.size()]->clicked();
}

void MainWindow::updateTime()
{
    ui->time->setText(QString::number(gameTime));
    gameTime += 0.1;
}

void MainWindow::on_trophyButton_clicked()
{
    int i = 1;
    QSqlQuery query = QSqlQuery(db);
    query.exec("SELECT * from barley_break order by seconds");
    while(query.next()){
        QLabel* info = new QLabel;
        QLabel* info2 = new QLabel;
        info->setText(query.value("nickname").toString());
        info2->setText(query.value("seconds").toString() + " s");
        info->setStyleSheet("QLabel{font-size: 11pt; color : white; }");
        info2->setStyleSheet("QLabel{font-size: 11pt; color : white; }");
        {
            int j = 1;
            ui->gridLayout->addWidget(info,i,j);
            ui->gridLayout->addWidget(info2,i,j+1);
        }
        i++;
    }

    if(swapPage == true)
    ui->stackedWidget->setCurrentIndex(2);
    else
    ui->stackedWidget->setCurrentIndex(1);
    swapPage = !swapPage;

}

void MainWindow::on_entryButton_clicked()
{
    if(ui->lineEdit->text() != "")
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->nickName->setText(ui->lineEdit->text());
        ui->trophyButton->setEnabled(true);
        ui->reButton->setEnabled(true);
        ui->reButton->clicked();
    }

}

void MainWindow::on_okButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->groupBox->setEnabled(false);
    ui->trophyButton->setEnabled(true);
    ui->reButton->setEnabled(true);
}

void MainWindow::on_lineEdit_returnPressed()
{
    ui->entryButton->clicked();
}
