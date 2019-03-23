#include "Lift.h"

Lift::Lift(int x, int y)
{
    xPos = x;
    yPos = y;
}

void Lift::up()
{
    yPos--; 
}

void Lift::down()
{   
    yPos++;
}

void Lift::stop()
{
    doorFlag = 1;
}

void Lift::check_position(void)
{
    for(int i = 0; i < 8; i++)
    {
        if(floorReed[i] == yPos)
        {
            currentPosition = i;
        }
    }
}

int Lift::lift_update()
{
    switch(myState)
    {
        case WAIT:
        {
            if(targetPosition >= 0)
            {
                myState = MOTION_DECIDE;
                status = -2;
            }
            break;
        }

        case MOTION_DECIDE:
        {
            if(targetPosition > currentPosition)
            {
                myState = UP;
            }
            else if(targetPosition < currentPosition)
            {
                myState = DOWN;
            }
            else
            {
                myState = STOP;
                status = -1;
            }
            break;
        }

        case UP:
        {
            up();
            myState = MOTION_DECIDE;
            break;
        }

        case DOWN:
        {
            down();
            myState = MOTION_DECIDE;
            break;
        }

        case STOP:
        {
            stop();
            status = 0;
            myState = DOOR_EXECUTION; //change
            break;
        }

        case DOOR_EXECUTION: //change
        {
            if(doorFlag == 0)
            {
                targetPosition = -1;
                myState = WAIT;
            }
            break;
        }
    }
    return status;
}

int Lift::get_door_flag() //change
{
    return doorFlag;
}

void Lift::set_door_flag(int val) //change
{
    doorFlag = val;
}
