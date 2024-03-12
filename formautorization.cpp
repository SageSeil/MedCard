#include "formautorization.h"
#include "ui_formautorization.h"
#include "globals.h"
#include "formoperator.h"//Соединили 2 формы

FormAutorization::FormAutorization(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FormAutorization)
{
    ui->setupUi(this);
}

FormAutorization::~FormAutorization()
{
    delete ui;
}


void FormAutorization::on_BT_Auth_clicked()
{
    db = QSqlDatabase::database("autorization");
    if(getDBConnection(db))
    {
        if(ui->LE_Login->text().remove("").length()>0)
        {
            if(ui->LE_Password->text().remove("").length()>0)
            {
                QSqlQuery query(db);
                query.prepare("select Users.useriD, concat(Users.UserSurname, ' ', Users.UserName, ' ', Users.UserOtchestvo) as FIO,role.RoleID,role.RoleName from Users inner join role on role.RoleID= Users.RoleID inner join autorization on autorization.AuthID = Users.AutorizationID where autorization.AuthLogin = ? and Autorization.AuthPassword = ?;");
                query.addBindValue(ui->LE_Login->text().remove(""));
                query.addBindValue(ui->LE_Password->text().remove(""));
                query.exec();
                if(query.next())
                {
                    FormOperator *mainForm = new FormOperator();
                    this->hide();
                    mainForm->show();
                    //Передача информации на следующую форму
                    connect(this,&FormAutorization::signal,mainForm,&FormOperator::slot);
                    emit signal(ui->LE_Login->text());//Активация сигнала

                    while(mainForm->isVisible())
                    {
                        QApplication::processEvents();
                    }
                    ui->LE_Login->clear();
                    ui->LE_Password->clear();
                    ui->LE_Login->setFocus();
                    this->show();
                }
                else
                {
                    showMessageBox("Неверный логин или пароль.\nПользователь не обнаружен.","Неудачно",QMessageBox::Ok,QMessageBox::Warning);
                }
            }
            else
            {
                showMessageBox("Поле 'Пароль' должно быть заполнено.","Действие прервано",QMessageBox::Ok,QMessageBox::Warning);
            }
        }
        else
        {
            showMessageBox("Поле 'Логин' должно быть заполнено","Действие прервано",QMessageBox::Ok,QMessageBox::Warning);
        }
    }
    else
    {
        showMessageBox("","Действие прервано",QMessageBox::Ok,QMessageBox::Warning);
    }
}

