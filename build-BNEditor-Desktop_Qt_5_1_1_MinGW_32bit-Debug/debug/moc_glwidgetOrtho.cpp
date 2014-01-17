/****************************************************************************
** Meta object code from reading C++ file 'glwidgetOrtho.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/glwidgetOrtho.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glwidgetOrtho.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GLWidgetOrtho_t {
    QByteArrayData data[13];
    char stringdata[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GLWidgetOrtho_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GLWidgetOrtho_t qt_meta_stringdata_GLWidgetOrtho = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 15),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 16),
QT_MOC_LITERAL(4, 48, 12),
QT_MOC_LITERAL(5, 61, 9),
QT_MOC_LITERAL(6, 71, 10),
QT_MOC_LITERAL(7, 82, 10),
QT_MOC_LITERAL(8, 93, 13),
QT_MOC_LITERAL(9, 107, 12),
QT_MOC_LITERAL(10, 120, 11),
QT_MOC_LITERAL(11, 132, 12),
QT_MOC_LITERAL(12, 145, 11)
    },
    "GLWidgetOrtho\0NodeBeamUpdated\0\0"
    "SelectionUpdated\0setNBPointer\0NodeBeam*\0"
    "NBPointer1\0setViewTop\0setViewBottom\0"
    "setViewFront\0setViewBack\0setViewRight\0"
    "setViewLeft\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWidgetOrtho[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x05,
       3,    0,   60,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    1,   61,    2, 0x0a,
       7,    0,   64,    2, 0x0a,
       8,    0,   65,    2, 0x0a,
       9,    0,   66,    2, 0x0a,
      10,    0,   67,    2, 0x0a,
      11,    0,   68,    2, 0x0a,
      12,    0,   69,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GLWidgetOrtho::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLWidgetOrtho *_t = static_cast<GLWidgetOrtho *>(_o);
        switch (_id) {
        case 0: _t->NodeBeamUpdated(); break;
        case 1: _t->SelectionUpdated(); break;
        case 2: _t->setNBPointer((*reinterpret_cast< NodeBeam*(*)>(_a[1]))); break;
        case 3: _t->setViewTop(); break;
        case 4: _t->setViewBottom(); break;
        case 5: _t->setViewFront(); break;
        case 6: _t->setViewBack(); break;
        case 7: _t->setViewRight(); break;
        case 8: _t->setViewLeft(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GLWidgetOrtho::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidgetOrtho::NodeBeamUpdated)) {
                *result = 0;
            }
        }
        {
            typedef void (GLWidgetOrtho::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidgetOrtho::SelectionUpdated)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject GLWidgetOrtho::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLWidgetOrtho.data,
      qt_meta_data_GLWidgetOrtho,  qt_static_metacall, 0, 0}
};


const QMetaObject *GLWidgetOrtho::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWidgetOrtho::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLWidgetOrtho.stringdata))
        return static_cast<void*>(const_cast< GLWidgetOrtho*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLWidgetOrtho::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void GLWidgetOrtho::NodeBeamUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void GLWidgetOrtho::SelectionUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
