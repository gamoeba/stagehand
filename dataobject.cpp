/*
 * Copyright (c) 2014 Gamoeba Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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



