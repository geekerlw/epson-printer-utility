/****************************************************************************
** Meta object code from reading C++ file 'EPUNozzleCheckResultView.h'
**
** Created: Tue Nov 19 10:10:01 2013
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EPUNozzleCheckResultView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EPUNozzleCheckResultView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_EPUNozzleCheckResultView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_EPUNozzleCheckResultView[] = {
    "EPUNozzleCheckResultView\0"
};

const QMetaObject EPUNozzleCheckResultView::staticMetaObject = {
    { &EPUView::staticMetaObject, qt_meta_stringdata_EPUNozzleCheckResultView,
      qt_meta_data_EPUNozzleCheckResultView, 0 }
};

const QMetaObject *EPUNozzleCheckResultView::metaObject() const
{
    return &staticMetaObject;
}

void *EPUNozzleCheckResultView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EPUNozzleCheckResultView))
	return static_cast<void*>(const_cast< EPUNozzleCheckResultView*>(this));
    return EPUView::qt_metacast(_clname);
}

int EPUNozzleCheckResultView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EPUView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
