#ifndef MODBUS_GLOBAL_H
#define MODBUS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MODBUS_LIBRARY)
#  define MODBUSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODBUSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MODBUS_GLOBAL_H
