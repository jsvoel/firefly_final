/* 
 * File:   SensorCommand.cpp
 * Author: johannes
 * 
 * Created on 13. Mai 2015, 03:12
 */

#include "SensorCommand.h"
#include "Firefly.h"

SensorCommand::SensorCommand(const char* description, int answersize, char descriptor, unsigned short packetbit) {
    commanddescription_ = description;
    request_.string[0] = '>';
    request_.string[1] = '*';
    request_.string[2] = '>';
    request_.string[3] = 'p';
    request_.packets = packetbit;
    answersize_ = sizeof (POLL_HEADER) + answersize + sizeof (POLL_FOOTER);
    answerbuffer_ = new char[answersize_];
    descriptor_ = descriptor;
}

SensorCommand::~SensorCommand() {
    delete[] answerbuffer_;
}

void SensorCommand::execute() {
    Comport* pcom = Firefly::getInstance()->getComport();

    if (pcom->Write((char*) &request_, 6) == false) {
        std::stringstream ss;
        ss << "Couldn't write command request of: " << commanddescription_ << std::endl;
        throw std::runtime_error(ss.str());
    }

    if (pcom->Read(answerbuffer_, answersize_, 10, 10) != answersize_) {
        std::stringstream ss;
        ss << "Answer incomplete of: " << commanddescription_ << std::endl;
        ss << (*this);
        throw std::runtime_error(ss.str());
    }

    if (getHeader()->length != (answersize_ - sizeof (POLL_HEADER) - sizeof (POLL_FOOTER))) {
        std::stringstream ss;
        ss << "Answer length inccorect of: " << commanddescription_ << std::endl;
        ss << (*this);
        throw std::runtime_error(ss.str());
    }

    if (getHeader()->packet_desc != descriptor_) {
        std::stringstream ss;
        ss << "Answer descriptor inccorect of: " << commanddescription_ << std::endl;
        ss << (*this);
        throw std::runtime_error(ss.str());
    }

    if (getFooter()->crc16 != crc16(getAnswer(), (answersize_ - sizeof (POLL_HEADER) - sizeof (POLL_FOOTER)))) {
        std::stringstream ss;
        ss << "Answer crc inccorect of: " << commanddescription_ << std::endl;
        ss << (*this);
        throw std::runtime_error(ss.str());
    }
}

unsigned short SensorCommand::crc16(void* data, unsigned short cnt) {
    unsigned short crc = 0xff;
    unsigned char * ptr = (unsigned char *) data;
    int i;

    for (i = 0; i < cnt; i++) {
        crc = crc_update(crc, *ptr);
        ptr++;
    }
    return crc;
}