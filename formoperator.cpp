#include "formoperator.h"
#include "ui_formoperator.h"
#include "globals.h"
#include "formhistorydescribe.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>

FormOperator::FormOperator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormOperator)
{
    ui->setupUi(this);
    ui->TW_History->setColumnWidth(0,300);
    ui->TW_History->setColumnWidth(1,150);
    ui->TW_History->setColumnWidth(2,150);
    ui->TW_Pacient->setColumnWidth(0,50);
    ui->TW_Pacient->setColumnWidth(1,250);
    ui->RB_AbZ->setChecked(true);
    GetDataMedCardtoTable();
}

FormOperator::~FormOperator()
{
    delete ui;
}

void FormOperator::GetDataMedCardtoTable()//Функци для загрузки данных по пациентам
{
    db = QSqlDatabase::database("GetData");
    if(getDBConnection(db))
   {
       QSqlQuery query(db);
       query.prepare("select userid as ID,concat(UserSurname ,' ', UserName ,' ', UserOtchestvo) as ФИО, PhoneNumber as Номер from users order by ФИО ASC;");
       query.exec();
       while(query.next())
       {
           ui->TW_Pacient->insertRow(ui->TW_Pacient->rowCount());
           ui->TW_Pacient->setItem(ui->TW_Pacient->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
           ui->TW_Pacient->setItem(ui->TW_Pacient->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));
           ui->TW_Pacient->item(ui->TW_Pacient->rowCount()-1,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft|Qt::Alignment(Qt::TextWordWrap));//Выравинвание текста
           ui->TW_Pacient->setItem(ui->TW_Pacient->rowCount()-1,2,new QTableWidgetItem(query.value(2).toString()));
       }
   }
    else
    {
        showMessageBox("Внимание","Соединение с базой данных прервано.",QMessageBox::Ok,QMessageBox::Warning);
    }
//   db.close();//удаляет все объекты с данными
}

void FormOperator::Delete()//Функция отвечающая за удаление записи истории
{

}

void FormOperator::slot(QString a)//Передаёт информацию из предыдущей формы
{

}

void FormOperator::on_TW_Pacient_itemSelectionChanged()
{
    if(ui->TW_Pacient->currentRow() > -1)
    {
    db = QSqlDatabase::database("GetData");
    ui->TW_History->clear();
    ui->TW_History->setHorizontalHeaderItem(0, new QTableWidgetItem ("Описание болезни"));
    ui->TW_History->setHorizontalHeaderItem(1,new QTableWidgetItem ("Начало болезни"));
    ui->TW_History->setHorizontalHeaderItem(2,new QTableWidgetItem ("Конец болезни"));
    ui->TW_History->setRowCount(0);
    if(getDBConnection(db))
   {
       QSqlQuery query(db);
       query.prepare("select h.MedicalID,	h.HisDescribe,	date_format(h.startDate,'%d-%m-%Y'),date_format(h.closeDate,'%d-%m-%Y'),u.UserName,u.UserSurname,u.UserOtchestvo,u.PhoneNumber from history as h join medicalcard as m on h.MedicalID = m.MedCardID  join Users as u on u.UserID = m.UserID where u.userid = ?;");
       query.addBindValue(ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text());
       query.exec();
       while(query.next())
       {
           ui->TW_History->insertRow(ui->TW_History->rowCount());
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,0,new QTableWidgetItem(query.value(1).toString()));
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,1,new QTableWidgetItem(query.value(2).toString()));
           ui->TW_History->item(ui->TW_History->rowCount()-1,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft|Qt::Alignment(Qt::TextWordWrap));//Выравинвание текста
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,2,new QTableWidgetItem(query.value(3).toString()));
       }
       query.last();
       ui->LE_Name->setText(query.value(4).toString());
       ui->LE_Surname->setText(query.value(5).toString());
       ui->LE_Otchestvo->setText(query.value(6).toString());
       ui->LE_NumMedCard->setText(query.value(0).toString());
       ui->LE_Phone->setText(query.value(7).toString());
   }
    else
    {
        showMessageBox("Внимание","Соединение с базой данных прервано.",QMessageBox::Ok,QMessageBox::Warning);
    }
//   db.close();
    }
}


void FormOperator::on_TW_History_cellDoubleClicked(int row, int column)//Двойное нажатие на строку. Запускается форма просмотра истории
{
    formHistoryDescribe *formDescHistory = new formHistoryDescribe();
    formDescHistory->show();

    connect(this,&FormOperator::signal,formDescHistory,&formHistoryDescribe::slot);
    emit signal(ui->LE_Name->text(),ui->LE_Surname->text(),ui->LE_Otchestvo->text(),ui->TW_History->item(row,0)->text(),ui->TW_History->item(row,1)->text(),ui->TW_History->item(row,2)->text());//Активация сигнала

    connect (this,&FormOperator::signalisBool,formDescHistory,&formHistoryDescribe::slotsisSatus);//Соединил сигнал(signalisBool) и слот(slotsisStatus)
    emit signalisBool(false,1);
}


void FormOperator::on_RB_AbZ_clicked()
{
    ui->TW_Pacient->sortItems(1,Qt::SortOrder(Qt::AscendingOrder));

}


void FormOperator::on_RB_ZbA_clicked()
{
 ui->TW_Pacient->sortItems(1,Qt::SortOrder(Qt::DescendingOrder));

}


void FormOperator::on_LE_Search_textChanged(const QString &arg1)
{
    db = QSqlDatabase::database("GetData");
 ui->TW_Pacient->setRowCount(0);

    if(getDBConnection(db))
   {
       QSqlQuery query(db);
        if(!arg1.isNull())
       {
           QString temp = "select userid as ID,concat(UserSurname ,' ', UserName ,' ', UserOtchestvo) as ФИО, "
                          "PhoneNumber as Номер from users where UserName like '%"+ arg1 +"%' or UserSurname like '%"+ arg1 +"%' "
                          "or UserOtchestvo like '%"+ arg1 +"%' order by ФИО ASC;";

        query.exec(temp);
        while(query.next())
        {
           ui->TW_Pacient->insertRow(ui->TW_Pacient->rowCount());
           ui->TW_Pacient->setItem(ui->TW_Pacient->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
           ui->TW_Pacient->setItem(ui->TW_Pacient->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));
           ui->TW_Pacient->item(ui->TW_Pacient->rowCount()-1,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft|Qt::Alignment(Qt::TextWordWrap));//Выравинвание текста
           ui->TW_Pacient->setItem(ui->TW_Pacient->rowCount()-1,2,new QTableWidgetItem(query.value(2).toString()));
        }
       }
   }
    else
    {
        showMessageBox("Соединение с базой данных прервано.","Внимание",QMessageBox::Ok,QMessageBox::Warning);
    }
//   db.close();
}



//Остановился на функции доабвления
void FormOperator::on_A_Add_triggered()
{
   if(db.open())
   {
        formHistoryDescribe *addForm = new formHistoryDescribe();

        QSqlQuery query(db);
        query.prepare("select UserName,UserSurname,UserOtchestvo from Users where userid = ?; ");
        query.addBindValue(ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text());
        if(query.exec())
        {
        query.last();
        connect (this,&FormOperator::signalisBool,addForm,&formHistoryDescribe::slotsisSatus);//Соединил сигнал(signalisBool) и слот(slotsisStatus)
        emit signalisBool(true,0);

        connect (this,&FormOperator::signalNameSurOtch,addForm,&formHistoryDescribe::slotNameSurOtch);//Соединил сигнал(signalisBool) и слот(slotsisStatus)
        emit signalNameSurOtch(query.value(0).toString(),query.value(1).toString(),query.value(2).toString());

//        connect (this,&FormOperator::signalMedID,addForm,&formHistoryDescribe::slotsDataMedID);//Соединил сигнал(signalisBool) и слот(slotsisStatus)
//        emit signalMedID(ui->LE_NumMedCard->text().toInt());
        addForm->UserID = ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text().toInt();
        addForm->showNormal();
        while(addForm->isVisible())
           QApplication::processEvents();
        // Часть отвечающая за обнрвление истории пациента
        ui->TW_History->setRowCount(0);
        query.clear();
        query.prepare("select * from history as h join Medicalcard as m on h.MedicalID = m.MedCardID where UserID = ? ;");
        query.addBindValue(ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text());
        query.exec();
        while(query.next())
        {
           ui->TW_History->insertRow(ui->TW_History->rowCount());
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,0,new QTableWidgetItem(query.value(1).toString()));
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,1,new QTableWidgetItem(query.value(2).toString()));
           ui->TW_History->item(ui->TW_History->rowCount()-1,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft|Qt::Alignment(Qt::TextWordWrap));//Выравинвание текста
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,2,new QTableWidgetItem(query.value(3).toString()));
        }

        }
        else
        {
        showMessageBox("Не получилось выполнить запрос.","Ошибка",QMessageBox::Ok,QMessageBox::Critical);
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
        }


   }
   else
   {
        db.lastError().text();
   }
}

//Функция для удаления выбранной истории
void FormOperator::on_A_Del_triggered()
{

   if(ui->TW_History->currentRow()> -1)//Если хоть что-то выбрано
   {
//        db.open();
        QSqlQuery query(db);
        query.prepare("select * from medicalcard where UserID = ? ;");
        query.addBindValue(ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text());
        query.exec();
        query.next();
        int medid = query.value(0).toInt();
        query.prepare("delete from history where startDate = ? and closeDate = ? and MedicalID = ?;");
        //Изменение формата даты для SQL запроса
        QString opDate = ui->TW_History->item(ui->TW_History->currentRow(),1)->text();
        QDateTime date = QDateTime::fromString(opDate, "dd-MM-yyyy");
        QString newDateString = date.toString("yyyy-MM-dd");
        query.addBindValue(newDateString);
        QString closeDate = ui->TW_History->item(ui->TW_History->currentRow(),2)->text();
        date = QDateTime::fromString(closeDate, "dd-MM-yyyy");
        newDateString = date.toString("yyyy-MM-dd");
        query.addBindValue(newDateString);
        query.addBindValue(medid);
        if(query.exec())
        {showMessageBox("История успешно удалена.","Информация",QMessageBox::Ok,QMessageBox::Information);
//            qDebug()<<query.lastQuery();
//            qDebug()<<query.lastError().text();
            ui->TW_History->setRowCount(0);
            query.clear();
            query.prepare("select * from history as h join Medicalcard as m on h.MedicalID = m.MedCardID where UserID = ? ;");
            query.addBindValue(ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text());
            query.exec();
            while(query.next())
            {
                ui->TW_History->insertRow(ui->TW_History->rowCount());
                ui->TW_History->setItem(ui->TW_History->rowCount()-1,0,new QTableWidgetItem(query.value(1).toString()));
                ui->TW_History->setItem(ui->TW_History->rowCount()-1,1,new QTableWidgetItem(query.value(2).toString()));
                ui->TW_History->item(ui->TW_History->rowCount()-1,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft|Qt::Alignment(Qt::TextWordWrap));//Выравинвание текста
                ui->TW_History->setItem(ui->TW_History->rowCount()-1,2,new QTableWidgetItem(query.value(3).toString()));
            }
        }

   }
   else
   {
        showMessageBox("Выберите историю которую собираетесь удалить.","Внимание",QMessageBox::Ok,QMessageBox::Warning);
   }
}


void FormOperator::on_A_Upd_triggered()
{
   if(ui->TW_History->currentRow() > -1)
   {
   if(db.open())
   {
        formHistoryDescribe *addForm = new formHistoryDescribe();

        QSqlQuery query(db);
        query.prepare("select UserName,UserSurname,UserOtchestvo from Users where userid = ?; ");
        query.addBindValue(ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text());
        if(query.exec())
        {
        query.last();
        connect (this,&FormOperator::signalisBool,addForm,&formHistoryDescribe::slotsisSatus);//Соединил сигнал(signalisBool) и слот(slotsisStatus)
        emit signalisBool(true,2);

        connect(this,&FormOperator::signal,addForm,&formHistoryDescribe::slot);
        emit signal(ui->LE_Name->text(),ui->LE_Surname->text(),ui->LE_Otchestvo->text(),ui->TW_History->item(ui->TW_History->currentRow(),0)->text(),ui->TW_History->item(ui->TW_History->currentRow(),1)->text(),ui->TW_History->item(ui->TW_History->currentRow(),2)->text());//Активация сигнала

//        connect (this,&FormOperator::signalNameSurOtch,addForm,&formHistoryDescribe::slotNameSurOtch);//Соединил сигнал(signalisBool) и слот(slotsisStatus)
//        emit signalNameSurOtch(query.value(0).toString(),query.value(1).toString(),query.value(2).toString());

//        connect (this,&FormOperator::signalMedID,addForm,&formHistoryDescribe::slotsDataMedID);//Соединил сигнал(signalisBool) и слот(slotsisStatus)
//        emit signalMedID(ui->LE_NumMedCard->text().toInt());
        addForm->UserID = ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text().toInt();
        addForm->showNormal();
        while(addForm->isVisible())
           QApplication::processEvents();
        // Часть отвечающая за обновление истории пациента
        ui->TW_History->setRowCount(0);
        query.clear();
        query.prepare("select * from history as h join Medicalcard as m on h.MedicalID = m.MedCardID where UserID = ? ;");
        query.addBindValue(ui->TW_Pacient->item(ui->TW_Pacient->currentRow(),0)->text());
        query.exec();
        while(query.next())
        {
           ui->TW_History->insertRow(ui->TW_History->rowCount());
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,0,new QTableWidgetItem(query.value(1).toString()));
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,1,new QTableWidgetItem(query.value(2).toString()));
           ui->TW_History->item(ui->TW_History->rowCount()-1,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft|Qt::Alignment(Qt::TextWordWrap));//Выравинвание текста
           ui->TW_History->setItem(ui->TW_History->rowCount()-1,2,new QTableWidgetItem(query.value(3).toString()));
        }

        }
        else
        {
        showMessageBox("Не получилось выполнить запрос.","Ошибка",QMessageBox::Ok,QMessageBox::Critical);
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
        }


   }
   else
   {
        db.lastError().text();
   }
   }
}

