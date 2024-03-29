#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QByteArray>
#include <QSqlError>
#include <iostream>
#include <QFileDialog>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void check_con();
    void get_departments();
    void add_new_department();
    void get_personal();
    void get_faces();
    void get_face_image();
    void get_cameras();
    void get_cabinets();
    void get_in_out();
    void choose_file();
private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
