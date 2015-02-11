#include "utils.h"
#include "settings.h"
#include "mainwindow.h"

#include <QDir>
#include <QString>
#include <QProcess>

QString appendPath(const QString &path1, const QString &path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}
