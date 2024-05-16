#include "arduino.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

#include <QThread>
#include <limits>
#include <QStringList>
#include <QString>
Arduino::Arduino()
{
    data="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
   return serial;
}
int Arduino::connect_arduino()
{   // recherche du port sur lequel la carte arduino identifée par  arduino_uno_vendor_id
    // est connectée
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
           if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()){
               if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier()
                       == arduino_uno_producy_id) {
                   arduino_is_available = true;
                   arduino_port_name=serial_port_info.portName();
               } } }
        qDebug() << "arduino_port_name is :" << arduino_port_name;
        if(arduino_is_available){ // configuration de la communication ( débit...)
            serial->setPortName(arduino_port_name);
            if(serial->open(QSerialPort::ReadWrite)){
                serial->setBaudRate(QSerialPort::Baud9600); // débit : 9600 bits/s
                serial->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
                serial->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
                serial->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
                serial->setFlowControl(QSerialPort::NoFlowControl);
                return 0;
            }
            return 1;
        }
        return -1;
}

int Arduino::close_arduino()

{

    if(serial->isOpen()){
            serial->close();
            return 0;
        }
    return 1;


}


 QByteArray Arduino::read_from_arduino()
{
    if(serial->isReadable()){
         data=serial->readAll(); //récupérer les données reçues

         return data;
    }
 }


int Arduino::write_to_arduino( QByteArray d)

{

    if(serial->isWritable()){
        serial->write(d);  // envoyer des donnés vers Arduino
    }else{
        qDebug() << "Couldn't write to serial!";
    }


}
QString Arduino::get_equipment_state_from_database(int equipmentID)
{
    // Assuming you have a method to establish a database connection
    // QSqlDatabase db = establish_database_connection();

    // Query the database to get the state of the equipment
    QSqlQuery query;
    query.prepare("SELECT etat FROM equipement WHERE id_equipement = :id");
    query.bindValue(":id", equipmentID);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Failed to retrieve equipment state from the database!";
        return "";
    }
}
QString Arduino::get_equipment_name_from_database(int equipmentID)
{
    // Assuming you have a method to establish a database connection
    // QSqlDatabase db = establish_database_connection();

    // Query the database to get the state of the equipment
    QSqlQuery query;
    query.prepare("SELECT designation FROM equipement WHERE id_equipement = :id");
    query.bindValue(":id", equipmentID);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Failed to retrieve equipment state from the database!";
        return "";
    }
}






 int Arduino::write_equipment_id_to_arduino(QString equipmentID)
 {
     // Retrieve the name and state of the equipment from the database
     QString equipmentName = get_equipment_name_from_database(equipmentID.toInt());
     QString equipmentState = get_equipment_state_from_database(equipmentID.toInt());

     // Combine name and state into a single string
     QString dataString = "Name: " + equipmentName + ", Etat: " + equipmentState;

     // Convert the string to QByteArray
     QByteArray data = dataString.toUtf8(); // Assuming UTF-8 encoding

     // Send the data to Arduino
     if (serial->isWritable()) {
         serial->write(data);  // Send the data to Arduino
         return 0; // Return 0 on success
     } else {
         qDebug() << "Couldn't write to serial!";
         return 1; // Return 1 on failure
     }
 }
 bool Arduino::store_serial_data()
 {
     if(serial->bytesAvailable()){
         data = serial->readAll(); // Read data from the serial port
         qDebug() << "Data received from serial:" << data;
         return true; // Data successfully received
     } else {
         qDebug() << "No data available from serial!";
         return false; // No data available
     }
 }

 float Arduino::readTemperature() {
     if (!serial->isOpen()) {
         qDebug() << "Trying to open serial port";
         if (connect_arduino() != 0) {
             qDebug() << "Failed to connect to Arduino";
             return std::numeric_limits<float>::quiet_NaN();
         }
     }

     QThread::sleep(3);  // Attendre 3 secondes avant de lire les données

     if (serial->waitForReadyRead(3000)) {  // Attend encore jusqu'à 3 secondes pour les données
         QByteArray rawData = serial->readAll();
         qDebug() << "Data received:" << rawData;

         QString data = rawData;  // Utilisez les données réelles au lieu d'une chaîne simulée
         QStringList temperatures = data.split("\r\n"); // Diviser les données par des sauts de ligne

         bool ok;
         float temp;
         foreach (const QString &value, temperatures) {
             temp = value.toFloat(&ok);
             if (ok && temp >= 10.0 && temp <= 50.0) {  // Vérifie que la température est entre 10.0°C et 50.0°C
                 qDebug() << "Parsed temperature:" << temp;
                 return temp;  // Renvoie la première température valide
             }
         }
         qDebug() << "No valid temperature found.";
         return std::numeric_limits<float>::quiet_NaN();  // Renvoie NaN si aucune température valide n'est trouvée
     } else {
         qDebug() << "No data available to read";
         return std::numeric_limits<float>::quiet_NaN();  // Renvoie NaN si aucune donnée n'est prête
     }
 }
 void Arduino::write(const QString &data) {
     if (serial->isWritable()) {
         serial->write(data.toUtf8());
     } else {
         qDebug() << "Couldn't write to serial!";
     }
 }
