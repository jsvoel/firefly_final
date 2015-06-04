/* 
 * File:   NavigationCommand.h
 * Author: johannes
 *
 * Created on 13. Mai 2015, 03:12
 */

#ifndef NAVIGATIONCOMMAND_H
#define	NAVIGATIONCOMMAND_H

#include <stdexcept>
#include <sstream>

class Comport;

typedef struct _WP_COMMAND {
    char string[4]; // always initialize with ">*>w"
    char cmd; // s, g, l, e or h
} __attribute__((packed)) WP_COMMAND;

typedef struct _WP_HEADER {
    char startstring[2]; // should be ">a"
    unsigned char packet_desc;
} __attribute__((packed)) WP_HEADER;

typedef struct _WP_FOOTER {
    char stopstring[2]; // should be "a<"
} __attribute__((packed)) WP_FOOTER;

typedef struct _WP_ACK {
    WP_HEADER header;
    WP_FOOTER footer;
} __attribute__((packed)) WP_ACK;

#define PD_SINGLEWAYPOINT 0x24
#define PD_GOTOCOMMAND 0x25
#define PD_LAUNCHCOMMAND 0x26
#define PD_LANDCOMMAND 0x27
#define PD_HOMECOMMAND 0x28

class NavigationCommand {
public:
    NavigationCommand(const char* description, char request, char descriptor);
    virtual ~NavigationCommand();

    virtual void execute();
protected:
    char* request_;
private:

    WP_COMMAND* getCommand() {
        return (WP_COMMAND*) request_;
    }

    const char* commanddescription_;
    int requestsize_;
    char descriptor_;
};

class GotoCommand : public NavigationCommand {
public:

    GotoCommand() : NavigationCommand("Goto Command", 'g', PD_GOTOCOMMAND) {
    }
};

class LaunchCommand : public NavigationCommand {
public:

    LaunchCommand() : NavigationCommand("Launch Command", 'l', PD_LAUNCHCOMMAND) {
    }
};

class EndCommand : public NavigationCommand {
public:

    EndCommand() : NavigationCommand("End Flight Command", 'e', PD_LANDCOMMAND) {
    }
};

class HomeCommand : public NavigationCommand {
public:

    HomeCommand() : NavigationCommand("Come Home Command", 'h', PD_HOMECOMMAND) {
    }
};


typedef struct _WAYPOINT {
    //always set to 1
    unsigned char wp_number;
    //don't care
    unsigned char dummy_1;
    unsigned short dummy_2;
    //see WPPROP defines below
    unsigned char properties;
    //max. speed to travel to waypoint in % (default 100)
    unsigned char max_speed;
    //time to stay at a waypoint (XYZ) in 1/100th s
    unsigned short time;
    //position accuracy to consider a waypoint reached in mm (default: 2500 (= 2.5 m))
    unsigned short pos_acc;
    //chksum = 0xAAAA + wp.yaw + wp.height + wp.time + wp.X + wp.Y + wp.max_speed +	wp.pos_acc + wp.properties + wp.wp_number;
    short chksum;
    //waypoint coordinates in mm	// longitude in abs coords
    int X;
    //waypoint coordinates in mm	// latitude in abs coords
    int Y;
    //Desired heading at waypoint
    int yaw;
    //height over 0 reference in mm
    int height;
} __attribute__((packed)) WAYPOINT;

#define WPPROP_ABSCOORDS 0x01 //if set waypoint is interpreted as absolute coordinates, else relative coords
#define WPPROP_HEIGHTENABLED 0x02 //set new height at waypoint
#define WPPROP_YAWENABLED 0x04 //set new yaw-angle at waypoint(not yet implemented)
#define WPPROP_AUTOMATICGOTO 0x10 //if set, vehicle will not wait for a goto command, but goto this waypoint directly

class WaypointCommand : public NavigationCommand {
public:
    WaypointCommand();

    virtual void execute();
    
    void setAbsolute(int latitude, int longitude, int height);
    void setRelative(int X, int Y, int height);
    void setSpeed(unsigned char speed);
    void setWaittime(unsigned short sec);
private:

    WAYPOINT* getWP() {
        return (WAYPOINT*)&request_[sizeof (WP_COMMAND)];
    }
    void checksum();
};

#endif	/* NAVIGATIONCOMMAND_H */
