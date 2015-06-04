/* 
 * File:   interactive_helpers.h
 * Author: johannes
 *
 * Created on 4. Juni 2015, 14:25
 */

#ifndef INTERACTIVE_HELPERS_H
#define	INTERACTIVE_HELPERS_H

#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <sstream>

#include "SensorCommand.h"
#include "NavigationCommand.h"

inline void sensors() {
    GPSDataCommand gps;
    IMUDataCommand imu;
    LLStatusCommand lls;
    timespec reqt, remt;

    reqt.tv_sec = 1;
    reqt.tv_nsec = 0;

    while (true) {
        try {
            gps.execute();
            if (gps.getData()->status == 0x03) {
                std::cout << "Latitude: " << gps.getData()->latitude << std::endl;
                std::cout << "Longtiude: " << gps.getData()->longitude << std::endl;
            }
            imu.execute();
            std::cout << "Nick: " << imu.getData()->angle_nick << std::endl;
            std::cout << "Roll: " << imu.getData()->angle_roll << std::endl;
            std::cout << "Yaw : " << imu.getData()->angle_yaw << std::endl;
            lls.execute();
            std::cout << "Battery: " << lls.getData()->battery_voltage_1 << std::endl;
            std::cout << "--------------------------------------------------------" << std::endl;
        } catch (std::runtime_error &e) {
            std::cout << "Runtime error: " << e.what() << std::endl;
        }
        nanosleep(&reqt, &remt);
    }
}

inline void waypoints() {
    LaunchCommand ln;
    WaypointCommand wp;
    EndCommand ed;
    timespec reqt, remt;

    reqt.tv_sec = 10;
    reqt.tv_nsec = 0;
    try {
        std::cout << "Launch.." << std::endl;
        ln.execute();

        reqt.tv_sec = 5;
        nanosleep(&reqt, &remt);
        std::cout << "2 Meter.." << std::endl;
        wp.setRelative(0, 0, 2);
        wp.execute();

        nanosleep(&reqt, &remt);
        std::cout << "Land.." << std::endl;
        ed.execute();
    } catch (std::runtime_error &e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    }
}

inline int kbhit(void) {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

inline int readint(){
	int ret = 0;
	char in = '$';
	int size = 0;
	std::stringstream ss;
	while(in != '\n'){
		if(kbhit()){
			in = getch();
			if(size == 0 && in == '-'){
				addch(in);
				ss << in;
				++size;
			} else if(in >= '0' && in <= '9'){
				addch(in);
				ss << in;
				++size;
			}
		}
	}
	if(size > 0){
		ss >> ret;
	}
	return ret;
}

inline void generatewaypoint() {
    char buffer[80];
    std::stringstream ss;
    WaypointCommand wpc;
    int x, y, h;
    char c;
    ss << "\n\rSet Relative Waypoint X: ";
    printw(ss.str().c_str());
    ss.str("");
    ss.clear();
    x = readint();
    
    ss << "\n\rSet Relative Waypoint Y: ";
    printw(ss.str().c_str());
    ss.str("");
    ss.clear();
    y = readint();
    
    ss << "\n\rSet Relative Waypoint H: ";
    printw(ss.str().c_str());
    ss.str("");
    ss.clear();
    h = readint();
    
    ss << "\n\rExecute Waypoint with:\n\rX: " << x << "\n\rY: " << y << "\n\rH: " << h << "\n\rYes (1) or anything else\n\r";
    printw(ss.str().c_str());
    ss.str("");
    ss.clear();
    
    
    if (readint() == 1) {
        ss << "\n\rExecuting!\n\r";
        wpc.setRelative(x, y, h);
        wpc.execute();
    } else {
        ss << "\n\rAborting!\n\r";
    }
    printw(ss.str().c_str());
}

inline void interactive() {
    std::stringstream ss;
    CurrentWayCommand cwc;
    LaunchCommand lnc;
    EndCommand edc;
    GotoCommand gtc;
    HomeCommand hec;
    GPSDataCommand gps;
    const char* statusstring;
    char input = '$';
    double sx, sy, st;

    timespec reqt, remt;

    reqt.tv_sec = 0;
    reqt.tv_nsec = 250 * 1000 * 1000;


    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);

    while (true) {
        try {
            cwc.execute();
            gps.execute();
            clear();
          //  if (gps.getData()->status == 0x03) {
                ss << "Latitude:\t\t" << gps.getData()->latitude << "\n\r"; 
		ss << "Longtitude:\t\t" << gps.getData()->longitude << "\n\r";
                sx = gps.getData()->speed_x;
                sy = gps.getData()->speed_y;
                st = sqrt((sx * sx)+(sy * sy));
                st = st * 0.0036;
                ss << "Speed:\t\t\t" << st << " km/h\n\r";
            //}
            switch (cwc.getData()->status) {
                case 0: statusstring = "Stand By\n\r";
                case 1: statusstring = "Execute from Flash\n\r";
                case 2: statusstring = "PC Controlled\n\r";
                default: statusstring = "undefined\n\r";
            }
            ss << "Navigation status:\t" << statusstring;
            ss << "Reached Waypoint:\t" << ((cwc.getData()->navigation_status & WP_NAVSTAT_REACHED_POS) ? "Yes\n\r" : "NO\n\r");
            ss << "Reached Time:\t\t" << ((cwc.getData()->navigation_status & WP_NAVSTAT_REACHED_POS_TIME) ? "Yes\n\r" : "NO\n\r");
            ss << "Within 20m of WP:\t" << ((cwc.getData()->navigation_status & WP_NAVSTAT_20M) ? "Yes\n\r" : "NO\n\r");
            ss << "Aborted by Pilot:\t" << ((cwc.getData()->navigation_status & WP_NAVSTAT_PILOT_ABORT) ? "Yes\n\r" : "NO\n\r");
	    ss << "Number of Waypoints:\t" << cwc.getData()->nr_of_wp << "\n\r";
            ss << "Next Waypoint:\t\t" << (int)cwc.getData()->current_wp << "\n\r";
            ss << "------------------------------------------------------\n\r";
            ss << "q - Abort Flight\n\rw - New Waypoint\n\re - Launch\n\rr - Goto Next\n\rt - Come Home\n\rInput: ";

            printw(ss.str().c_str());
	    ss.str("");
            ss.clear();
            reqt.tv_sec = 0;
            if (kbhit()) {
                reqt.tv_sec = 2;
                input = getch();
                ss << input << "\n\rExecuting ";
                switch (input) {
                    case 'q': ss << "End\n\r";
                        edc.execute();
                        break;
                    case 'w':ss << "Waypoint\n\r";
                        generatewaypoint();
                        break;
                    case 'e': ss << "Launch\n\r";
                        lnc.execute();
                        break;
                    case 'r': ss << "Goto\n\r";
                        gtc.execute();
                        break;
                    case 't':ss << "Home\n\r";
                        hec.execute();
                        break;
                    default: ss << "Unknown Command!\n\r";
                }
            }
            printw(ss.str().c_str());
	    ss.str("");
            ss.clear();
            refresh();
            nanosleep(&reqt, &remt);
        } catch (std::runtime_error &e) {
            std::cerr << "Runtime error: " << e.what() << std::endl;
        }
    }
}

#endif	/* INTERACTIVE_HELPERS_H */

