/****************************************************************************
** Meta object code from reading C++ file 'tllanguage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Component/language/tllanguage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tllanguage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TLLanguage_t {
    QByteArrayData data[5];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TLLanguage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TLLanguage_t qt_meta_stringdata_TLLanguage = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TLLanguage"
QT_MOC_LITERAL(1, 11, 13), // "Language_Type"
QT_MOC_LITERAL(2, 25, 5), // "Zh_SM"
QT_MOC_LITERAL(3, 31, 5), // "Zh_TR"
QT_MOC_LITERAL(4, 37, 7) // "English"

    },
    "TLLanguage\0Language_Type\0Zh_SM\0Zh_TR\0"
    "English"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TLLanguage[] = {

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
       1,    1, 0x0,    3,   19,

 // enum data: key, value
       2, uint(TLLanguage::Zh_SM),
       3, uint(TLLanguage::Zh_TR),
       4, uint(TLLanguage::English),

       0        // eod
};

void TLLanguage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TLLanguage::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TLLanguage.data,
    qt_meta_data_TLLanguage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TLLanguage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TLLanguage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TLLanguage.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TLLanguage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
