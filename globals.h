#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSqlDatabase>
#include <QMessageBox>

bool getDBConnection(QSqlDatabase);
int showMessageBox(QString,QString,QMessageBox::Button,QMessageBox::Icon);
int showMessageBox(QString,QString,QList<QMessageBox::Button>,QMessageBox::Icon);

//extern QString UserID;

#endif // GLOBALS_H
