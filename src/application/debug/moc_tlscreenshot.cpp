/****************************************************************************
** Meta object code from reading C++ file 'tlscreenshot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Component/screenshot/tlscreenshot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tlscreenshot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TLScreenshot_t {
    QByteArrayData data[14];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TLScreenshot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TLScreenshot_t qt_meta_stringdata_TLScreenshot = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TLScreenshot"
QT_MOC_LITERAL(1, 13, 11), // "sigBtnPress"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "BtnType"
QT_MOC_LITERAL(4, 34, 7), // "btnType"
QT_MOC_LITERAL(5, 42, 21), // "sigScreenshotFinished"
QT_MOC_LITERAL(6, 64, 5), // "image"
QT_MOC_LITERAL(7, 70, 4), // "name"
QT_MOC_LITERAL(8, 75, 17), // "sigExitScreenshot"
QT_MOC_LITERAL(9, 93, 14), // "sigSaveRequest"
QT_MOC_LITERAL(10, 108, 20), // "sigSaveImageFinished"
QT_MOC_LITERAL(11, 129, 13), // "onGetSavePath"
QT_MOC_LITERAL(12, 143, 8), // "savePath"
QT_MOC_LITERAL(13, 152, 10) // "onBtnClick"

    },
    "TLScreenshot\0sigBtnPress\0\0BtnType\0"
    "btnType\0sigScreenshotFinished\0image\0"
    "name\0sigExitScreenshot\0sigSaveRequest\0"
    "sigSaveImageFinished\0onGetSavePath\0"
    "savePath\0onBtnClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TLScreenshot[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    2,   52,    2, 0x06 /* Public */,
       8,    0,   57,    2, 0x06 /* Public */,
       9,    0,   58,    2, 0x06 /* Public */,
      10,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   60,    2, 0x0a /* Public */,
      13,    0,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QImage, QMetaType::QString,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,

       0        // eod
};

void TLScreenshot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TLScreenshot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigBtnPress((*reinterpret_cast< BtnType(*)>(_a[1]))); break;
        case 1: _t->sigScreenshotFinished((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->sigExitScreenshot(); break;
        case 3: _t->sigSaveRequest(); break;
        case 4: _t->sigSaveImageFinished(); break;
        case 5: _t->onGetSavePath((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onBtnClick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TLScreenshot::*)(BtnType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TLScreenshot::sigBtnPress)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TLScreenshot::*)(const QImage & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TLScreenshot::sigScreenshotFinished)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TLScreenshot::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TLScreenshot::sigExitScreenshot)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TLScreenshot::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TLScreenshot::sigSaveRequest)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TLScreenshot::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TLScreenshot::sigSaveImageFinished)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TLScreenshot::staticMetaObject = { {
    &QGraphicsObject::staticMetaObject,
    qt_meta_stringdata_TLScreenshot.data,
    qt_meta_data_TLScreenshot,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TLScreenshot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TLScreenshot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TLScreenshot.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsObject::qt_metacast(_clname);
}

int TLScreenshot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void TLScreenshot::sigBtnPress(BtnType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TLScreenshot::sigScreenshotFinished(const QImage & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TLScreenshot::sigExitScreenshot()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TLScreenshot::sigSaveRequest()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TLScreenshot::sigSaveImageFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
struct qt_meta_stringdata_TLScreenshotPixmap_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TLScreenshotPixmap_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TLScreenshotPixmap_t qt_meta_stringdata_TLScreenshotPixmap = {
    {
QT_MOC_LITERAL(0, 0, 18) // "TLScreenshotPixmap"

    },
    "TLScreenshotPixmap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TLScreenshotPixmap[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void TLScreenshotPixmap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TLScreenshotPixmap::staticMetaObject = { {
    &QGraphicsObject::staticMetaObject,
    qt_meta_stringdata_TLScreenshotPixmap.data,
    qt_meta_data_TLScreenshotPixmap,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TLScreenshotPixmap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TLScreenshotPixmap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TLScreenshotPixmap.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsObject::qt_metacast(_clname);
}

int TLScreenshotPixmap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
