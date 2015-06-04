/* 
 * File:   Firefly.cpp
 * Author: johannes
 * 
 * Created on 27. April 2015, 12:27
 */

#include "Firefly.h"
#include "NavigationCommand.h"
#include "SensorCommand.h"

Firefly* Firefly::instance_ = 0;

Firefly::Firefly()
: comport_(COMPORT, BAUDRATE) {
    strategy_ = 0;
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
            std::cout << "Couldn't create instance of Firefly" << std::endl;
        }
    }
    return instance_;
}

void Firefly::start() {
    std::cout << "Fyling " << waypoints_.size() << " Waypoints:" << std::endl;
    for (std::vector<WAYPOINT*>::iterator it(waypoints_.begin()); it != waypoints_.end(); ++it) {
        std::cout << (*it)->Y << " " << (*it)->X << std::endl;
    }
}

void Firefly::stop() {

}

void Firefly::clearRoute() {
    if (strategy_ != 0) {
        delete strategy_;
        strategy_ = 0;
    }
    for (std::vector<WAYPOINT*>::iterator it(waypoints_.begin()); it != waypoints_.end(); ++it) {
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

void Firefly::pushWaypoint(WAYPOINT* wp) {
    waypoints_.push_back(wp);
}