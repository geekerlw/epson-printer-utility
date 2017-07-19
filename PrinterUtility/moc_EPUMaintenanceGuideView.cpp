/****************************************************************************
** Meta object code from reading C++ file 'EPUMaintenanceGuideView.h'
**
** Created: Thu Dec 5 15:14:17 2013
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EPUMaintenanceGuideView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EPUMaintenanceGuideView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_EPUMaintenanceGuideView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x0a,
      44,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EPUMaintenanceGuideView[] = {
    "EPUMaintenanceGuideView\0\0btnCancelClicked()\0"
    "btnPrintClicked()\0"
};

const QMetaObject EPUMaintenanceGuideView::staticMetaObject = {
    { &EPUOperationView::staticMetaObject, qt_meta_stringdata_EPUMaintenanceGuideView,
      qt_meta_data_EPUMaintenanceGuideView, 0 }
};

const QMetaObject *EPUMaintenanceGuideView::metaObject() const
{
    return &staticMetaObject;
}

void *EPUMaintenanceGuideView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EPUMaintenanceGuideView))
	return static_cast<void*>(const_cast< EPUMaintenanceGuideView*>(this));
    return EPUOperationView::qt_metacast(_clname);
}

int EPUMaintenanceGuideView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EPUOperationView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnCancelClicked(); break;
        case 1: btnPrintClicked(); break;
        }
        _id -= 2;
    }
    return _id;
}
