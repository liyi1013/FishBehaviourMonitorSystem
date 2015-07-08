/****************************************************************************
** Meta object code from reading C++ file 'ImgProcessSet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ImgProcessSet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImgProcessSet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ImgProcessSet_t {
    QByteArrayData data[10];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImgProcessSet_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImgProcessSet_t qt_meta_stringdata_ImgProcessSet = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ImgProcessSet"
QT_MOC_LITERAL(1, 14, 12), // "set_num_fish"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 1), // "n"
QT_MOC_LITERAL(4, 30, 21), // "set_segment_threshold"
QT_MOC_LITERAL(5, 52, 1), // "t"
QT_MOC_LITERAL(6, 54, 12), // "set_min_area"
QT_MOC_LITERAL(7, 67, 3), // "min"
QT_MOC_LITERAL(8, 71, 12), // "set_max_area"
QT_MOC_LITERAL(9, 84, 3) // "max"

    },
    "ImgProcessSet\0set_num_fish\0\0n\0"
    "set_segment_threshold\0t\0set_min_area\0"
    "min\0set_max_area\0max"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImgProcessSet[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    1,   37,    2, 0x0a /* Public */,
       6,    1,   40,    2, 0x0a /* Public */,
       8,    1,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int, QMetaType::Int,    3,
    QMetaType::Int, QMetaType::Int,    5,
    QMetaType::Int, QMetaType::Int,    7,
    QMetaType::Int, QMetaType::Int,    9,

       0        // eod
};

void ImgProcessSet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImgProcessSet *_t = static_cast<ImgProcessSet *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->set_num_fish((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->set_segment_threshold((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->set_min_area((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->set_max_area((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject ImgProcessSet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ImgProcessSet.data,
      qt_meta_data_ImgProcessSet,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ImgProcessSet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImgProcessSet::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ImgProcessSet.stringdata0))
        return static_cast<void*>(const_cast< ImgProcessSet*>(this));
    return QObject::qt_metacast(_clname);
}

int ImgProcessSet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_ImgProcessSet_view_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImgProcessSet_view_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImgProcessSet_view_t qt_meta_stringdata_ImgProcessSet_view = {
    {
QT_MOC_LITERAL(0, 0, 18) // "ImgProcessSet_view"

    },
    "ImgProcessSet_view"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImgProcessSet_view[] = {

 // content:
       7,       // revision
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

void ImgProcessSet_view::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ImgProcessSet_view::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ImgProcessSet_view.data,
      qt_meta_data_ImgProcessSet_view,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ImgProcessSet_view::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImgProcessSet_view::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ImgProcessSet_view.stringdata0))
        return static_cast<void*>(const_cast< ImgProcessSet_view*>(this));
    return QWidget::qt_metacast(_clname);
}

int ImgProcessSet_view::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
