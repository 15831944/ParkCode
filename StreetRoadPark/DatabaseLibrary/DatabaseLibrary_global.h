#ifndef DATABASELIBRARY_GLOBAL_H
#define DATABASELIBRARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DATABASELIBRARY_LIBRARY)
#  define DATABASELIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATABASELIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DATABASELIBRARY_GLOBAL_H