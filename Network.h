/* 
 * File:   Drohne.h
 * Author: asctec
 *
 * Created on May 13, 2015, 2:18 PM
 */

#include <pthread.h>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <time.h>


#ifndef NETWORK_H
#define	NETWORK_H

struct waypoint {
    std::string typ;
    double latitude;
    double longitude;
    double height;
    double charge;
    int speed;
};

struct cameraStatus {
    bool cameraLeft;
    bool cameraRight;
};


/**
 * struct mit den Parametern pictureString(enthält einen richtig formatierten string) und 
 * length(enthält die länge des strings);
 */
struct pictureStruct{
    std::string picture;
    int length;
};

class Network {
public:
    /**
     * Die IpAdresse wird Standardmässig auf den localhost gesetzt.
     */
    Network();
    Network(const Network& orig);

    /**
     * Initialisiert einen Socket zum senden von Daten an das Ipad.
     * @return Im Fehlerfall wird eine -1 zurückgegeben
     */
    int initSocketSendInformation();

    /**
     * Initialisiert einen Socket zum senden von Daten an das Ipad.
     * @return Im Fehlerfall wird eine -1 zurückgegeben
     */
    int initSocketSendPicture();

    /**
     * Initialisiert einen Socket zum Empfangen von Daten vom Ipad.
     * @return Im Fehlerfall wird eine -1 zurückgegeben
     */
    int initSocketRecv();

    /**
     * Die Nutzinformationen von der Drohne werden übergben und in einen string mit passendem format umgewandelt.
     * @param typ
     * @param latitude
     * @param longitude
     * @param height
     * @param charge
     * @param speed
     * @return 
     */
    std::string parseSend(std::string typ, double latitude, double longitude, double height, double charge, int speed);



    /**
     * Empfängt Missionen von der IpadApp. 
     * Die empfangen Daten werden in sturct Waypoint umgewandelt und im vector waypoints_.
     * @return 
     * Im Fehlerfall wird eine -1 zurückgegeben.
     * Wenn keine witeren Missionen mehr kommen wird -2 zurückgegeben.
     * Bei Erfolgreichen Empafngen der Dtaen wird Größe der Empfangen Daten zurückgegeben.
     * 
     */
    int recvMission();

    /**
     * 
     * @param information string der vorher von parseSend() erzeugt wird.
     * Sendet die formatierten Nutzdaten an die IpadApp.
     * @return 
     */
    int sendDataInformation(std::string information);

    /**
     * 
     * @param picture string der vorher von parsePicture() erzeugt wird.
     * Sendet ein vorher formatiertes Bild an die IpadApp.
     * @return 
     */
    int sendDataPicture(pictureStruct picture);

    /**
     * Setzt die IPadresse des Empfängers.
     * @param ipAdress
     */
    void setIpadress(std::string ipAdress);

    /**
     * 
     * @param information string der vorher von parseSend() erzeugt wird.
     * Führt die Funktion sendDataInformation(std::string information) in einem thread aus.
     */
    void threadSendDataInformation(std::string information);

    /**
     * 
     * @param picture string der vorher von parsePicture() erzeugt wird.
     * Führt die Funktion sendDataPicture(std::string picture) in einem thread aus.
     */
    void threadSendDataPicture(pictureStruct picture);

    /**
     * Führt die Funktion recvMission() in einem thread aus.
     */
    void threadRecvData();

    /**
     * Die Funktion liest ein Bild ein und formatiert den string in ein passendes format.
     * Der string wird in einer struct parsedPciture gespeichert und zurückgegeben
     * @param picturepath Gibt den Pfad des Bildes an.
     * @param position Position in der Bildreihenfolge.
     * @return struct vom typ parsedPicture.
     */
    pictureStruct readPicture(std::string picturePath, int position);
    virtual ~Network();

    /**
     * Liefert den vector waypoints_ zurück und löscht ihn anschießend.
     * @return vector<struct waypoint>
     */
    std::vector<struct waypoint> getWaypoints();
    
    int getCameraStatus();

    /**
     * Die Funktion überprüft was für ein Paket Typ empfangen wurde.
     * Dabei kann es sich um Mission mit Koordinaten sein oder Toggel Befehle für die Kamera.
     * Der Typ wird zurückgegeben.
     * @param information
     * @return 
     */
    std::string checkDataTyp(std::string information);


private:


     /**
     * Ein Bild, dessen Position in der Bildreihenfolge und dessen länge
     *  wird übergeben und in einen strign mit passendem fprmat umgewandelt.
     * @param picture
     * @param position
     * @param length
     * @return parsedPicture
     
    parsedPicture parsePicture(char picture[], int position, int length);
       */
    
    /**
     * Die Funktion entpackt ein empfangenen toggel Befehl.
     * Der Befehl wird in einer struct cmaeraStatus gespeichert
     * @param toggelCommand
     * @return struct cameraStatus
     */
    cameraStatus parseRecvToggel(std::string toggelCommand);

    /**
     * Die Funktion entpackt ein empfangenen Missions Wegpunkt.
     * Der Wegpunkt wird in einer stuct waypoint gepeichert.
     * @param information
     * @return struct waypoint
     */
    waypoint parseRecvData(std::string information);

    struct cameraStatus cameraStatus_;
    int socketSendInformation_;
    int socketSendPicture_;
    int socketRecv_;
    std::string ipAdress_;
    std::vector<struct waypoint> waypoints_;
    struct sockaddr_in server_addr_sendInformation_;
    struct sockaddr_in server_addr_sendPicture_;
    struct sockaddr_in server_addr_recv_;
    pthread_mutex_t waypointMutex_;
    pthread_mutex_t toggelMutex_;

};


class DrohneWrapperClass {
public:
    DrohneWrapperClass(Network* drohne, pictureStruct picture);
    DrohneWrapperClass(Network* drohne, std::string information);
    DrohneWrapperClass(const DrohneWrapperClass& orig);
    virtual ~DrohneWrapperClass();

    Network* myDrohne_;
    std::string information_;
    struct pictureStruct picture_;
private:

};

#endif	/* NETWORK_H */