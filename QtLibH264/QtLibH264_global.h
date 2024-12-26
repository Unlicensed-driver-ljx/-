#ifndef QTLIBH264_GLOBAL_H
#define QTLIBH264_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTLIBH264_LIBRARY)
#  define QTLIBH264_EXPORT Q_DECL_EXPORT
#else
#  define QTLIBH264_EXPORT Q_DECL_IMPORT
#endif

#endif // QTLIBH264_GLOBAL_H
