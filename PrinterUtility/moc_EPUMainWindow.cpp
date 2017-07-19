/****************************************************************************
** Meta object code from reading C++ file 'EPUMainWindow.h'
**
** Created: Thu Dec 5 15:14:13 2013
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EPUMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EPUMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_EPUMainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x09,
      34,   14,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_EPUMainWindow[] = {
    "EPUMainWindow\0\0btnBuyInkClicked()\0"
    "btnDriverSettingClicked()\0"
};

const QMetaObject EPUMainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EPUMainWindow,
      qt_meta_data_EPUMainWindow, 0 }
};

const QMetaObject *EPUMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *EPUMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EPUMainWindow))
	return static_cast<void*>(const_cast< EPUMainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int EPUMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnBuyInkClicked(); break;
        case 1: btnDriverSettingClicked(); break;
        }
        _id -= 2;
    }
    return _id;
}
