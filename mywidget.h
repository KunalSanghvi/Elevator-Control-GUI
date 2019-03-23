#ifndef MYWIDGET_H
#define MYWIDGET_H

//change
#define  DOOR_OPEN_INIT_1             0
#define  DOOR_OPEN_1                  1
#define  WAIT_FOR_DOOR_CLOSE_1        2
#define  DOOR_CLOSE_1                 3

#define  DOOR_OPEN_INIT_2             0
#define  DOOR_OPEN_2                  1
#define  WAIT_FOR_DOOR_CLOSE_2        2
#define  DOOR_CLOSE_2                 3

#define  DOOR_OPEN_INIT_3             0
#define  DOOR_OPEN_3                  1
#define  WAIT_FOR_DOOR_CLOSE_3        2
#define  DOOR_CLOSE_3                 3

#define  DOOR_OPEN_INIT_4             0
#define  DOOR_OPEN_4                  1
#define  WAIT_FOR_DOOR_CLOSE_4        2
#define  DOOR_CLOSE_4                 3

#define DOOR_WAIT_AFTER_OPEN        500

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QLabel>
#include <QVector>

#include "Lift.h"
#include "imagesequence.h"
#include "global.h"

class mywidget : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent *);

public:
    mywidget(QWidget *parent = 0);
    void draw_lift(int liftX, int liftY);
    void draw_line(void);
    void floor_label(void);
    void door_state_1(Lift* liftObj, int imageIndex);// change
    void door_state_2(Lift* liftObj, int imageIndex);// change
    void door_state_3(Lift* liftObj, int imageIndex);// change
    void door_state_4(Lift* liftObj, int imageIndex);// change
    void door_execution(int liftX, int liftY, int i); //change
    int myDoorState1; //chhange
    int myDoorState2; //chhange
    int myDoorState3; //chhange
    int myDoorState4; //chhange
    Lift *liftMove1;
    Lift *liftMove2;
    Lift *liftMove3;
    Lift *liftMove4;
    int status1, status2, status3, status4;

    QVector<ImageSequence *> myImage; //Door opening sequence execution //change
    QVector<QImage> doorOpen;

    QImage image;
    QImage imageDoorClose;
    int waitTime1;
    int waitTime2;
    int waitTime3;
    int waitTime4;
};

#endif // MYWIDGET_H
