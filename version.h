#ifndef VERSION_H
#define VERSION_H
#include <QtGlobal>
static const double STAGEHAND_VERSION=0.43;

#if defined Q_OS_LINUX
static const char* versionSuffix = "Linux";
#elif defined Q_OS_DARWIN
static const char* versionSuffix = "Darwin";
#elif defined Q_OS_WINDOWS
static const char* versionSuffix = "Windows";
#else
static const char* versionSuffix = "Unknown";
#endif

#endif // VERSION_H
