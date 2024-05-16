#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnection()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("karim");//inserer nom de l'utilisateur
db.setPassword("karim");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}


void Connection::closeConnection(){db.close();}
