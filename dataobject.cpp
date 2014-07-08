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

#include "dataobject.h"

#include <QString>
#include <QRegExp>
#include <QDebug>
#include <vector>

DataObject::DataObject(QString val)
{
    mValue = val.trimmed();

    if (isVector()) {
        mVector = parseVector(mValue);
    } else if (is4x4Matrix()) {
        m4x4Matrix = parse4x4Matrix(mValue);
    } else if (isMatrix()) {
        mMatrix = parseMatrix(mValue);
    }
}

DataObject::~DataObject()
{
}

bool DataObject::isVector()
{
    QString result = mValue;
    result.replace(QRegExp("[^\\[]"), "");
    return (result.length() == 1);
}

bool DataObject::isMatrix()
{
    QString result = mValue;
    result.replace(QRegExp("[^\\[]"), "");
    return (result.length() > 1);
}

bool DataObject::is4x4Matrix()
{
    QString r1 = mValue;
    QString r2 = mValue;
    return (r1.replace(QRegExp("[^\\[]"), "").length() == 5) &&
            (r2.replace(QRegExp("[^,]"), "").length() == 15);
}

QMatrix4x4 DataObject::get4x4Matrix()
{
    return m4x4Matrix;
}

std::vector<double> DataObject::parseVector(QString str)
{
   QString sub = str;
   sub.replace(QRegExp("(\\[|\\])"),"");
   QStringList ls = sub.split(",");
   int n = ls.size();
   std::vector<double> vec(n);
   int index = 0;
   QStringList::iterator iter;
   for (iter = ls.begin() ;iter != ls.end() ; iter++) {
       QString out = *iter;
       vec[index++] = out.toDouble();
   }
   return vec;
}

std::vector< std::vector<double> > DataObject::parseMatrix(QString str)
{
    QString sub = str.mid(1,str.length()-2);
    QStringList ls = sub.split(QRegExp("\\] *, *\\["));
    int n = ls.size();
    std::vector< std::vector<double> > vec(n);
    int index = 0;
    QStringList::iterator iter;
    for (iter = ls.begin() ;iter != ls.end() ; iter++) {
        QString out = *iter;
        vec[index++] = parseVector(out);
    }
    return vec;
}

QMatrix4x4 DataObject::parse4x4Matrix(QString str)
{
    QString sub = str.mid(1,str.length()-2);
    QStringList ls = sub.split(QRegExp("\\] *, *\\["));
    QMatrix4x4 matrix;
    int index = 0;
    float* data = matrix.data();
    QStringList::iterator iter;
    for (iter = ls.begin() ;iter != ls.end() ; iter++) {
        QString out = *iter;
        std::vector<double> vd = parseVector(out);
        data[index] = vd[0];
        data[index+1] = vd[1];
        data[index+2] = vd[2];
        data[index+3] = vd[3];
        index+=4;
    }
    return matrix;
}



