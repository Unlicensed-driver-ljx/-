/****************************************************************************
** Meta object code from reading C++ file 'tcpworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../camlink/H264code/QTracker_booster_G4040/IO/tcpworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpWorker_t {
    QByteArrayData data[23];
    char stringdata0[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpWorker_t qt_meta_stringdata_TcpWorker = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TcpWorker"
QT_MOC_LITERAL(1, 10, 11), // "resultReady"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 6), // "result"
QT_MOC_LITERAL(4, 30, 7), // "initTcp"
QT_MOC_LITERAL(5, 38, 15), // "slot_newconnect"
QT_MOC_LITERAL(6, 54, 16), // "slot_sendmessage"
QT_MOC_LITERAL(7, 71, 16), // "slot_recvmessage"
QT_MOC_LITERAL(8, 88, 15), // "slot_disconnect"
QT_MOC_LITERAL(9, 104, 25), // "contrastStretchSendMatrix"
QT_MOC_LITERAL(10, 130, 3), // "Mat"
QT_MOC_LITERAL(11, 134, 5), // "mat16"
QT_MOC_LITERAL(12, 140, 4), // "Mat&"
QT_MOC_LITERAL(13, 145, 4), // "mat8"
QT_MOC_LITERAL(14, 150, 11), // "upper_bound"
QT_MOC_LITERAL(15, 162, 11), // "lower_bound"
QT_MOC_LITERAL(16, 174, 13), // "drawDetectBox"
QT_MOC_LITERAL(17, 188, 5), // "frame"
QT_MOC_LITERAL(18, 194, 12), // "drawTrackBox"
QT_MOC_LITERAL(19, 207, 9), // "get_color"
QT_MOC_LITERAL(20, 217, 1), // "c"
QT_MOC_LITERAL(21, 219, 1), // "x"
QT_MOC_LITERAL(22, 221, 3) // "max"

    },
    "TcpWorker\0resultReady\0\0result\0initTcp\0"
    "slot_newconnect\0slot_sendmessage\0"
    "slot_recvmessage\0slot_disconnect\0"
    "contrastStretchSendMatrix\0Mat\0mat16\0"
    "Mat&\0mat8\0upper_bound\0lower_bound\0"
    "drawDetectBox\0frame\0drawTrackBox\0"
    "get_color\0c\0x\0max"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   67,    2, 0x0a /* Public */,
       5,    0,   68,    2, 0x0a /* Public */,
       6,    0,   69,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x0a /* Public */,
       8,    0,   71,    2, 0x0a /* Public */,
       9,    4,   72,    2, 0x0a /* Public */,
      16,    1,   81,    2, 0x0a /* Public */,
      18,    1,   84,    2, 0x0a /* Public */,
      19,    3,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 12, QMetaType::Double, QMetaType::Double,   11,   13,   14,   15,
    QMetaType::Void, 0x80000000 | 10,   17,
    QMetaType::Void, 0x80000000 | 10,   17,
    QMetaType::Float, QMetaType::Int, QMetaType::Int, QMetaType::Int,   20,   21,   22,

       0        // eod
};

void TcpWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->resultReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->initTcp(); break;
        case 2: _t->slot_newconnect(); break;
        case 3: _t->slot_sendmessage(); break;
        case 4: _t->slot_recvmessage(); break;
        case 5: _t->slot_disconnect(); break;
        case 6: _t->contrastStretchSendMatrix((*reinterpret_cast< Mat(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 7: _t->drawDetectBox((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 8: _t->drawTrackBox((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 9: { float _r = _t->get_color((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpWorker::resultReady)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TcpWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TcpWorker.data,
    qt_meta_data_TcpWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TcpWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void TcpWorker::resultReady(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
