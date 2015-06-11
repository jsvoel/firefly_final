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
#include "SensorCommand.h"

int main(int argc, char** argv) {
    try {
        
        Firefly::getInstance()->go();

    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }catch(...){
        std::cerr << "Uncaught Exception: " << std::endl;
    }
    return 0;
}
