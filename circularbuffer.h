#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H


class CircularBuffer
{
public:
    CircularBuffer(int size);
    ~CircularBuffer();

    double average();
    void addValue(double val);
    double maxValue();
private:
    double* mValues;
    int mCurrentPointer;
    int mSize;
    int mFilledSize;
};

#endif // CIRCULARBUFFER_H
