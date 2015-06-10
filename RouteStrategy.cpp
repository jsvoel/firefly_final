/* 
 * File:   RouteStrategy.cpp
 * Author: johannes
 * 
 * Created on 27. April 2015, 12:34
 */

#include "RouteStrategy.h"

RouteStrategy::RouteStrategy() {
#ifdef ROUTESTRAT_LOGFILE
    logfile_.open(ROUTELOGFILE, std::ios::trunc | std::ios::out); 
#endif
}

RouteStrategy::~RouteStrategy() {
}

void RouteStrategy::onStart() {
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "onStart()...baseclass" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "onStart()...baseclass" << std::endl;
#endif
    // initialize the iterators so we know what state of the route we are at
    cur_ = Firefly::getInstance()->getWaypoints()->begin();
    end_ = Firefly::getInstance()->getWaypoints()->end();
    // we are at the start of routing, so can't be completed
    completed_ = false;
    // send the launch command to set our Home Position
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "Launch command" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "Launch command" << std::endl;
#endif
    LaunchCommand lnc;
    lnc.execute();

    if (cur_ != end_) { // just making sure we acutally have any waypoints
        // get the Navigation rolling by exectuing the first Waypoint
        wpc_.setSpeed(cur_->speed);
        if (cur_->isAbsolute()) // check if the waypoint is absolute or relative type
            wpc_.setAbsolute(cur_->getLatitude(), cur_->getLongitude(), cur_->getHeight());
        else
            wpc_.setRelative(cur_->getX(), cur_->getY(), cur_->getHeight());
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "WP command" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "WP command" << std::endl;
#endif
        wpc_.execute();
    } else {
        // dont have waypoints
        onEnd();
    }
}

void RouteStrategy::onEnd() {
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "onEnd()...baseclass\nEnd command" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "onEnd()...baseclass\nEnd command"<< std::endl;
#endif
    // Standard End of the Navigation is to just land wherever the Drone is at
    EndCommand edc;
    edc.execute();
    // Since we are done, set the completition status
    completed_ = true;
}

void RouteStrategy::onReachWP() {
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "onReachWP()...baseclass" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "onReachWP()...baseclass"<< std::endl;
#endif
    // Default Strategy upon reaching the Waypoing, but not completed the time to stay
    // do nothing
}

void RouteStrategy::onTime() {
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "onTime()...baseclass" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "onTime()...baseclass"<< std::endl;
#endif
    // Reached cur_ Waypoint and stayed the time, go to the next one
    ++cur_;
    // First check if we completed all Waypoints, if so, end Navigation
    if (cur_ != end_) {
        wpc_.setSpeed(cur_->speed);
        if (cur_->isAbsolute()) // check if the waypoint is absolute or relative type
            wpc_.setAbsolute(cur_->getLatitude(), cur_->getLongitude(), cur_->getHeight());
        else
            wpc_.setRelative(cur_->getX(), cur_->getY(), cur_->getHeight());
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "WP command" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "WP command" << std::endl;
#endif
        wpc_.execute();
    } else {
        // dont have waypoints
        onEnd();
    }
}

void RouteStrategy::onAbort() {
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "onAbort()...baseclass\nHome command" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "onAbort()...baseclass\nHome command" << std::endl;
#endif
    // Standard Abort of the Navigation is to fly home and than Land
    HomeCommand hec;
    hec.execute();
    // Achieve the Landing part by setting the the our cur_ Waypoint to one before end
    // So when the Firefly reaches the Home Point it will Execute onEnd
    // this will probably crash if there are no Waypoints in the List. 
    cur_ = end_;
    --cur_;
}

void RouteStrategy::onLeave() {
#ifdef ROUTESTRAT_VERBOSE
    std::cout << "onLeave()...baseclass" << std::endl;
#endif
#ifdef ROUTESTRAT_LOGFILE
    logfile_ << "onLeave()...baseclass" << std::endl;
#endif
    // do nothing
}