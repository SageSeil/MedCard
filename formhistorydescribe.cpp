#include "formhistorydescribe.h"
#include "ui_formhistorydescribe.h"
#include "globals.h"
#include "formoperator.h"

#include <QDebug>
#include <QSqlError>

formHistoryDescribe::formHistoryDescribe(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::formHistoryDescribe)
{
    ui->setupUi(this);
    ready = true;
    opDateOld = ui->DE_Open->date();
    clsDateOld = ui->DE_Close->date();
}

formHistoryDescribe::~formHistoryDescribe()
{
    delete ui;
}

void formHistoryDescribe::slot(QString name, QString surname, QString otchestvo, QString history, QString opDate, QString clsDate)
{

        ui->LE_Name->setText(name);
        ui->LE_Surname->setText(surname);
        ui->LE_Otchestvo->setText(otchestvo);
        ui->TE_DescribeHistory->setText(history);
        ui->DE_Open->setDate(QDate::fromString(opDate,"dd-MM-yyyy"));
        ui->DE_Close->setDate(QDate::fromString(clsDate,"dd-MM-yyyy"));


}

void formHistoryDescribe::slotNameSurOtch(QString name, QString surname, QString otchestvo)
{
    ui->LE_Name->setText(name);
    ui->LE_Surname->setText(surname);
    ui->LE_Otchestvo->setText(otchestvo);

}

void formHistoryDescribe::slotsisSatus(bool isVisibleBT, int modForm)
{
    switch (modForm)
    {
    case 0:
        {
        ui->LE_Name->setReadOnly(true);
        ui->LE_Surname->setReadOnly(true);
        ui->LE_Otchestvo->setReadOnly(true);
        ui->TE_DescribeHistory->setReadOnly(false);
        ui->DE_Open->setReadOnly(false);
        ui->DE_Close->setReadOnly(false);
        ui->BT_Add->setEnabled(true);
        ui->BT_Add->setVisible(true);
        ui->BT_Upd->setEnabled(false);
        ui->BT_Upd->setVisible(false);
            break;
        }
    case 1:
    {
            ui->LE_Name->setReadOnly(true);
            ui->LE_Surname->setReadOnly(true);
            ui->LE_Otchestvo->setReadOnly(true);
            ui->TE_DescribeHistory->setReadOnly(true);
            ui->DE_Open->setReadOnly(true);
            ui->DE_Close->setReadOnly(true);
            ui->BT_Add->setEnabled(false);
            ui->BT_Add->setVisible(false);
            ui->BT_Upd->setEnabled(false);
            ui->BT_Upd->setVisible(false);
            break;
    }
    case 2:
    {
            ui->LE_Name->setReadOnly(true);
            ui->LE_Surname->setReadOnly(true);
            ui->LE_Otchestvo->setReadOnly(true);
            ui->TE_DescribeHistory->setReadOnly(false);
            ui->DE_Open->setReadOnly(false);
            ui->DE_Close->setReadOnly(false);
            ui->BT_Add->setEnabled(false);
            ui->BT_Add->setVisible(false);
            ui->BT_Upd->setEnabled(true);
            ui->BT_Upd->setVisible(true);
            break;
    }
    }
}
int ids;
void formHistoryDescribe::slotsDataMedID(int id)
{
    ids = id;
}



//void formHistoryDescribe::slotsisSatus(bool a, bool isVisibleBT)
//{
//    ui->LE_Name->setReadOnly(a);
//    ui->LE_Surname->setReadOnly(a);
//    ui->LE_Otchestvo->setReadOnly(a);
//    ui->TE_DescribeHistory->setReadOnly(a);
//    ui->DE_Open->setReadOnly(a);
//    ui->DE_Close->setReadOnly(a);
//    ui->BT_Add->setVisible(isVisibleBT);
//}

void formHistoryDescribe::on_BT_Add_clicked()
{
    if(ui->TE_DescribeHistory->toPlainText() != "")
    {
    //При нажатии на кнопку добавить выполняется запрос
    db = QSqlDatabase::database("Insert");
    //db.open();
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("select * from medicalcard where UserID = ? ;");
        query.addBindValue(UserID);
        query.exec();
        if(query.next())
        {
            int medid = query.value(0).toInt();
            //QString strquery = "insert into history(MedicalID,HisDescribe,startDate,closeDate) value(,,,);";// Формируется запрос Для добавление новой истории;
            query.clear();
            query.prepare("insert into history(MedicalID,HisDescribe,startDate,closeDate) value(?, ?, ?, ?);");
            query.addBindValue(medid);
            query.addBindValue(ui->TE_DescribeHistory->toPlainText());
            query.addBindValue(ui->DE_Open->date());
            query.addBindValue(ui->DE_Close->date());
            if(query.exec())
            {
                showMessageBox("История успешно добавлена!","Информация",QMessageBox::Ok,QMessageBox::Information);
                this->close();
            }
            else
            {
                showMessageBox("Проблема с выполнением запроса!","Внимание!",QMessageBox::Ok,QMessageBox::Warning);
                qDebug() <<query.lastError().text();
                qDebug() << query.lastQuery();
            }
        }
        else
        {
          //Если нету мед карты то нужно сначала создать её а потом добавить историю
            query.clear();
            query.prepare("insert into medicalcard(UserID) value (?);");
            query.addBindValue(UserID);
            query.exec();
            query.exec("select max(MedCardID) from medicalcard;");
            qDebug()<< "1.Запрос на добавление новой медкарты и сразу получение ID новой медкарты \n"<<query.lastQuery();
            qDebug()<< "1.Запрос на добавление новой медкарты и сразу получение ID новой медкарты \n"<<query.result();
            if(query.next())
            {
                int medID = query.value(0).toInt();
                qDebug()<< "2. Последний запрос запоминание новсозданной ID медкарты"<<query.lastQuery();
                qDebug()<< "2. Последняя ошибка запоминание новсозданной ID медкарты"<<query.lastError().text();
                query.clear();
                query.prepare("insert into history(MedicalID,HisDescribe,startDate,closeDate) value(?, ?, ?, ?);");
                query.addBindValue(medID);
                query.addBindValue(ui->TE_DescribeHistory->toPlainText());
                query.addBindValue(ui->DE_Open->date());
                query.addBindValue(ui->DE_Close->date());
                if(query.exec())
                {
                    showMessageBox("История успешно добавлена!","Информация",QMessageBox::Ok,QMessageBox::Information);
                }
                else
                {
                    showMessageBox("Проблема с выполнением запроса!","Внимание!",QMessageBox::Ok,QMessageBox::Warning);
                    qDebug() <<query.lastError().text();
                    qDebug() << query.lastQuery();
                }
            }


        }
    }
    else
    {
        showMessageBox("Проблема с подключением базы данных","Внимание!",QMessageBox::Ok,QMessageBox::Warning);
        qDebug() << db.lastError().databaseText();
    }

    }
    else
    {
        showMessageBox("История болезни не заполнена.","Внимание!",QMessageBox::Ok,QMessageBox::Warning);
    }
}


void formHistoryDescribe::on_BT_Upd_clicked()
{
    if(ui->TE_DescribeHistory->toPlainText() != "")
    {
        db = QSqlDatabase::database("Insert");
        if(getDBConnection(db))
        {
        QSqlQuery query(db);
        query.prepare("select * from medicalcard where UserID = ? ;");
        query.addBindValue(UserID);
        query.exec();
        if(query.next())
         {
            int medid = query.value(0).toInt();
            query.clear();
            query.prepare("update history set  HisDescribe = '?' ,startDate = ?,closeDate = ? where MedicalID = ? and startDate = ? and closeDate = ?;");
            query.addBindValue(ui->TE_DescribeHistory->toPlainText());
            query.addBindValue(ui->DE_Open->date());
            query.addBindValue(ui->DE_Close->date());
            query.addBindValue(medid);
            query.addBindValue(opDateOld);
            query.addBindValue(clsDateOld);

            if(query.exec())
            {
                qDebug()<<query.lastQuery();
                qDebug()<<query.lastError().text();
                showMessageBox("История успешно отредактирована!","Информация",QMessageBox::Ok,QMessageBox::Information);
                this->close();
            }
            else
            {
                showMessageBox("Проблема с выполнением запроса!","Внимание!",QMessageBox::Ok,QMessageBox::Warning);
                qDebug() <<query.lastError().text();
                qDebug() << query.lastQuery();
            }
         }
        }
    }
}

