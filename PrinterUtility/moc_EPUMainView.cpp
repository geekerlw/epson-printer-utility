/****************************************************************************
** Meta object code from reading C++ file 'EPUMainView.h'
**
** Created: Thu Dec 5 15:14:19 2013
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EPUMainView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EPUMainView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_EPUMainView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      48,   41,   12,   12, 0x0a,
      80,   74,   12,   12, 0x0a,
     105,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EPUMainView[] = {
    "EPUMainView\0\0btnShowPrinterListClicked()\0"
    "status\0setPrinterStatus(QString)\0state\0"
    "setPrinterState(QString)\0update()\0"
};

const QMetaObject EPUMainView::staticMetaObject = {
    { &EPUView::staticMetaObject, qt_meta_stringdata_EPUMainView,
      qt_meta_data_EPUMainView, 0 }
};

const QMetaObject *EPUMainView::metaObject() const
{
    return &staticMetaObject;
}

void *EPUMainView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EPUMainView))
	return static_cast<void*>(const_cast< EPUMainView*>(this));
    return EPUView::qt_metacast(_clname);
}

int EPUMainView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EPUView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnShowPrinterListClicked(); break;
        case 1: setPrinterStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: setPrinterState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: update(); break;
        }
        _id -= 4;
    }
    return _id;
}
