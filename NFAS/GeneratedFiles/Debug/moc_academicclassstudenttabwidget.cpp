/****************************************************************************
** Meta object code from reading C++ file 'academicclassstudenttabwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../academicclassstudenttabwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'academicclassstudenttabwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AcademicClassStudentTabWidget_t {
    QByteArrayData data[16];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AcademicClassStudentTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AcademicClassStudentTabWidget_t qt_meta_stringdata_AcademicClassStudentTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 29), // "AcademicClassStudentTabWidget"
QT_MOC_LITERAL(1, 30, 11), // "AcademicAdd"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 14), // "AcademicDelete"
QT_MOC_LITERAL(4, 58, 8), // "ClassAdd"
QT_MOC_LITERAL(5, 67, 11), // "ClassModify"
QT_MOC_LITERAL(6, 79, 11), // "ClassDelete"
QT_MOC_LITERAL(7, 91, 10), // "StudentAdd"
QT_MOC_LITERAL(8, 102, 13), // "StudentModify"
QT_MOC_LITERAL(9, 116, 13), // "StudentDelete"
QT_MOC_LITERAL(10, 130, 11), // "updateTable"
QT_MOC_LITERAL(11, 142, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(12, 159, 4), // "item"
QT_MOC_LITERAL(13, 164, 3), // "col"
QT_MOC_LITERAL(14, 168, 26), // "StudentAcademicClassChange"
QT_MOC_LITERAL(15, 195, 5) // "index"

    },
    "AcademicClassStudentTabWidget\0AcademicAdd\0"
    "\0AcademicDelete\0ClassAdd\0ClassModify\0"
    "ClassDelete\0StudentAdd\0StudentModify\0"
    "StudentDelete\0updateTable\0QTreeWidgetItem*\0"
    "item\0col\0StudentAcademicClassChange\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AcademicClassStudentTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    0,   67,    2, 0x0a /* Public */,
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    0,   71,    2, 0x0a /* Public */,
      10,    2,   72,    2, 0x0a /* Public */,
      14,    1,   77,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,   12,   13,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void AcademicClassStudentTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AcademicClassStudentTabWidget *_t = static_cast<AcademicClassStudentTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->AcademicAdd(); break;
        case 1: _t->AcademicDelete(); break;
        case 2: _t->ClassAdd(); break;
        case 3: _t->ClassModify(); break;
        case 4: _t->ClassDelete(); break;
        case 5: _t->StudentAdd(); break;
        case 6: _t->StudentModify(); break;
        case 7: _t->StudentDelete(); break;
        case 8: _t->updateTable((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->StudentAcademicClassChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AcademicClassStudentTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AcademicClassStudentTabWidget.data,
      qt_meta_data_AcademicClassStudentTabWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AcademicClassStudentTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AcademicClassStudentTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AcademicClassStudentTabWidget.stringdata0))
        return static_cast<void*>(const_cast< AcademicClassStudentTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AcademicClassStudentTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
