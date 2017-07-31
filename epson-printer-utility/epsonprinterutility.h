#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_epsonprinterutility.h"

class epsonprinterutility : public QMainWindow
{
	Q_OBJECT

public:
	epsonprinterutility(QWidget *parent = Q_NULLPTR);

private:
	Ui::epsonprinterutilityClass ui;
};
