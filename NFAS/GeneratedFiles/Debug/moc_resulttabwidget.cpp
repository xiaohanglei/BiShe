/****************************************************************************
** Meta object code from reading C++ file 'resulttabwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../resulttabwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'resulttabwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ResultTabWidget_t {
    QByteArrayData data[13];
    char stringdata0[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ResultTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ResultTabWidget_t qt_meta_stringdata_ResultTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ResultTabWidget"
QT_MOC_LITERAL(1, 16, 9), // "ResultAdd"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "ResultAModify"
QT_MOC_LITERAL(4, 41, 12), // "ResultDelete"
QT_MOC_LITERAL(5, 54, 11), // "updateTable"
QT_MOC_LITERAL(6, 66, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(7, 83, 4), // "item"
QT_MOC_LITERAL(8, 88, 3), // "col"
QT_MOC_LITERAL(9, 92, 17), // "AttendanceFileGet"
QT_MOC_LITERAL(10, 110, 11), // "CheckChange"
QT_MOC_LITERAL(11, 122, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(12, 140, 12) // "GeneTestData"

    },
    "ResultTabWidget\0ResultAdd\0\0ResultAModify\0"
    "ResultDelete\0updateTable\0QTreeWidgetItem*\0"
    "item\0col\0AttendanceFileGet\0CheckChange\0"
    "QTableWidgetItem*\0GeneTestData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ResultTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    2,   52,    2, 0x0a /* Public */,
       9,    0,   57,    2, 0x0a /* Public */,
      10,    1,   58,    2, 0x0a /* Public */,
      12,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int,    7,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,    7,
    QMetaType::Void,

       0        // eod
};

void ResultTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ResultTabWidget *_t = static_cast<ResultTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ResultAdd(); break;
        case 1: _t->ResultAModify(); break;
        case 2: _t->ResultDelete(); break;
        case 3: _t->updateTable((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->AttendanceFileGet(); break;
        case 5: _t->CheckChange((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->GeneTestData(); break;
        default: ;
        }
    }
}

const QMetaObject ResultTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ResultTabWidget.data,
      qt_meta_data_ResultTabWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ResultTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ResultTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ResultTabWidget.stringdata0))
        return static_cast<void*>(const_cast< ResultTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ResultTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
