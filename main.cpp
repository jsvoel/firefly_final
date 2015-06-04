/* 
 * File:   main.cpp
 * Author: johannes
 *
 * Created on 27. April 2015, 12:26
 */



#include "Firefly.h"
#include "interactive_helpers.h"


/*
 * 
 */


int main(int argc, char** argv) {
    try {
        //sensors();
        //waypoints();
        interactive();
    } catch (std::runtime_error &e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Uncaught Exception" << std::endl;
    }

    return 0;
}
