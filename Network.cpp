/* 
 * File:   Drohne.cpp
 * Author: asctec
 * 
 * Created on May 13, 2015, 2:18 PM
 */

#include "Network.h"

Network::Network() {
    ipAdress_ = "127.0.0.1";
    waypointMutex_ = PTHREAD_MUTEX_INITIALIZER;
    toggelMutex_ = PTHREAD_MUTEX_INITIALIZER;
}

Network::Network(const Network& orig) {
}

Network::~Network() {
}

int Network::initSocketSendInformation() {
    int result = -1;


    // memset(buf, 0, 10);

    socketSendInformation_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketSendInformation_ < 0) {
        perror("failed to create TCP socket\n");
        return -1;
    }


    int optval = 1;
    result = setsockopt(socketSendInformation_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval)); //SO_REUSeADDR
    if (result < 0) {
        perror("Option REUSEADDR konnte nicht gesetzt werden");
        return -1;
    }



    server_addr_sendInformation_.sin_family = AF_INET;
    server_addr_sendInformation_.sin_port = htons(50000);
    server_addr_sendInformation_.sin_addr.s_addr = inet_addr(ipAdress_.c_str());

    int addr_len = sizeof (struct sockaddr_in);

    if (connect(socketSendInformation_, (struct sockaddr *) &server_addr_sendInformation_, sizeof (server_addr_sendInformation_)) == 0) {
        printf("Verbindung mit dem Server (%s) hergestellt\n", inet_ntoa(server_addr_sendInformation_.sin_addr));
        return 1;
    } else
        return -1;

}

int Network::initSocketSendPicture() {
    int result = -1;


    // memset(buf, 0, 10);

    socketSendPicture_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketSendPicture_ < 0) {
        perror("failed to create TCP socket\n");
        return -1;
    }


    int optval = 1;
    result = setsockopt(socketSendPicture_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval)); //SO_REUSeADDR
    if (result < 0) {
        perror("Option REUSEADDR konnte nicht gesetzt werden");
        return -1;
    }



    server_addr_sendPicture_.sin_family = AF_INET;
    server_addr_sendPicture_.sin_port = htons(50001);
    server_addr_sendPicture_.sin_addr.s_addr = inet_addr(ipAdress_.c_str());

    int addr_len = sizeof (struct sockaddr_in);

    if (connect(socketSendPicture_, (struct sockaddr *) &server_addr_sendPicture_, sizeof (server_addr_sendPicture_)) == 0) {
        printf("Verbindung mit dem Server (%s) hergestellt\n", inet_ntoa(server_addr_sendPicture_.sin_addr));
        return 1;
    } else
        return -1;

}

int Network::initSocketRecv() {
    int result = -1;


    // memset(buf, 0, 10);

    socketRecv_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketRecv_ < 0) {
        perror("failed to create TCP socket\n");
        return -1;
    }


    int optval = 1;
    result = setsockopt(socketRecv_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval)); //SO_REUSeADDR
    if (result < 0) {
        perror("Option REUSEADDR konnte nicht gesetzt werden");
        return -1;
    }



    server_addr_recv_.sin_family = AF_INET;
    server_addr_recv_.sin_port = htons(60000);
    server_addr_recv_.sin_addr.s_addr = inet_addr(ipAdress_.c_str());

    int addr_len = sizeof (struct sockaddr_in);

    if (connect(socketRecv_, (struct sockaddr *) &server_addr_recv_, sizeof (server_addr_recv_)) == 0) {
        printf("Verbindung mit dem Server (%s) hergestellt\n", inet_ntoa(server_addr_recv_.sin_addr));
        return 1;
    } else
        return -1;
}

void Network::setIpadress(std::string ipAdress) {
    ipAdress_ = ipAdress;
}

std::string Network::parseSend(std::string typ, double latitude, double longitude, double height, double charge, int speed) {
    std::ostringstream latitudeStr;
    std::ostringstream longitudeStr;
    std::ostringstream heightStr;
    std::ostringstream chargeStr;
    std::ostringstream speedStr;
    std::string slatitude;
    std::string slongitude;
    std::string sheight;
    std::string scharge;
    std::string sspeed;
    std::string information;

    latitudeStr << latitude;
    slatitude = latitudeStr.str();
    longitudeStr << longitude;
    slongitude = longitudeStr.str();
    heightStr << height;
    sheight = heightStr.str();
    chargeStr << charge;
    scharge = chargeStr.str();
    speedStr << speed;
    sspeed = speedStr.str();

    information = typ + ";" + slatitude + "," + slongitude + "," + sheight + ";" + scharge + ";" + sspeed + ";" + "end";

    //std::cout<<slatitude<<" "<<slongitude<<" "<<sheight<<std::endl;
    // std::cout << information << std::endl;


    return information;
}

waypoint Network::parseRecvData(std::string information) {
    struct waypoint waypoint;
    std::string missionArray[6];
    std::string mission;
    std::stringstream Str;
    int missionIndex = 0;
    for (int i = 0; i < information.size(); i++) {
        if (information[i] == ';' || information[i] == ',') {
            missionArray[missionIndex] = mission;
            mission = "";
            missionIndex++;
        } else
            mission = mission + information[i];
    }

    /*for(int i = 0; i<4;i++){
        std::cout<<missionArray[i]<<std::endl;
    }*/

    waypoint.typ = missionArray[0];
    Str << missionArray[1];
    Str >> waypoint.latitude;
    Str.clear();
    Str << missionArray[2];
    Str >> waypoint.longitude;
    Str.clear();
    Str << missionArray[3];
    Str >> waypoint.height;
    Str.clear();
    Str << missionArray[4];
    Str >> waypoint.charge;
    Str.clear();
    Str << missionArray[5];
    Str >> waypoint.speed;
    Str.clear();

    /*waypoint.typ = missionArray[0];
    waypoint.latitude = atof(missionArray[1].c_str());
    waypoint.longitude = atof(missionArray[2].c_str());
    waypoint.height = atof(missionArray[3].c_str());*/

    //std::cout<<waypoint.longitude<<std::endl;

    return waypoint;
}

/*parsedPicture Drohne::parsePicture(char picture[], int position ,int length) { 
    struct parsedPicture parsedPicture;

    std::string sPicture;
    std::string sPosition;
    std::stringstream pictureStr;
    std::stringstream positionStr;
    for(int i = 0;i<length;i++){
        sPicture = sPicture+picture[i]; 
    }
    positionStr << position;
    sPosition = positionStr.str();

    //parsedPicture.pictureString = "picture;" + sPicture + ";" + sPosition + ";" + "end";
    //parsedPicture.length = length+strlen(sPosition.c_str())+13;
    return parsedPicture;

}*/

cameraStatus Network::parseRecvToggel(std::string toggelCommand) {
    struct cameraStatus cameraStatus;
    std::string toggelArray[2];
    std::string toggel;
    std::stringstream Str;
    int missionIndex = 0;
    for (int i = 0; i < toggelCommand.size(); i++) {
        if (toggelCommand[i] == ';') {
            toggelArray[missionIndex] = toggel;
            toggel = "";
            missionIndex++;
        } else
            toggel = toggel + toggelCommand[i];
    }

    Str << toggelArray[0];
    Str >> cameraStatus.cameraLeft;
    Str.clear();
    Str << toggelArray[1];
    Str >> cameraStatus.cameraRight;
    Str.clear();



    return cameraStatus;

}

std::string Network::checkDataTyp(std::string information) {
    std::string typ;
    std::string checkMission;
    int i = 0;
    while (information[i] != ';') {
        typ = typ + information[i];
        i++;
    }

    if (typ == "mission") {
        i++;
        while (information[i] != ';' && checkMission != "start") {
            checkMission = checkMission + information[i];
            i++;
        }
        if (checkMission == "start")
            return "start";

    }

    return typ;
}

int Network::recvMission() {
    int result = -1;
    int new_socket;
    char buf[512];


    result = recv(socketRecv_, buf, 512, 0);
    if (result == -1)
        printf("Verbindung mit dem Server (%s) konnte nicht hergestellt werden\n", inet_ntoa(server_addr_recv_.sin_addr));
    else {
        std::string information(buf);
        std::string typ = checkDataTyp(information);
        if (typ == "start")
            return -2;
        if (typ == "mission") {
            struct waypoint waypoint = parseRecvData(information);
            pthread_mutex_lock(&waypointMutex_);
            waypoints_.push_back(waypoint);
            pthread_mutex_unlock(&waypointMutex_);
        }
        if (typ == "toggel") {
            pthread_mutex_lock(&toggelMutex_);
            cameraStatus_ = parseRecvToggel(information);
            pthread_mutex_unlock(&toggelMutex_);
        }
    }



    /*for(int i = 0; i<512;i++ ){
        std::cout<<buf[i]<<std::endl;
    }*/

    return result;
}

int Network::sendDataInformation(std::string information) {
    int result = -1;


    result = send(socketSendInformation_, information.c_str(), strlen(information.c_str()), 0);
    if (result < 0)
        printf("Verbindung mit dem Server (%s) konnte nicht hergestellt werden\n", inet_ntoa(server_addr_sendInformation_.sin_addr));




    return result;
}

int Network::sendDataPicture(pictureStruct picture) {
    int result = -1;
    std::string strLength;
    std::stringstream Str;
    Str<<picture.length;
    strLength = Str.str();
    Str.clear();
    Str<<picture.picture;
    std::string a = Str.str();

    
    result = send(socketSendPicture_, strLength.c_str(), strlen(strLength.c_str()), 0);
    if (result < 0)
        printf("Verbindung mit dem Server (%s) konnte nicht hergestellt werden\n", inet_ntoa(server_addr_sendPicture_.sin_addr));

    result = send(socketSendPicture_, a.c_str(), picture.length, 0);
    if (result < 0)
        printf("Verbindung mit dem Server (%s) konnte nicht hergestellt werden\n", inet_ntoa(server_addr_sendPicture_.sin_addr));

    return result;
}

void *threadFuncSendDataInformation(void *p) {
    DrohneWrapperClass* wrapper = (DrohneWrapperClass*) p;
    Network* drohne = wrapper->myDrohne_;
    std::string information = wrapper->information_;
    int result = -1;

    result = drohne->sendDataInformation(information);
}

void *threadFuncSendDataPicture(void *p) {
    DrohneWrapperClass* wrapper = (DrohneWrapperClass*) p;
    Network* drohne = wrapper->myDrohne_;
    struct pictureStruct picture = wrapper->picture_;
    int result = -1;

    result = drohne->sendDataPicture(picture);
}

void * threadFuncRecvData(void *p) {
    DrohneWrapperClass* wrapper = (DrohneWrapperClass*) p;
    Network* drohne = wrapper->myDrohne_;
    int result = -1;

    result = drohne->recvMission();

    //struct waypoint waypoint{"test",1,1,1,1,1};
    //drohne->waypoints_.push_back(waypoint);

}

std::vector<struct waypoint> Network::getWaypoints() {
    std::vector<struct waypoint> waypointTmp = waypoints_;
    waypoints_.clear();
    return waypointTmp;
}

int Network::getCameraStatus() {
    int status;

    if((cameraStatus_.cameraLeft == false) && (cameraStatus_.cameraRight == false))
        status = 0;
    if((cameraStatus_.cameraLeft == true) && (cameraStatus_.cameraRight == false))
        status = 1;
    if((cameraStatus_.cameraLeft == false) && (cameraStatus_.cameraRight == true))
        status = 2;
    if((cameraStatus_.cameraLeft == true) && (cameraStatus_.cameraRight == true))
        status = 3;
    
    
    return status;
}

pictureStruct Network::readPicture(std::string picturePath, int position) {
    int length;
    FILE* image;
    image = fopen(picturePath.c_str(), "rb");
    if (image != NULL) {
        fseek(image, 0, SEEK_END);
        length = ftell(image);
        fseek(image, 0, SEEK_SET);
    }
    char buffer[length];
    fread(buffer, sizeof (char), length, image);

    fclose(image);

    std::string pictureTemp;
    struct pictureStruct pictureStruct;
    for (int i = 0; i < length; i++) {
        pictureTemp = pictureTemp + buffer[i];
    }
    pictureStruct.picture = pictureTemp;
    pictureStruct.length = length;
    //struct parsedPicture parsedPciture = parsePicture(buffer, position, length);

    return pictureStruct;

}

void Network::threadSendDataInformation(std::string information) {
    pthread_t sendDataInformation;
    pthread_create(&sendDataInformation, NULL, threadFuncSendDataInformation, new DrohneWrapperClass(this, information));
}

void Network::threadSendDataPicture(pictureStruct picture) {
    pthread_t sendDataPicture;
    pthread_create(&sendDataPicture, NULL, threadFuncSendDataPicture, new DrohneWrapperClass(this, picture));
}

void Network::threadRecvData() {
    pthread_t recvData;
    pthread_create(&recvData, NULL, threadFuncRecvData, new DrohneWrapperClass(this, ""));
}

DrohneWrapperClass::DrohneWrapperClass(Network* drohne, std::string information) {
    this->myDrohne_ = drohne;
    this->information_ = information;
}

DrohneWrapperClass::DrohneWrapperClass(Network* drohne, pictureStruct picture) {
    this->myDrohne_ = drohne;
    this->picture_ = picture;
}

DrohneWrapperClass::DrohneWrapperClass(const DrohneWrapperClass& orig) {
}

DrohneWrapperClass::~DrohneWrapperClass() {
}

/*int Drohne::sendMission(std::string information){
    int result = -1;

    
    result = send(socketSend_, information.c_str(), strlen(information.c_str()), 0);
    if (result < 0)
        printf("Verbindung mit dem Server (%s) konnte nicht hergestellt werden\n", inet_ntoa(server_addr_send_.sin_addr));
    
    return result;
}

int Drohne::sendPicture(std::string picture){
    int result = -1;
    int length = strlen(picture.c_str());
     
    result = send(socketSend_, &length, 4, 0);
    if (result < 0)
        printf("Verbindung mit dem Server (%s) konnte nicht hergestellt werden\n", inet_ntoa(server_addr_send_.sin_addr));
    
    result = send(socketSend_, picture.c_str(), length, 0);
    if (result < 0)
        printf("Verbindung mit dem Server (%s) konnte nicht hergestellt werden\n", inet_ntoa(server_addr_send_.sin_addr));
    
    return result;
   
}*/

/*FILE* image;
image = fopen("FuBK-Testbild.png","rb");
if (image != NULL) {
    fseek(image, 0, SEEK_END);
    length = ftell(image);
    fseek(image, 0, SEEK_SET);
}
char buffer[length];
fread(buffer, sizeof(char),length,image);
 */
/*FILE * pFile;
pFile = fopen ("myfile.png", "wb");
fwrite(buffer,sizeof(char),length,pFile);
fclose (pFile);*/


