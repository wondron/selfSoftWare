#ifndef FRAMEWORK_GLOBAL_H
#define FRAMEWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEWORK_LIBRARY)
#  define FRAMEWORK_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEWORK_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEWORK_GLOBAL_H
