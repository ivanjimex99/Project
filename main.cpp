#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <iostream>
#include <QtDebug>
#include <QtSql/QSqlTableModel>
#include <QDateTime>
#include <QElapsedTimer>
#include <stdlib.h>
#include <sqlite3.h>
#include <QtSql/QSqlQuery>
#include <QVariant>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString user = argv[0];
    QString passw = argv[1];
    QSqlDatabase data_base;
    QDateTime timer = QDateTime::currentDateTime();
    QString fecha = timer.toString("yyyy-MM-dd");
    QString hora = timer.toString("hh:mm:ss");
    QElapsedTimer time;
    QSqlQuery query;

    data_base = QSqlDatabase::addDatabase("QMYSQL");
    data_base.setDatabaseName("temperaturas");
    data_base.setHostName("localhost");
    data_base.setPort(3306);
    data_base.setUserName("root");
    data_base.setPassword("root");

    if( !data_base.open()){
        //QMessageBox::critical(this,"Conexion",mDataBase.lastError().text());
        //return;
    }

    query.prepare("INSERT INTO sesiones (usuario,fecha,hora) VALUES (:usuario,:fecha,:hora)");
    query.bindValue(":usuario", user);
    query.bindValue(":fecha",fecha);
    query.bindValue(":hora", hora);
    query.exec();

    time.start();
    qint64 han_pasado;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    float temperatura,humedad,velocidad, direccion, precipitacion, intensidad;

    float maximo = -1, minimo = 99999, promedio = 0;

    int i = 0;

    while(i >= 0){

        han_pasado = time.nsecsElapsed();
        temperatura = rand()%35+10;
        humedad = rand()%100;
        velocidad = rand()%40;
        direccion = rand()%360-180;
        precipitacion = rand()%50;
        intensidad = rand()%2000;

        if( temperatura > maximo){
            maximo = temperatura;
        }
        if(humedad > maximo){
            maximo = humedad;
        }
        if( velocidad > maximo){
            maximo = velocidad;
        }
        if(direccion > maximo ){
            maximo = direccion;
        }
        if( precipitacion > maximo){
            maximo = precipitacion;
        }
        if( intensidad > maximo){
            maximo = intensidad;
        }

        if( temperatura < minimo){
            minimo = temperatura;
        }
        if(humedad < minimo){
            minimo = humedad;
        }
        if( velocidad < minimo){
            minimo = velocidad;
        }
        if(direccion < minimo ){
            minimo = direccion;
        }
        if( precipitacion < minimo){
            minimo = precipitacion;
        }
        if( intensidad < minimo){
            minimo = intensidad;
        }
        i += 1;
        promedio += humedad+temperatura+velocidad+direccion+precipitacion+intensidad;
        if( han_pasado >= 10000){
            //ha transcurrido un minuto

            time.restart();
            promedio = promedio/i;
            QSqlQuery query2;
            query2.prepare("INSERT INTO temperaturas (minimo,maximo,promedio) VALUES (:minimo,:maximo,:promedio)");
            query2.bindValue(":minimo",minimo);
            query2.bindValue(":maximo",maximo);
            query2.bindValue(":promedio",promedio);

                if(query2.exec()){
                //    QMessageBox::critical(this,"Alerta","Ha pasado un minuto ...");

                }
            maximo = -1;
            minimo = 99999;
            promedio = 0;
            i = 0;
        }


    }

    return a.exec();
}
