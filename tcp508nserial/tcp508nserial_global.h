#ifndef TCP508NSERIAL_GLOBAL_H
#define TCP508NSERIAL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TCP508NSERIAL_LIBRARY)
#  define TCP508NSERIAL_EXPORT Q_DECL_EXPORT
#else
#  define TCP508NSERIAL_EXPORT Q_DECL_IMPORT
#endif

#endif // TCP508NSERIAL_GLOBAL_H
