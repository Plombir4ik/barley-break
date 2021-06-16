#ifndef POINT_H
#define POINT_H
class Point
{
private:
    int id;
    int x;
    int y;
    bool status;

public:
    Point(){}
    Point(int id, int x, int y, bool status)
    {
        this->id = id;
        this->x = x;
        this->y = y;
        this->status = status;
    }
    int getId()
    {
        return id;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    bool getStatus()
    {
        return status;
    }
    void changeStatus(bool status)
    {
        this->status = status;
    }
};
#endif // POINT_H
