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

#define COMPORT "/dev/ttyS3" // the port were the serial is to be opened
#define BAUDRATE 57600 // the baud rate it shall be set to

class WaypointIpad;
class RouteStrategy;

typedef std::list<WaypointIpad*> wpcontainer_t;

class Firefly {
public:
    virtual ~Firefly();

    static Firefly* getInstance();

    void start(); // start to fly the route using provided Waypoints and Strategy
    void stop(); // stop flight and land instantly

    void clearRoute(); // clear all waypoints
    void setRouteStrategy(RouteStrategy *rs); // set the RouteStrategy
    void pushWaypoint(WaypointIpad* wp); // put a Waypoint into the List

    Comport* getComport();
    wpcontainer_t* getWaypoints();
private:
    Firefly();
    // Send the data for drone status visualization
    void sendData(int latitude, int longitude, int height, int speed_x, int speed_y, short voltage, short navstat);
    // Check if the operator wants to abort the route
    bool checkAbort();

    static Firefly *instance_;

    Comport comport_;
    RouteStrategy *strategy_;
    wpcontainer_t waypoints_;
};

inline Comport* Firefly::getComport() {
    return &comport_;
}

inline wpcontainer_t* Firefly::getWaypoints(){
    return &waypoints_;
}
#endif	/* FIREFLY_H */
