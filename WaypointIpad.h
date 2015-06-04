/* 
 * File:   WaypointIpad.h
 * Author: johannes
 *
 * Created on 4. Juni 2015, 15:04
 */

#ifndef WAYPOINTIPAD_H
#define	WAYPOINTIPAD_H

#include <string>

class WaypointIpad{
public:
    WaypointIpad(double lat, double lon){
        latitude = lat;
        longitude = lon;
        height = 10.0;
        speed = 30;
    }
    
    int getLatitude(){
        return (latitude * 10000000.0); // lat * 10 ^ 7
    }
    
    int getLongitude(){
        return (longitude * 10000000.0); // lon * 10 ^ 7
    }
    
    int getHeight(){
        return (height); // hei * 1
    }
    
    std::string typ;
    double latitude;
    double longitude;
    double height;
    int charge;
    int speed;
};

#endif	/* WAYPOINTIPAD_H */

