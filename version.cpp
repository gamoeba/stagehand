#include "version.h"

const double STAGEHAND_VERSION=0.54;

#if defined Q_OS_LINUX
const char* versionSuffix = "Linux";
#elif defined Q_OS_DARWIN
const char* versionSuffix = "Darwin";
#elif defined Q_OS_WINDOWS
const char* versionSuffix = "Windows";
#else
const char* versionSuffix = "Unknown";
#endif
