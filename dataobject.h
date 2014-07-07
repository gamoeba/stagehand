#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QString>
#include <QMatrix4x4>
#include <vector>

class DataObject
{
public:
    DataObject(QString val);
    ~DataObject();

    bool isVector();
    bool isMatrix();
    bool is4x4Matrix();

    std::vector<double> getVector() {return mVector;}
    std::vector<std::vector<double> > getMatrix() { return mMatrix; }
    QMatrix4x4 get4x4Matrix();

    QString toString() {return mValue;}
    double toDouble() {return mValue.toDouble();}

private:
    std::vector<double> parseVector(QString str);
    std::vector<std::vector<double> > parseMatrix(QString str);
    QMatrix4x4 parse4x4Matrix(QString str);

    QString mValue;
    std::vector<double> mVector;
    std::vector<std::vector<double> > mMatrix;
    QMatrix4x4 m4x4Matrix;
};

#endif // DATAOBJECT_H
