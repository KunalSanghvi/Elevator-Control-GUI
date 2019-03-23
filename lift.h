#ifndef LIFT_H
#define LIFT_H

#define WAIT            0
#define MOTION_DECIDE   1
#define UP              2
#define DOWN            3
#define STOP            4
#define DOOR_EXECUTION  5


#include <QString>
#include <QDebug>
#include <QList>
#include <QObject>


class Lift
{
public:
    Lift(int x, int y);
    int lift_update();
    QList<int> myIntegerList;
    int status;
    int xPos;
    int yPos;
    int currentPosition;
    int targetPosition = -1;
    int myState = WAIT;
    int doorFlag = 0; //change
    int get_door_flag(); //change
    void set_door_flag(int val); //change
    void check_position();
    int floorReed[8] = {610, 525, 440, 355, 270, 185, 100, 15};
private:
    void up();
    void down();
    void stop();
};

#endif // LIFT_H
