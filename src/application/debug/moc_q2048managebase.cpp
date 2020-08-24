/****************************************************************************
** Meta object code from reading C++ file 'q2048managebase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../manager/q2048managebase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'q2048managebase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Q2048ManagerBase_t {
    QByteArrayData data[7];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Q2048ManagerBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Q2048ManagerBase_t qt_meta_stringdata_Q2048ManagerBase = {
    {
QT_MOC_LITERAL(0, 0, 16), // "Q2048ManagerBase"
QT_MOC_LITERAL(1, 17, 11), // "sigSendData"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 22), // "QVector<QVector<int> >"
QT_MOC_LITERAL(4, 53, 6), // "isOver"
QT_MOC_LITERAL(5, 60, 10), // "onKeyPress"
QT_MOC_LITERAL(6, 71, 3) // "key"

    },
    "Q2048ManagerBase\0sigSendData\0\0"
    "QVector<QVector<int> >\0isOver\0onKeyPress\0"
    "key"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Q2048ManagerBase[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       1,    1,   34,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    2,    4,
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void Q2048ManagerBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Q2048ManagerBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSendData((*reinterpret_cast< QVector<QVector<int> >(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->sigSendData((*reinterpret_cast< QVector<QVector<int> >(*)>(_a[1]))); break;
        case 2: _t->onKeyPress((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector<int> > >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector<int> > >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Q2048ManagerBase::*)(QVector<QVector<int>> , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Q2048ManagerBase::sigSendData)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Q2048ManagerBase::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Q2048ManagerBase.data,
    qt_meta_data_Q2048ManagerBase,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Q2048ManagerBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Q2048ManagerBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Q2048ManagerBase.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Q2048ManagerBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Q2048ManagerBase::sigSendData(QVector<QVector<int>> _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
