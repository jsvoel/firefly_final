/* 
 * File:   WaypointIpad.h
 * Author: johannes
 *
 * Created on 4. Juni 2015, 15:04
 */

#ifndef WAYPOINTIPAD_H
#define	WAYPOINTIPAD_H

#include <string>

class WaypointIpad {
public:

    WaypointIpad(double lat, double lon, double hei, double spe, bool isAbsolute = true) {
        if (isAbsolute) typ = "a"; // absolute waypoint
        else typ = "r"; // relative waypoint
        latitude = lat;
        longitude = lon;
        height = hei;
        speed = spe;
    }

    int getLatitude() {
        return (latitude * 10000000.0); // lat * 10 ^ 7
    }
    
    int getX() {
        return latitude;
    }
    
    int getY() {
        return longitude;
    }

    int getLongitude() {
        return (longitude * 10000000.0); // lon * 10 ^ 7
    }

    int getHeight() {
        return (height); // hei * 1
    }

    bool isAbsolute() {
        return (typ == "a");
    }

    std::string typ;
    double latitude;
    double longitude;
    double height;
    int charge;
    int speed;
};

#endif	/* WAYPOINTIPAD_H */

