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
#include "WaypointIpad.h"

class RouteStrategy {
public:
    RouteStrategy();
    virtual ~RouteStrategy();

    // check if the Routeing is done or not

    virtual bool completed() {
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
// Also increase the height of the flight slightly in order to prevent destruction of the drone

class SpeedFlightStrategy : public RouteStrategy {
public:
    // edit the onStart method to manipulate the Way Point List before navigating it

    virtual void onStart() {
        // edit the persistant Waypoint Command of the base class to have no wait time
        wpc_.setWaittime(0);
        wpcontainer_t::iterator beg = Firefly::getInstance()->getWaypoints()->begin();
        wpcontainer_t::iterator end = Firefly::getInstance()->getWaypoints()->end();
        for (; beg != end; ++beg) {
            beg->speed = 100;
            beg->height += 10.0;
        }
        // execute the regular onStart method
        RouteStrategy::onStart();
    }
};

// Implements pretty much the Standard Route, but instead of landing at the last Waypoint
// the drone simply lands at the home position set during onStart()

class ComeHomeStrategy : public RouteStrategy {
public:

    ComeHomeStrategy() : onwayhome_(false) {
    }
    // edit the onEnd method to not land, but execute the home command instead

    virtual void onEnd() {
        HomeCommand hec;
        hec.execute();
        onwayhome_ = true;
    }
    // edit the on reach method to check if the drone was on the home route
    // if yes, execute the regular onEnd() to set the status of the strategy
    // to completed and land at the current position

    virtual void onReach() {
        if (onwayhome_) {
            RouteStrategy::onEnd();
        }
    }
private:
    bool onwayhome_;
};

// Implements a Strategy to turn around when reaching the last waypoint and fly back to the first
// than land at the first waypoint

class ReverseStrategy : public RouteStrategy {
public:
    // Achieve the strategy by simply copying the waypoints in reverse order
    // and add them at the back of the waypoint container

    virtual void onStart() {
        wpcontainer_t *wpc = Firefly::getInstance()->getWaypoints();
        wpcontainer_t::iterator beg = wpc->begin();
        wpcontainer_t::iterator end = wpc->end();
        for (--end; beg != end; --end) {
            wpc->push_back(*end);
        }
        wpc->push_back(*beg); // push first wp because it didn't get pushed in the loop
        // Execute the regular onStart() to get the drone going
        RouteStrategy::onStart();
    }
};

// Implements a Strategy that when reaching a waypoint, the drone flies a square shape
// dimensions set in the constructor

class SquareStrategy : public RouteStrategy {
public:

    SquareStrategy(int squaresize) : ssize_(squaresize) {
    }
    // Achieve the goal by adding the waypoints of the square in the waypoint list
    // during onStart. Use of relative waypoints makes it an easy task

    virtual void onStart() {
        wpcontainer_t *wpc = Firefly::getInstance()->getWaypoints();
        wpcontainer_t::iterator beg = wpc->begin();
        wpcontainer_t::iterator end = wpc->end();
        for (; beg != end;) {
            wpcontainer_t::iterator start = beg;
            ++beg; // first increment the pointer, because insertion puts elements in front of the iterator
            wpc->insert(beg, WaypointIpad(ssize_ / 2, ssize_ / 2, beg->height, beg->speed, false)); // fly to first corner
            wpc->insert(beg, WaypointIpad(0, -ssize_, beg->height, beg->speed, false)); // fly to second corner
            wpc->insert(beg, WaypointIpad(-ssize_, 0, beg->height, beg->speed, false)); // fly to third corner
            wpc->insert(beg, WaypointIpad(0, ssize_, beg->height, beg->speed, false)); // fly to forth corner
            wpc->insert(beg, WaypointIpad(ssize_, 0, beg->height, beg->speed, false)); // fly back to first corner
            // return back to the start by using an absolute copy of the starting point
            wpc->insert(beg, *start);
        }
        // Execute the regular onStart() to get the drone going
        RouteStrategy::onStart();
    }
private:
    int ssize_;
};

#endif	/* ROUTESTRATEGY_H */

