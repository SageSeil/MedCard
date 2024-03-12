#ifndef FORMOPERATOR_H
#define FORMOPERATOR_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class FormOperator;
}

class FormOperator : public QMainWindow
{
    Q_OBJECT

public:
    explicit FormOperator(QWidget *parent = nullptr);
    ~FormOperator();
    void GetDataMedCardtoTable();
    void Delete();

private:
    Ui::FormOperator *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","GetData");//Определили метку подключения
//    QSqlDatabase dbh = QSqlDatabase::addDatabase("QMYSQL","GetHistory");
//    QSqlDatabase dbs = QSqlDatabase::addDatabase("QMYSQL","Search");
public slots:
    void slot(QString a);
private slots:
    void on_TW_Pacient_itemSelectionChanged();
    void on_TW_History_cellDoubleClicked(int row, int column);
    void on_RB_AbZ_clicked();

    void on_RB_ZbA_clicked();

    void on_LE_Search_textChanged(const QString &arg1);


    void on_A_Add_triggered();

    void on_A_Del_triggered();

    void on_A_Upd_triggered();

signals:
    void signal(QString,QString,QString,QString,QString,QString);
    void signalNameSurOtch(QString,QString,QString);
    void signalisBool(bool,int);//Сигнал для настройки в новой форме доступа к элементам.
    void signalMedID(int);//Информация о медкарте
    void signalUserID(int);//Информация о пользователе
};

#endif // FORMOPERATOR_H
