/* 
 * File:   main.cpp
 * Author: johannes
 *
 * Created on 27. April 2015, 12:26
 */

#include <iostream>

#include "Firefly.h"
#include "RouteStrategy.h"
#include "WaypointIpad.h"

int main(int argc, char** argv) {
    try {
        Firefly* pfly = Firefly::getInstance();

        pfly->pushWaypoint(WaypointIpad(49.846063, 8.636496, 10, 30));
        pfly->pushWaypoint(WaypointIpad(49.845989, 8.636873, 10, 30));
        pfly->pushWaypoint(WaypointIpad(49.845839, 8.636782, 10, 30));
        pfly->pushWaypoint(WaypointIpad(49.845812, 8.636400, 10, 30));
        pfly->pushWaypoint(WaypointIpad(49.845645, 8.636434, 10, 30));
        pfly->pushWaypoint(WaypointIpad(49.845753, 8.636693, 10, 30));
        pfly->pushWaypoint(WaypointIpad(49.845989, 8.636311, 10, 30));

        std::cout << "What Strategy to use?" << std::endl;
        std::cout << "q - Speed Flight Strategy" << std::endl;
        std::cout << "w - Come Home Strategy" << std::endl;
        std::cout << "e - Reverse Strategy" << std::endl;
        std::cout << "r - Come Square Strategy" << std::endl;
        char in = 0;
        std::cin.get(in);
        if (in == 'q') {
            pfly->setRouteStrategy(new SpeedFlightStrategy());
            std::cout << "Using Speed Mode" << std::endl;
        }else if(in == 'w'){
            pfly->setRouteStrategy(new ComeHomeStrategy());
            std::cout << "Using Home Mode" << std::endl;
        }else if(in == 'e'){
            pfly->setRouteStrategy(new ReverseStrategy());
            std::cout << "Using Reverse Mode" << std::endl;
        }else if(in == 'r'){
            pfly->setRouteStrategy(new SquareStrategy(16));
            std::cout << "Using Square Mode" << std::endl;
        }

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
