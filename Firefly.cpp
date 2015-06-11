/* 
 * File:   Firefly.cpp
 * Author: johannes
 * 
 * Created on 27. April 2015, 12:27
 */

#include <cmath> // needef for sqrt calculate the speed vector in sendData()
#include <iomanip> // used by placeholder sendData()

#include "Firefly.h"
#include "NavigationCommand.h"
#include "SensorCommand.h"
#include "RouteStrategy.h"
#include "WaypointIpad.h"

Firefly* Firefly::instance_ = 0;

Firefly::Firefly()
: comport_(COMPORT, BAUDRATE) {
    strategy_ = new RouteStrategy();
    network_.setIpadress(IPADDRESS_HOST);
    
    timespec reqt, remt;
    reqt.tv_sec = 1;
    reqt.tv_nsec = 0;
    // don't do anything unless we can get our Network up and running
    while(network_.initSocketRecv() == -1){
        nanosleep(&reqt, &remt);
    }
    while(network_.initSocketSendInformation() == -1){
        nanosleep(&reqt, &remt);
    }
}

Firefly::~Firefly() {
}

Firefly* Firefly::getInstance() {
    if (instance_ == 0) {
        try {
            instance_ = new Firefly();
        } catch (boost::system::system_error &e) {
            std::cerr << "Boost Exception on creating comport!" << std::endl << e.what() << std::endl;
        }
        if (instance_ == 0) {
            std::cerr << "Couldn't create instance of Firefly" << std::endl;
        }
    }
    return instance_;
}

void Firefly::go(){
    while(true){
        int status = network_.recvMission();
        if(status == -2){ // received complete mission, lets go!
            clearRoute(); // delete the last Mission
            std::vector<waypoint> vwp = network_.getWaypoints(); // get the waypoint vector. waypoint defined in Network.h
            for(int i = 0; i < vwp.size(); ++i){
                // convert network waypoint to IpadWaypoint and add them to the Firefly waypoint Container
                pushWaypoint(WaypointIpad(vwp[i].latitude, vwp[i].longitude, vwp[i].height, vwp[i].speed));
            }
            // get Mission type and add the specific strategy
            //this->setRouteStrategy("Whatever");
            // start the whole thing up
            start();
            // we are done, send the the Ipad a note that the mission is over and wait for the next mission
            // network_.sendDataInformation("missioncomplete");
        }
    }
}

void Firefly::start() {
    GPSDataCommand gps; // Poll the GPS to send information about the drones current location and speed
    LLStatusCommand lls; // Poll the LL Processor to send the battery voltage
    CurrentWayCommand cwc; // Poll the current navigation status to exectue routing
    GPS_DATA* pgps = gps.getData();
    LL_STATUS* plls = lls.getData();
    CURRENT_WAY* pcwc = cwc.getData();
    bool reached_pos = false;

    // retry starting until it works. Dont want to mess a single flipped bit on startup to mess up everything
    while (true) {
        try {
            strategy_->onStart();
            break;
        } catch (std::runtime_error &e) {
            std::cerr << "Error on start: " << e.what() << std::endl;
        }
    }

    // Main Navigation / Data send Loop
    // loop until done with the Navigation of this Route
    while (!strategy_->completed()) {
        try {
            // Poll all the data that is send to the operator
            gps.execute();
            lls.execute();
            // Poll the current way point in order to control Navigation
            cwc.execute();
            // send the updated data
            sendData(pgps->latitude, pgps->longitude, pgps->height, pgps->speed_x, pgps->speed_y,
                    plls->battery_voltage_1, pcwc->navigation_status);

            // check if the operator wants to abort the navigation
            // don't leave the navigation loop unless RouteStrategy says so
            if (checkAbort()) {
                strategy_->onAbort();
            }

            // check if we reached the current waypoint, call only once before time is reached
            if (pcwc->navigation_status & WP_NAVSTAT_REACHED_POS && !reached_pos) {
                strategy_->onReachWP();
                // make sure we call this only once
                reached_pos = true;
            }
            // check if we reached the current waypoint and stayed the time
            if (pcwc->navigation_status & WP_NAVSTAT_REACHED_POS_TIME) {
                strategy_->onTime();
                // make sure we can call onReachWP() again for the next Waypoint
                reached_pos = false;
            }
        } catch (std::runtime_error &e) {
            std::cerr << "Error durring navigation loop: " << e.what() << std::endl;
        }
    }
    // Leaving the navigation routine, maybe RouteStrategy has some clean up to do
    strategy_->onLeave();
}

void Firefly::sendData(int latitude, int longitude, int height, int speed_x, int speed_y, short voltage, short navstat) {
    // calculate the speed from the vector
    double dsx = speed_x, dsy = speed_y, speed = 0.0;
    dsx = dsx * dsx;
    dsy = dsy * dsy;
    speed = sqrt(dsx + dsy);
    speed = speed * 0.0036; // we want km/h not mm/s
    
    double lat = double(latitude) / 10000000.0;    
    double lon = double(longitude) / 10000000.0;
    double chg = double(voltage) / 1000.0;
    double hei = double(height);
    
    network_.sendDataInformation(network_.parseSend("",lat, lon, hei, chg, speed));
}

bool Firefly::checkAbort() {
    //TODO put actual net code here that can check if the flight shall be aborted
    return false;
}

void Firefly::clearRoute() {
    waypoints_.clear();
}

void Firefly::setRouteStrategy(RouteStrategy *rs) {
    if (strategy_ != 0) {
        delete strategy_;
    }
    strategy_ = rs;
}

void Firefly::pushWaypoint(const WaypointIpad& wp) {
    waypoints_.push_back(wp);
}