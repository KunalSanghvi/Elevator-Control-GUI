#include "mywidget.h"

mywidget::mywidget(QWidget *parent) :
    QWidget(parent)
{
    liftMove1 = new Lift(90, 610);
    liftMove2 = new Lift(310, 610);
    liftMove3 = new Lift(530, 610);
    liftMove4 = new Lift(750, 610);

    for(int i = 0; i < 20; i++)
    {
        imageDoorClose = QImage ("D:\\QTCodes\\LiftSimulator\\LiftDoor\\Lift_"+QString::number(i));  //image path to display
        doorOpen.append(imageDoorClose); //change
    }

    image = QImage ("D:\\QTCodes\\LiftSimulator\\LiftDoor\\Lift_0");

    for(int i = 0; i < 4; i++) //change
    {
        this->myImage.append(new ImageSequence(doorOpen.size()));
    }

    myDoorState1 = DOOR_OPEN_INIT_1;
    myDoorState2 = DOOR_OPEN_INIT_2;
    myDoorState3 = DOOR_OPEN_INIT_3;
    myDoorState4 = DOOR_OPEN_INIT_4;
}

void mywidget::paintEvent(QPaintEvent *)
{
    draw_line();
    floor_label();

    liftMove1->check_position();
    int tempDoorFlag = liftMove1->get_door_flag();
    if(tempDoorFlag == 0) // change
    {
        status1 = liftMove1->lift_update();
        draw_lift(liftMove1->xPos ,liftMove1->yPos);
    }
    else
    {
        door_state_1(liftMove1, 0);
    }

    liftMove2->check_position();
    tempDoorFlag = liftMove2->get_door_flag();
    if(tempDoorFlag == 0)
    {
        status2 = liftMove2->lift_update();
        draw_lift(liftMove2->xPos ,liftMove2->yPos);
    }
    else 
    {
        door_state_2(liftMove2, 1);
    }

    liftMove3->check_position();
    tempDoorFlag = liftMove3->get_door_flag();
    if(tempDoorFlag == 0)
    {
        status3 = liftMove3->lift_update();
        draw_lift(liftMove3->xPos ,liftMove3->yPos);
    }
    else 
    {
        door_state_3(liftMove3, 2);
    }

    liftMove4->check_position();
    tempDoorFlag = liftMove4->get_door_flag();
    if(tempDoorFlag == 0)
    {
        status4 = liftMove4->lift_update();
        draw_lift(liftMove4->xPos ,liftMove4->yPos);
    }
    else 
    {
        door_state_4(liftMove4, 2);
    }
}

void mywidget::draw_lift(int liftX, int liftY)
{
    QPainter painter(this);
    painter.drawImage(QRect(liftX, liftY, 60, 75), image);
}

void mywidget::draw_line(void)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap)); //make lines thick

    //section for floor number
    painter.drawLine(890, 5, 890, 685);

    //Makng box //(x1,y1,x2,y2)
    painter.drawLine(10, 5, 10, 685); //1v
    painter.drawLine(10, 5, 1050, 5); //1h
    painter.drawLine(1050, 5, 1050, 685); //2v
    painter.drawLine(10, 685, 1050, 685); //2h

    //section for lift
    painter.drawLine(230, 5, 230, 685);
    painter.drawLine(450, 5, 450, 685);
    painter.drawLine(670, 5, 670, 685);

    //section for floor
    for(int i = 0; i < 7; i++)
    {
        painter.drawLine(10, (((i + 1) * 85)+5) , 1050, (((i + 1) * 85)+5));
    }
}

void mywidget::floor_label(void)
{
    QPainter painter(this);

    QFont font = painter.font();  //to change the size of the font
    font.setPointSize(12);
    painter.setFont(font);

    for(int i = 0; i < 8; i++)
    {
        painter.drawText(950, (650 - (i * 85)), "Floor "+ QString::number(i));
    }

    painter.drawText(88, 720,"LIFTA = " +QString::number(liftMove1->currentPosition));
    painter.drawText(308, 720,"LIFTB = " +QString::number(liftMove2->currentPosition));
    painter.drawText(528, 720,"LIFTC = " +QString::number(liftMove3->currentPosition));
    painter.drawText(748, 720,"LIFTD = " +QString::number(liftMove4->currentPosition));
}

void mywidget::door_execution(int liftX, int liftY, int i) // change
{
    QPainter painter(this);
    painter.drawImage(QRect(liftX, liftY, 60,  75), doorOpen.at(i));
}

void mywidget::door_state_1(Lift* liftObj, int imageIndex) //change
{
    switch(myDoorState1)
    {
        case DOOR_OPEN_INIT_1:
        {
            myDoorState1 = DOOR_OPEN_1;       
            break;
        }   

        case DOOR_OPEN_1:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_forward();
            if(temp == 1)
            {
                waitTime1 = myTime;
                myDoorState1 = WAIT_FOR_DOOR_CLOSE_1;
            }
            break;
        }

        case WAIT_FOR_DOOR_CLOSE_1:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            if((myTime - waitTime1) > DOOR_WAIT_AFTER_OPEN)
            {
                myDoorState1 = DOOR_CLOSE_1;
            }
            break;
        }

        case DOOR_CLOSE_1:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_backward();
            if(temp == 1)
            {
                myDoorState1 = DOOR_OPEN_INIT_1;
                liftMove1->set_door_flag(0);
            }
            break;
        }
    }
}

void mywidget::door_state_2(Lift* liftObj, int imageIndex) //change
{
    switch(myDoorState2)
    {
        case DOOR_OPEN_INIT_2:
        {
            myDoorState2 = DOOR_OPEN_2;       
            break;
        }   

        case DOOR_OPEN_2:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_forward();
            if(temp == 1)
            {
                waitTime2 = myTime;
                myDoorState2 = WAIT_FOR_DOOR_CLOSE_2;
            }
            break;
        }

        case WAIT_FOR_DOOR_CLOSE_2:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            if((myTime - waitTime2) > DOOR_WAIT_AFTER_OPEN)
            {
                myDoorState2 = DOOR_CLOSE_2;
            }
            break;
        }

        case DOOR_CLOSE_2:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_backward();
            if(temp == 1)
            {
                myDoorState2 = DOOR_OPEN_INIT_2;
                liftMove2->set_door_flag(0);
            }
            break;
        }
    }
}

void mywidget::door_state_3(Lift* liftObj, int imageIndex) //change
{
    switch(myDoorState3)
    {
        case DOOR_OPEN_INIT_3:
        {
            myDoorState3 = DOOR_OPEN_3;       
            break;
        }   

        case DOOR_OPEN_3:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_forward();
            if(temp == 1)
            {
                waitTime3 = myTime;
                myDoorState3 = WAIT_FOR_DOOR_CLOSE_3;
            }
            break;
        }

        case WAIT_FOR_DOOR_CLOSE_3:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            if((myTime - waitTime3) > DOOR_WAIT_AFTER_OPEN)
            {
                myDoorState3 = DOOR_CLOSE_3;
            }
            break;
        }

        case DOOR_CLOSE_3:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_backward();
            if(temp == 1)
            {
                myDoorState3 = DOOR_OPEN_INIT_3;
                liftMove3->set_door_flag(0);
            }
            break;
        }
    }
}

void mywidget::door_state_4(Lift* liftObj, int imageIndex) //change
{
    switch(myDoorState4)
    {
        case DOOR_OPEN_INIT_4:
        {
            myDoorState4 = DOOR_OPEN_4;       
            break;
        }   

        case DOOR_OPEN_4:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_forward();
            if(temp == 1)
            {
                waitTime4 = myTime;
                myDoorState4 = WAIT_FOR_DOOR_CLOSE_4;
            }
            break;
        }

        case WAIT_FOR_DOOR_CLOSE_4:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            if((myTime - waitTime4) > DOOR_WAIT_AFTER_OPEN)
            {
                myDoorState4 = DOOR_CLOSE_4;
            }
            break;
        }

        case DOOR_CLOSE_4:
        {
            door_execution(liftObj->xPos,liftObj->yPos, this->myImage.at(imageIndex)->index);
            int temp = this->myImage.at(imageIndex)->display_image_backward();
            if(temp == 1)
            {
                myDoorState4 = DOOR_OPEN_INIT_4;
                liftMove4->set_door_flag(0);
            }
            break;
        }
    }
}

