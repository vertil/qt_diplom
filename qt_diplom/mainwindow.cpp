#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //test button
    connect(this->ui->pushButton, SIGNAL(clicked(bool)),this,SLOT(check_con()));
    //dep table
        //list
    connect(this->ui->get_deps_button, SIGNAL(clicked(bool)),this,SLOT(get_departments()));
    ui->tableDep->setHorizontalHeaderLabels(QStringList()<<"id"<<"Отдел");
    ui->tableDep->horizontalHeader()->setStretchLastSection(true);
        //add new
    connect(this->ui->NewDepBut, SIGNAL(clicked(bool)),this,SLOT(add_new_department()));

    //per table
        //list
    connect(this->ui->get_per_button, SIGNAL(clicked(bool)),this,SLOT(get_personal()));
    ui->tablePer->setHorizontalHeaderLabels(QStringList()<<"id"<<"Имя"
                                                          <<"Фамилия"<<"Отчество"
                                                          <<"email"<<"Дата рождения"
                                                          <<"dep_id");


    //db settings
    db=QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("192.168.126.130");

    db.setPort(5432);
    db.setDatabaseName("facedb");
    db.setUserName("userP");
    db.setPassword("mypass");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::check_con()
{

    bool ok = db.open();

    ui->checkBox->setChecked(ok);

    QSqlQuery query;
    QString commm="SELECT file "
                    "FROM public.faces "
                    "WHERE id=";
    commm+=std::to_string(ui->spinBox->value());
    query.exec(commm);

    qDebug()<< query.lastError();

    query.next();
    QByteArray name = query.value(0).toByteArray();
    qDebug()<<name.size()<<" "<<query.size();

    QPixmap image;
    if(ui->spinBox_2->value()==0){
        image.loadFromData(name,"PNG");

    }else{
        image.loadFromData(name,"JPG");
    }

    ui->label->resize(image.width(),image.height());
    ui->label->setPixmap(image);

    db.close();

    //////////////////////////
    // ui->label->setPixmap(image.scaled(image.width(),image.height(),Qt::KeepAspectRatio));
    //ui->label.setS
    // query.exec("SELECT * "
    //            "FROM public.department ");

    //while (query.next()) {
        // int id = query.value(0).toInt();



    //     QString name = query.value(1).toString();

    //     qDebug() << name;
    // }



}

void MainWindow::get_departments()
{
    //ui->tableDep->clear();
    ui->tableDep->setRowCount(0);

    bool ok = db.open();

    QSqlQuery query;
    QString commm="SELECT * "
                    "FROM public.department ";
    query.exec(commm);
    for(int i = 0; query.next(); i++){
        QString id = query.value(0).toString();

        QString name = query.value(1).toString();

        ui->tableDep->insertRow(i);
        ui->tableDep->setItem(i,0, new QTableWidgetItem(id));
        ui->tableDep->setItem(i,1, new QTableWidgetItem(name));

    }
    db.close();
}

void MainWindow::add_new_department()
{
    QString new_name=ui->NewDepEdit->text();

    bool ok = db.open();

    QSqlQuery query;
    QString commm="INSERT INTO public.department(name) "
                    "VALUES('"+new_name+"'); ";
    query.exec(commm);
    db.close();
    ui->NewDepEdit->clear();
}

void MainWindow::get_personal()
{
    //ui->tablePer->clear();
    ui->tablePer->setRowCount(0);

    bool ok = db.open();


    QSqlQuery query;
    QString commm="SELECT * "
                    "FROM public.personal ";
    query.exec(commm);
    for(int i = 0; query.next(); i++){
        QString id = query.value(0).toString();
        QString first_name = query.value(1).toString();
        QString last_name = query.value(2).toString();
        QString email = query.value(3).toString();
        QString birth = query.value(4).toString();
        QString father_name = query.value(5).toString();
        QString dep_id = query.value(6).toString();



        ui->tablePer->insertRow(i);
        ui->tablePer->setItem(i,0, new QTableWidgetItem(id));
        ui->tablePer->setItem(i,1, new QTableWidgetItem(first_name));
        ui->tablePer->setItem(i,2, new QTableWidgetItem(last_name));
        ui->tablePer->setItem(i,3, new QTableWidgetItem(father_name));
        ui->tablePer->setItem(i,4, new QTableWidgetItem(email));
        ui->tablePer->setItem(i,5, new QTableWidgetItem(birth));
        ui->tablePer->setItem(i,6, new QTableWidgetItem(dep_id));



    }
    db.close();
}
