/* 
 * File:   Comport.h
 * Author: johannes
 *
 * Created on 12. Mai 2015, 12:48
 */

#ifndef COMPORT_H
#define	COMPORT_H

#include <boost/asio/serial_port.hpp>
#include <time.h>
#include <iostream>

//#define COMPORT_VERBOSE

class Comport {
public:
    Comport(const char* port, int baudrate);
    virtual ~Comport();
    
    bool Write(const char* buffer, int size);
    int Read(char* buffer, int size, int timeout, int retrycount);
    void clear();
private:
    boost::asio::io_service ioser_; //IO Service object to instantiate the serial port
    boost::asio::serial_port port_; //boost serial port
};

#endif	/* COMPORT_H */
