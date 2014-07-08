/****************************************************************************
Copyright (C) 2014 Gamoeba Ltd
All rights reserved

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

****************************************************************************/

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
