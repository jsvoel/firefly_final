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

void Firefly::start() {
    GPSDataCommand gps; // Poll the GPS to send information about the drones current location and speed
    LLStatusCommand lls; // Poll the LL Processor to send the battery voltage
    CurrentWayCommand cwc; // Poll the current navigation status to exectue routing
    GPS_DATA* pgps = gps.getData();
    LL_STATUS* plls = lls.getData();
    CURRENT_WAY* pcwc = cwc.getData();
    bool reached_pos = false;
    
    // retry starting until it works. Dont want to mess a single flipped bit on startup to mess up everything
    while(true){ 
        try{
            strategy_->onStart();
            break;
        }catch(std::runtime_error &e){
            std::cerr << "Error on start: " << e.what() << std::endl;
        }
    }
    
    // Main Navigation / Data send Loop
    // loop until done with the Navigation of this Route
    while(!strategy_->completed()){ 
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
            if(checkAbort()){
                strategy_->onAbort();
            }
            
            // check if we reached the current waypoint, call only once before time is reached
            if(pcwc->navigation_status & WP_NAVSTAT_REACHED_POS && !reached_pos) {
                strategy_->onReachWP();
                // make sure we call this only once
                reached_pos = true;
            }
            // check if we reached the current waypoint and stayed the time
            if(pcwc->navigation_status & WP_NAVSTAT_REACHED_POS_TIME) {
                strategy_->onTime();
                // make sure we can call onReachWP() again for the next Waypoint
                reached_pos = false;
            }
        }catch(std::runtime_error &e){
            std::cerr << "Error durring navigation loop: " << e.what() << std::endl;
        }
    }
    // Leaving the navigation routine, maybe RouteStrategy has some clean up to do
    strategy_->onLeave();
}

void Firefly::stop() {

}

void Firefly::sendData(int latitude, int longitude, int height, int speed_x, int speed_y, short voltage, short navstat){
    //TODO put actual net code here to transmit the data to the operator
    
    // calculate the speed from the vector
    double dsx = speed_x, dsy = speed_y, speed = 0.0;
    dsx = dsx * dsx;
    dsy = dsy * dsy;
    speed = sqrt(dsx + dsy);
    speed = speed * 0.0036; // we want km/h not mm/s
    
    static int count = 0; // dont flood the cout
    if(++count == 5) { // and disply only every 5th status update
        std::cout << "############################################" << std::endl;
        
        std::cout << "NavStat: " << std::hex << navstat << std::dec;
        if(navstat & WP_NAVSTAT_REACHED_POS_TIME)
        {
            std::cout << " Reached Time\n";
        }else if(navstat & WP_NAVSTAT_REACHED_POS ) {
            std::cout << " Reached Waypoint\n";
        }else {
            std::cout << "\n";
        }
            
        std::cout << "Latitu: " << latitude << "\nLongit:  " << longitude << std::endl;
        std::cout << "Height: " << height / 1000 << " m\nSpeed:  " << std::fixed << std::setprecision(2) << speed << " km/h" << std::endl;
        std::cout << "Batter: " << voltage << std::endl;
        count = 0;
    }
}

bool Firefly::checkAbort(){
    //TODO put actual net code here that can check if the flight shall be aborted
    return false;
}

void Firefly::clearRoute() {
    for (wpcontainer_t::iterator it(waypoints_.begin()); it != waypoints_.end(); ++it) {
        delete *it;
    }
    waypoints_.clear();
}

void Firefly::setRouteStrategy(RouteStrategy *rs) {
    if (strategy_ != 0) {
        delete strategy_;
    }
    strategy_ = rs;
}

void Firefly::pushWaypoint(WaypointIpad* wp) {
    waypoints_.push_back(wp);
}