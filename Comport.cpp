/* 
 * File:   Comport.cpp
 * Author: johannes
 * 
 * Created on 12. Mai 2015, 12:48
 */

#include "Comport.h"

Comport::Comport(const char* port, int baudrate)
: port_(ioser_, port) {
    port_.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
}

Comport::~Comport() {
}

bool Comport::Write(const char* buffer, int size) {
    int index = 0;
#ifdef COMPORT_VERBOSE
    std::cout << "Writing " << size << " bytes." << std::endl;
#endif
    try {
        while (index < size) {
            index += port_.write_some(boost::asio::buffer(&buffer[index], size - index));
#ifdef COMPORT_VERBOSE
            std::cout << "Wrote total of: " << index << " bytes." << std::endl;
#endif
        }
    } catch (boost::system::system_error &e) {
        std::cerr << "Boost Exception on write some!" << std::endl << e.what() << std::endl;
    }

    return index == size;
}

int Comport::Read(char* buffer, int size, int timeout, int retrycount) {
    int index = 0;
    timespec reqt, remt;

    reqt.tv_sec = 0;
    reqt.tv_nsec = timeout * 1000 * 1000;
#ifdef COMPORT_VERBOSE
    std::cout << "Reading " << size << " bytes." << std::endl;
#endif
    try {
        do {
            nanosleep(&reqt, &remt);
            index += port_.read_some(boost::asio::buffer(&buffer[index], size - index));
#ifdef COMPORT_VERBOSE
            std::cout << "Read total of: " << index << " bytes." << std::endl;
#endif
            retrycount--;
        } while (index < size && retrycount > 0);
    } catch (boost::system::system_error &e) {
        std::cerr << "Boost Exception on read some!" << std::endl << e.what() << std::endl;
    }
    return index;
}

void Comport::clear(){
    ::tcflush(port_.handle(), TCIOFLUSH); // flush read and write buffers
}
