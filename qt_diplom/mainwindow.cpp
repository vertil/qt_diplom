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


    //faces
        //list
    connect(this->ui->get_faces_button, SIGNAL(clicked(bool)),this,SLOT(get_faces()));
    ui->tableFaces->setHorizontalHeaderLabels(QStringList()<<"id"<<"id работника");
        //single
    connect(this->ui->get_face_image_but, SIGNAL(clicked(bool)),this,SLOT(get_face_image()));


    //cameras
        //list
    connect(this->ui->get_cameras_button, SIGNAL(clicked(bool)),this,SLOT(get_cameras()));
    ui->tableCam->setHorizontalHeaderLabels(QStringList()<<"id"<<"модель"
                                                         <<"ip"<<"id кабинета"
                                                         <<"направление"<<"№ прохода"
                                                         <<"статус");



    //cabinets
        //list
    connect(this->ui->get_cabinets_button, SIGNAL(clicked(bool)),this,SLOT(get_cabinets()));
    ui->tableCabinets->setHorizontalHeaderLabels(QStringList()<<"id"<<"название"
                                                          <<"этаж"<<"id отдела");


    //in_out
        //list
    connect(this->ui->get_inout_button, SIGNAL(clicked(bool)),this,SLOT(get_in_out()));
    ui->tableCabinets->setHorizontalHeaderLabels(QStringList()<<"time"<<"id работника"
                                                               <<"id камеры");



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
                    "FROM public.department; ";
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
                    "FROM public.personal; ";
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

void MainWindow::get_faces()
{
    ui->tableFaces->setRowCount(0);

    bool ok = db.open();

    QSqlQuery query;
    QString commm="SELECT id,personal_id "
                    "FROM public.faces; ";
    query.exec(commm);
    for(int i = 0; query.next(); i++){
        QString id = query.value(0).toString();
        QString per_id = query.value(1).toString();

        ui->tableFaces->insertRow(i);
        ui->tableFaces->setItem(i,0, new QTableWidgetItem(id));
        ui->tableFaces->setItem(i,1, new QTableWidgetItem(per_id));

    }
    db.close();
}

void MainWindow::get_face_image()
{
    bool ok = db.open();


    QSqlQuery query;
    QString commm="SELECT file "
                    "FROM public.faces "
                    "WHERE id=";
    commm+=std::to_string(ui->spin_face_id->value());
    commm+=";";
    query.exec(commm);

    qDebug()<< query.lastError()<<"ban";


    query.next();
    QByteArray name = query.value(0).toByteArray();
    qDebug()<<name.size()<<" "<<query.size();

    QPixmap image;
    image.loadFromData(name,"PNG");

    ui->face_image->resize(image.width()/5,image.height()/5);
    ui->face_image->setPixmap(image);

    db.close();
}

void MainWindow::get_cameras()
{
    ui->tableCam->setRowCount(0);

    bool ok = db.open();

    QSqlQuery query;
    QString commm="SELECT * "
                    "FROM public.cameras; ";
    query.exec(commm);
    for(int i = 0; query.next(); i++){
        QString id = query.value(0).toString();
        QString cam_model = query.value(1).toString();
        QString addr = query.value(2).toString();
        QString cab_id = query.value(3).toString();
        QString in_pos = query.value(4).toString();
        QString pass_num = query.value(5).toString();
        QString status = query.value(6).toString();

        ui->tableCam->insertRow(i);
        ui->tableCam->setItem(i,0, new QTableWidgetItem(id));
        ui->tableCam->setItem(i,1, new QTableWidgetItem(cam_model));
        ui->tableCam->setItem(i,2, new QTableWidgetItem(addr));
        ui->tableCam->setItem(i,3, new QTableWidgetItem(cab_id));

        QTableWidgetItem *item1 =new QTableWidgetItem();
        item1->data(Qt::CheckStateRole);
        if(in_pos=="true"){
            item1->setCheckState(Qt::Checked);
        }else{
            item1->setCheckState(Qt::Unchecked);
        }

        ui->tableCam->setItem(i,4, item1);

        ui->tableCam->setItem(i,5, new QTableWidgetItem(pass_num));

        QTableWidgetItem *item2=new QTableWidgetItem();
        item2->data(Qt::CheckStateRole);
        if(status=="true"){
            item2->setCheckState(Qt::Checked);
        }else{
            item2->setCheckState(Qt::Unchecked);
        }

        ui->tableCam->setItem(i,6, item2);

    }
    db.close();
}

void MainWindow::get_cabinets()
{
    ui->tableCabinets->setRowCount(0);

    bool ok = db.open();

    QSqlQuery query;
    QString commm="SELECT * "
                    "FROM public.cabinets; ";
    query.exec(commm);
    for(int i = 0; query.next(); i++){
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        QString floor = query.value(2).toString();
        QString dep_id = query.value(3).toString();

        ui->tableCabinets->insertRow(i);
        ui->tableCabinets->setItem(i,0, new QTableWidgetItem(id));
        ui->tableCabinets->setItem(i,1, new QTableWidgetItem(name));
        ui->tableCabinets->setItem(i,2, new QTableWidgetItem(floor));
        ui->tableCabinets->setItem(i,3, new QTableWidgetItem(dep_id));


    }
    db.close();
}

void MainWindow::get_in_out()
{
    ui->tableInOut->setRowCount(0);

    bool ok = db.open();

    QSqlQuery query;
    QString commm="SELECT * "
                    "FROM public.in_out_date; ";
    query.exec(commm);
    QString size=QString::number(query.size());
    ui->totalInOutLabel->setText(size);
    for(int i = 0; query.next(); i++){
        QString time = query.value(0).toString();
        QString per_id = query.value(1).toString();
        QString cam_id = query.value(2).toString();

        ui->tableInOut->insertRow(i);
        ui->tableInOut->setItem(i,0, new QTableWidgetItem(time));
        ui->tableInOut->setItem(i,1, new QTableWidgetItem(per_id));
        ui->tableInOut->setItem(i,2, new QTableWidgetItem(cam_id));


    }
    db.close();
}
