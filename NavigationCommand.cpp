/* 
 * File:   NavigationCommand.cpp
 * Author: johannes
 * 
 * Created on 13. Mai 2015, 03:12
 */

#include "Firefly.h"
#include "NavigationCommand.h"

NavigationCommand::NavigationCommand(const char* description, char request, char descriptor) {
    commanddescription_ = description;
    if (request != 's') {
        requestsize_ = sizeof (WP_COMMAND);
    } else {
        requestsize_ = sizeof (WP_COMMAND) + sizeof (WAYPOINT);
    }
    request_ = new char[requestsize_];
    getCommand()->string[0] = '>';
    getCommand()->string[1] = '*';
    getCommand()->string[2] = '>';
    getCommand()->string[3] = 'w';
    getCommand()->string[4] = request;
    descriptor_ = descriptor;
}

NavigationCommand::~NavigationCommand() {
    delete[] request_;
}

void NavigationCommand::execute() {
    WP_ACK ack;
    Comport* pcom = Firefly::getInstance()->getComport();

    if (pcom->Write(request_, requestsize_) == false) {
        std::stringstream ss;
        ss << "Couldn't write command request of: " << commanddescription_ << std::endl;
        throw std::runtime_error(ss.str());
    }

    if (pcom->Read((char*) &ack, sizeof (WP_ACK), 10, 10) != sizeof (WP_ACK)) {
        std::stringstream ss;
        ss << "Acknowledge incomplete of: " << commanddescription_ << std::endl;
        throw std::runtime_error(ss.str());
    }

    if (ack.header.startstring[0] != '>' || ack.header.startstring[1] != 'a' ||
            ack.header.packet_desc != descriptor_ ||
            ack.footer.stopstring[0] != 'a' || ack.footer.stopstring[1] != '<') {
        std::stringstream ss;
        ss << "Acknowledge incorrect of: " << commanddescription_ << std::endl;
        ss << ack.header.startstring[0] << ack.header.startstring[1] << ack.header.packet_desc
                << ack.footer.stopstring[0] << ack.footer.stopstring[1] << std::endl;
        throw std::runtime_error(ss.str());
    }
}

WaypointCommand::WaypointCommand() : NavigationCommand("Waypoint Command", 's', PD_SINGLEWAYPOINT) {
    WAYPOINT* wp = getWP();
    wp->wp_number = 1;
    wp->properties = WPPROP_HEIGHTENABLED;
    wp->max_speed = 20;
    wp->time = 1000;
    wp->pos_acc = 2500;
    wp->X = 0;
    wp->Y = 0;
    wp->yaw = 0;
    wp->height = 10000;
}

void WaypointCommand::execute(){
    checksum(); // calculate checksum before transmitting the waypoint
    NavigationCommand::execute();
}

void WaypointCommand::setAbsolute(int latitude, int longitude, int height) {
    WAYPOINT* wp = getWP();
    wp->properties = WPPROP_HEIGHTENABLED | WPPROP_ABSCOORDS;
    wp->X = longitude;
    wp->Y = latitude;
    wp->height = height * 1000;
}

void WaypointCommand::setRelative(int X, int Y, int height) {
    WAYPOINT* wp = getWP();
    wp->properties = WPPROP_HEIGHTENABLED;
    wp->X = X * 1000;
    wp->Y = Y * 1000;
    wp->height = height * 1000;
}

void WaypointCommand::setSpeed(unsigned char speed) {
    WAYPOINT* wp = getWP();
    if (speed > 100) {
        speed = 100;
    }
    wp->max_speed = speed;
}

void WaypointCommand::setWaittime(unsigned short sec) {
    WAYPOINT* wp = getWP();
    wp->time = sec * 100;
}

void WaypointCommand::checksum() {
    short chk = 0xAAAA;
    WAYPOINT* wp = getWP();
    chk += wp->yaw + wp->height + wp->time + wp->X + wp->Y + wp->max_speed;
    chk += wp->pos_acc + wp->properties + wp->wp_number;
    wp->chksum = chk;
}