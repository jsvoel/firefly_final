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
        
        Firefly::getInstance()->go();

    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
