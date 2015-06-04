/* 
 * File:   RouteStrategy.cpp
 * Author: johannes
 * 
 * Created on 27. April 2015, 12:34
 */

#include "RouteStrategy.h"
#include "WaypointIpad.h"

RouteStrategy::RouteStrategy() {
}

RouteStrategy::~RouteStrategy() {
}

void RouteStrategy::onStart(){
    // initialize the iterators so we know what state of the route we are at
    wpcontainer_t *wps = Firefly::getInstance()->getWaypoints();
    cur_ = wps->begin();
    end_ = wps->end();
    // we are at the start of routing, so can't be completed
    completed_ = false;
    // send the launch command to set our Home Position
    LaunchCommand lnc;
    lnc.execute();
    
    if(cur_ != end_){ // just making sure we acutally have any waypoints
        // get the Navigation rolling by exectuing the first Waypoint
        wpc_.setSpeed((*cur_)->speed);
        wpc_.setAbsolute((*cur_)->getLatitude(), (*cur_)->getLongitude(), (*cur_)->getHeight());
        wpc_.execute();
    }else{
        // dont have waypoints
        onEnd();
    }
}

void RouteStrategy::onEnd(){
    // Standard End of the Navigation is to just land wherever the Drone is at
    EndCommand edc;
    edc.execute();
    // Since we are done, set the completition status
    completed_ = true;
}

void RouteStrategy::onReachWP(){
    // Default Strategy upon reaching the Waypoing, but not completed the time to stay
    // do nothing
}

void RouteStrategy::onTime(){
    // Reached cur_ Waypoint and stayed the time, go to the next one
    ++cur_;
    // First check if we completed all Waypoints, if so, end Navigation
    if(cur_ != end_){ 
        wpc_.setSpeed((*cur_)->speed);
        wpc_.setAbsolute((*cur_)->getLatitude(), (*cur_)->getLongitude(), (*cur_)->getHeight());
        wpc_.execute();
    }else{
        // dont have waypoints
        onEnd();
    }
}

void RouteStrategy::onAbort(){
    // Standard Abort of the Navigation is to fly home and than Land
    HomeCommand hec;
    hec.execute();
    // Achieve the Landing part by setting the the our cur_ Waypoint to one before end
    // So when the Firefly reaches the Home Point it will Execute onEnd
    // this will probably crash if there are no Waypoints in the List. 
    cur_ = end_;
    --cur_;
}

void RouteStrategy::onLeave(){
    // do nothing
}