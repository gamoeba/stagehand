#include "utils.h"
#include <QDir>
#include <QString>

QString appendPath(const QString &path1, const QString &path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}
