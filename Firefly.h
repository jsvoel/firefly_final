/* 
 * File:   Firefly.h
 * Author: johannes
 *
 * Created on 27. April 2015, 12:27
 */

#ifndef FIREFLY_H
#define	FIREFLY_H

#include <list>
#include <iostream>

#include "Comport.h"
#include "WaypointIpad.h"
#include "Network.h"
#include "FireflyErrors.h"

// Serial Port Magic Numbers //
#define COMPORT "/dev/ttyS3" // the port were the serial is to be opened
#define BAUDRATE 57600 // the baud rate it shall be set to
// Network Socket Magic Numbers //
#define IPADDRESS_HOST "192.168.0.1"

class RouteStrategy;

typedef std::list<WaypointIpad> wpcontainer_t;

class Firefly {
public:
    virtual ~Firefly();
    
    // get the Firefly instance
    static Firefly* getInstance();
    // call go() on it, to start everything. Doesn't return if everything is ok
    void go();
    
    void start(); // start to fly the route using provided Waypoints and Strategy
    void clearRoute(); // clear all waypoints
    void setRouteStrategy(RouteStrategy *rs); // set the RouteStrategy
    void pushWaypoint(const WaypointIpad& wp); // put a Waypoint into the List

    Comport* getComport();
    wpcontainer_t* getWaypoints();
    ErrorLogger* getErrorLogger();
private:
    Firefly();
    // Send the data for drone status visualization
    void sendData(int latitude, int longitude, int height, int speed_x, int speed_y, short voltage, short navstat);
    // Check if the operator wants to abort the route
    bool checkAbort();

    static Firefly *instance_;

    Comport comport_;
    Network network_;
    RouteStrategy *strategy_;
    wpcontainer_t waypoints_;
    ErrorLogger errlog_;
};

inline Comport* Firefly::getComport() {
    return &comport_;
}

inline wpcontainer_t* Firefly::getWaypoints(){
    return &waypoints_;
}

inline ErrorLogger* Firefly::getErrorLogger(){
    return &errlog_;
}
#endif	/* FIREFLY_H */
