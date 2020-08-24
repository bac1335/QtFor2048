/****************************************************************************
** Meta object code from reading C++ file 'tllinguist.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Utility/header/tllinguist.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tllinguist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TLLinguist_t {
    QByteArrayData data[11];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TLLinguist_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TLLinguist_t qt_meta_stringdata_TLLinguist = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TLLinguist"
QT_MOC_LITERAL(1, 11, 8), // "Language"
QT_MOC_LITERAL(2, 20, 5), // "Zh_SM"
QT_MOC_LITERAL(3, 26, 5), // "Zh_TR"
QT_MOC_LITERAL(4, 32, 7), // "English"
QT_MOC_LITERAL(5, 40, 8), // "Japanese"
QT_MOC_LITERAL(6, 49, 6), // "Korean"
QT_MOC_LITERAL(7, 56, 7), // "Russian"
QT_MOC_LITERAL(8, 64, 6), // "Arabic"
QT_MOC_LITERAL(9, 71, 7), // "Espanol"
QT_MOC_LITERAL(10, 79, 5) // "Zh_TW"

    },
    "TLLinguist\0Language\0Zh_SM\0Zh_TR\0English\0"
    "Japanese\0Korean\0Russian\0Arabic\0Espanol\0"
    "Zh_TW"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TLLinguist[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    9,   19,

 // enum data: key, value
       2, uint(TLLinguist::Zh_SM),
       3, uint(TLLinguist::Zh_TR),
       4, uint(TLLinguist::English),
       5, uint(TLLinguist::Japanese),
       6, uint(TLLinguist::Korean),
       7, uint(TLLinguist::Russian),
       8, uint(TLLinguist::Arabic),
       9, uint(TLLinguist::Espanol),
      10, uint(TLLinguist::Zh_TW),

       0        // eod
};

void TLLinguist::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TLLinguist::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TLLinguist.data,
    qt_meta_data_TLLinguist,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TLLinguist::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TLLinguist::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TLLinguist.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TLLinguist::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
