/* 
 * File:   RouteStrategy.h
 * Author: johannes
 *
 * Created on 27. April 2015, 12:34
 */

#ifndef ROUTESTRATEGY_H
#define	ROUTESTRATEGY_H

#include "Firefly.h"
#include "NavigationCommand.h"

class RouteStrategy {
public:
    RouteStrategy();
    virtual ~RouteStrategy();
    
    // check if the Routeing is done or not
    virtual bool completed(){
        return completed_;
    }
    
    // on Start of Navigation this will be called once by Firefly
    virtual void onStart();
    // When RoutingStrategey notices it reached its end it should call this
    virtual void onEnd();
    // Called by Firefly whenever it's status goes to "Reached Waypoint"
    virtual void onReachWP();
    // Called by Firefly whenever it's status goes to "Reached Waypoing and Time"
    virtual void onTime();
    // Called by Firefly when the User Issues an abortion of the Routing Process
    virtual void onAbort();
    // Called by Firefly when the Navigation Loop is left
    virtual void onLeave();
protected:
    wpcontainer_t::iterator cur_, end_;
    WaypointCommand wpc_;
    bool completed_;
};

// Implement a speedflight mode, change all waypoints to max speed no wait time
class SpeedFlightStrategy : public RouteStrategy {
public:
    // edit the onStart method to manipulate the Way Point List before navigating it
    virtual void onStart(){
        // edit the persistant Waypoint Command of the base class to have no wait time
        wpc_.setWaittime(0);
        // c++11 style loop
        for(auto wp : *(Firefly::getInstance()->getWaypoints()))
            wp->speed = 100;
        // execute the regular onStart method
        RouteStrategy::onStart();
    }
};

#endif	/* ROUTESTRATEGY_H */

