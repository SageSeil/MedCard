#ifndef FORMHISTORYDESCRIBE_H
#define FORMHISTORYDESCRIBE_H

#include "qdatetime.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class formHistoryDescribe;
}

class formHistoryDescribe : public QMainWindow
{
    Q_OBJECT

public:
    explicit formHistoryDescribe(QWidget *parent = nullptr);
    ~formHistoryDescribe();
    int UserID;
    QDate opDateOld;
    QDate clsDateOld;
public slots:
    void slot(QString name,QString surname,QString otchestvo,QString history,QString opDate, QString clsDate);
    void slotNameSurOtch(QString name,QString surname,QString otchestvo);
    void slotsisSatus(bool isVisibleBT,int modForm);
    void slotsDataMedID(int id);//номер медкарты

private slots:
    void on_BT_Add_clicked();

    void on_BT_Upd_clicked();

private:
    Ui::formHistoryDescribe *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","Insert");
    bool ready = false;
};

#endif // FORMHISTORYDESCRIBE_H
