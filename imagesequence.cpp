//change

#include "imagesequence.h"

ImageSequence::ImageSequence(int tempSize)
{
	this->listSize = tempSize;
    this->index = 0;
}

int ImageSequence::display_image_forward(void)
{
    int ret = 0;
    this->index++;
    // qDebug() << "index = " << index;
    if(this->index == this->listSize)
    {
        this->index = (this->listSize - 1);
        ret = 1;
    }
    return ret;
}

int ImageSequence::display_image_backward(void)
{
	int ret = 0;
	this->index--;
    if(this->index <= 0)
    {
        this->index = 0;
        ret = 1;
    }
    return ret;
}