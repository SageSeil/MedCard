#ifndef FORMAUTORIZATION_H
#define FORMAUTORIZATION_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class FormAutorization; }
QT_END_NAMESPACE

class FormAutorization : public QMainWindow
{
    Q_OBJECT

public:
    FormAutorization(QWidget *parent = nullptr);
    ~FormAutorization();

public slots:
    void on_BT_Auth_clicked();

private:
    Ui::FormAutorization *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","autorization");//Определили метку подключения
signals:
    void signal(QString);
};
#endif // FORMAUTORIZATION_H
