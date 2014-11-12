#ifndef VERSION_H
#define VERSION_H
#include <QtGlobal>
static const double STAGEHAND_VERSION=0.41;

#if defined Q_OS_LINUX
static const char* versionSuffix = "Linux";
#elif defined Q_OS_DARWIN
static const char* QString versionSuffix = "Darwin";
#elif defined Q_OS_WINDOWS
static const char* QString versionFile += "Windows";
#else
static const char* versionFile += "Unknown";
#endif

#endif // VERSION_H
