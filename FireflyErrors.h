/* 
 * File:   FireflyErrors.h
 * Author: johannes
 *
 * Created on 11. Juni 2015, 15:46
 */

#ifndef FIREFLYERRORS_H
#define	FIREFLYERRORS_H

#include <stdexcept>
#include <fstream>
#include <ctime>

class FFExceptionNavREQ : public std::runtime_error {
public:
    FFExceptionNavREQ(const char* what) : runtime_error(what){
        
    }
};

class FFExceptionNavACK : public std::runtime_error {
public:
    FFExceptionNavACK(const char* what) : runtime_error(what){
        
    }
};

class FFExceptionSensREQ : public std::runtime_error {
public:
    FFExceptionSensREQ(const char* what) : runtime_error(what){
        
    }
};

class FFExceptionSensLEN : public std::runtime_error {
public:
    FFExceptionSensLEN(const char* what) : runtime_error(what){
        
    }
};

class FFExceptionSensDSC : public std::runtime_error {
public:
    FFExceptionSensDSC(const char* what) : runtime_error(what){
        
    }
};

class FFExceptionSensCRC : public std::runtime_error {
public:
    FFExceptionSensCRC(const char* what) : runtime_error(what){
        
    }
};

class ErrorLogger{
public:
    ErrorLogger(){
        char filename[30];
        time_t tmr = time(0);
        struct tm * ptm = localtime(&tmr);
        strftime(filename, 30, "ErrorLog_%D_%T", ptm);
        logfile_.open(filename, std::ios::out);
    }
    void log(const char* what){
        char errortime[12];
        time_t tmr = time(0);
        struct tm * ptm = localtime(&tmr);
        strftime(errortime, 12, "%T: ", ptm);
        logfile_ << errortime << what << std::endl;
    }
private:
    std::fstream logfile_;
};

#endif	/* FIREFLYERRORS_H */

