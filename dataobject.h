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
    int toInt() {return mValue.toInt();}

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
