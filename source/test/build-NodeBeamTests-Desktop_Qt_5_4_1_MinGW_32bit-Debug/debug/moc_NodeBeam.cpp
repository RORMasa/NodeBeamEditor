/****************************************************************************
** Meta object code from reading C++ file 'NodeBeam.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../NodeBeam.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NodeBeam.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NodeBeam_t {
    QByteArrayData data[9];
    char stringdata[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NodeBeam_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NodeBeam_t qt_meta_stringdata_NodeBeam = {
    {
QT_MOC_LITERAL(0, 0, 8), // "NodeBeam"
QT_MOC_LITERAL(1, 9, 7), // "LUA_log"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 3), // "msg"
QT_MOC_LITERAL(4, 22, 16), // "LUA_getJbeamText"
QT_MOC_LITERAL(5, 39, 16), // "LUA_setJbeamText"
QT_MOC_LITERAL(6, 56, 4), // "text"
QT_MOC_LITERAL(7, 61, 19), // "LUA_HtmlInputDialog"
QT_MOC_LITERAL(8, 81, 8) // "htmlfile"

    },
    "NodeBeam\0LUA_log\0\0msg\0LUA_getJbeamText\0"
    "LUA_setJbeamText\0text\0LUA_HtmlInputDialog\0"
    "htmlfile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NodeBeam[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    0,   37,    2, 0x06 /* Public */,
       5,    1,   38,    2, 0x06 /* Public */,
       7,    1,   41,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void NodeBeam::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NodeBeam *_t = static_cast<NodeBeam *>(_o);
        switch (_id) {
        case 0: _t->LUA_log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->LUA_getJbeamText(); break;
        case 2: _t->LUA_setJbeamText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->LUA_HtmlInputDialog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NodeBeam::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NodeBeam::LUA_log)) {
                *result = 0;
            }
        }
        {
            typedef void (NodeBeam::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NodeBeam::LUA_getJbeamText)) {
                *result = 1;
            }
        }
        {
            typedef void (NodeBeam::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NodeBeam::LUA_setJbeamText)) {
                *result = 2;
            }
        }
        {
            typedef void (NodeBeam::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NodeBeam::LUA_HtmlInputDialog)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject NodeBeam::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NodeBeam.data,
      qt_meta_data_NodeBeam,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NodeBeam::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NodeBeam::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NodeBeam.stringdata))
        return static_cast<void*>(const_cast< NodeBeam*>(this));
    return QObject::qt_metacast(_clname);
}

int NodeBeam::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void NodeBeam::LUA_log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NodeBeam::LUA_getJbeamText()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void NodeBeam::LUA_setJbeamText(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NodeBeam::LUA_HtmlInputDialog(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
