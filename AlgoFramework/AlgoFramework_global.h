#ifndef ALGOFRAMEWORK_GLOBAL_H
#define ALGOFRAMEWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ALGOFRAMEWORK_LIBRARY)
#  define ALGOFRAMEWORK_EXPORT Q_DECL_EXPORT
#else
#  define ALGOFRAMEWORK_EXPORT Q_DECL_IMPORT
#endif

#endif // ALGOFRAMEWORK_GLOBAL_H
