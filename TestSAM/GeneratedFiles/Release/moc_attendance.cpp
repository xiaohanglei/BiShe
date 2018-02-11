/****************************************************************************
** Meta object code from reading C++ file 'attendance.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../attendance.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'attendance.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AttendanceM_t {
    QByteArrayData data[8];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AttendanceM_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AttendanceM_t qt_meta_stringdata_AttendanceM = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AttendanceM"
QT_MOC_LITERAL(1, 12, 5), // "chang"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 8), // "Changeui"
QT_MOC_LITERAL(4, 28, 14), // "Disconnectdeal"
QT_MOC_LITERAL(5, 43, 6), // "attend"
QT_MOC_LITERAL(6, 50, 8), // "slotSign"
QT_MOC_LITERAL(7, 59, 10) // "attendList"

    },
    "AttendanceM\0chang\0\0Changeui\0Disconnectdeal\0"
    "attend\0slotSign\0attendList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AttendanceM[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AttendanceM::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AttendanceM *_t = static_cast<AttendanceM *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->chang(); break;
        case 1: _t->Changeui(); break;
        case 2: _t->Disconnectdeal(); break;
        case 3: _t->attend(); break;
        case 4: _t->slotSign(); break;
        case 5: _t->attendList(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AttendanceM::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AttendanceM::chang)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject AttendanceM::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AttendanceM.data,
      qt_meta_data_AttendanceM,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AttendanceM::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AttendanceM::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AttendanceM.stringdata0))
        return static_cast<void*>(const_cast< AttendanceM*>(this));
    return QWidget::qt_metacast(_clname);
}

int AttendanceM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AttendanceM::chang()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
