#ifndef USER_H
#define USER_H
#include<QString>
class User
{
private:
    QString nickname;
    double seconds;
public:
    User(QString nickname,double seconds)
    {
        this->nickname = nickname;
        this->seconds = seconds;
    }
    QString getNick()
    {
        return nickname;
    }
    double getSeconds()
    {
        return seconds;
    }
};
#endif // USER_H
