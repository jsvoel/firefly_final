/* 
 * File:   main.cpp
 * Author: johannes
 *
 * Created on 27. April 2015, 12:26
 */



#include "Firefly.h"
#include "WaypointIpad.h"

int main(int argc, char** argv) {
    try {
        Firefly* pfly = Firefly::getInstance();
        
        pfly->pushWaypoint(new WaypointIpad(49.846063, 8.636496));
        pfly->pushWaypoint(new WaypointIpad(49.845989, 8.636873));
        pfly->pushWaypoint(new WaypointIpad(49.845839, 8.636782));
        pfly->pushWaypoint(new WaypointIpad(49.845812, 8.636400));
        pfly->pushWaypoint(new WaypointIpad(49.845645, 8.636434));
        pfly->pushWaypoint(new WaypointIpad(49.845753, 8.636693));
        pfly->pushWaypoint(new WaypointIpad(49.845989, 8.636311));
        
        pfly->start();
        
    } catch (std::runtime_error &e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Uncaught Exception" << std::endl;
    }

    return 0;
}
