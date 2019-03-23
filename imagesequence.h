//change

#ifndef IMAGESEQUENCE_H
#define IMAGESEQUENCE_H

#include <QVector>
#include <QDebug>

class ImageSequence
{
private:
	int listSize;
public:
    ImageSequence(int tempSize);
    int display_image_forward(void);
	int display_image_backward(void);
    int index;
};

#endif // IMAGESEQUENCE_H