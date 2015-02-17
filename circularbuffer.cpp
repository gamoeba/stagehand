#include "circularbuffer.h"
#include <float.h>

CircularBuffer::CircularBuffer(int size)
    : mCurrentPointer(0),
      mSize(size),
      mFilledSize(0)
{
    mValues = new double[mSize];
}

CircularBuffer::~CircularBuffer()
{
    delete [] mValues;
}

double CircularBuffer::average()
{
    if (mFilledSize ==0){
        return 0;
    }
    double sum = 0.0;
    for (int i=0;i<mFilledSize;++i) {
        sum += mValues[i];
    }
    return sum /= (double)mFilledSize;
}

double CircularBuffer::maxValue()
{
    double max = -DBL_MAX;
    for (int i=0;i<mFilledSize;++i) {
        if (mValues[i] > max) {
            max = mValues[i];
        }
    }
    return max;
}

void CircularBuffer::addValue(double val)
{
    mValues[mCurrentPointer] = val;
    mCurrentPointer += 1;
    if (mCurrentPointer > mFilledSize) {
        mFilledSize = mCurrentPointer;
    }
    if (mCurrentPointer == mSize)
        mCurrentPointer = 0;
}

