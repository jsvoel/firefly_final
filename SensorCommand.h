/* 
 * File:   SensorCommand.h
 * Author: johannes
 *
 * Created on 13. Mai 2015, 03:12
 */

#ifndef SENSORCOMMAND_H
#define	SENSORCOMMAND_H

#include <stdexcept>
#include <sstream>

#define PD_LLSTATUS 0x02
#define PD_IMUCALCDATA 0x03
#define PD_GPSDATA 0x23
#define PD_CURRENTWAY 0x21
#define RQ_LLSTATUS 0x0001
#define RQ_IMUCALCDATA 0x0004
#define RQ_GPSDATA 0x0080
#define RQ_CURRENTWAY 0x0100

typedef struct _POLL_REQUEST {
    char string[4]; // always initialize with ">*>p"
    unsigned short packets;
} __attribute__((packed)) POLL_REQUEST;

typedef struct _POLL_HEADER {
    char startstring[3];
    unsigned short length;
    unsigned char packet_desc;
} __attribute__((packed)) POLL_HEADER;

typedef struct _POLL_FOOTER {
    unsigned short crc16;
    char stopstring[3];
} __attribute__((packed)) POLL_FOOTER;

class SensorCommand {
public:
    SensorCommand(const char* description, int answersize, char descriptor, unsigned short packetbit);
    virtual ~SensorCommand();

    virtual void execute();
protected:

    char* getAnswer() {
        return &answerbuffer_[6];
    }
private:
    unsigned short crc_update(unsigned short crc, unsigned char data);
    unsigned short crc16(void* data, unsigned short cnt);

    POLL_HEADER* getHeader() {
        return (POLL_HEADER*)&answerbuffer_[0];
    }

    POLL_FOOTER* getFooter() {
        return (POLL_FOOTER*)&answerbuffer_[answersize_ - 5];
    }

    friend std::ostream& operator<<(std::ostream& os, SensorCommand& sc) {
        for (int i = 0; i < sc.answersize_; ++i) {
            os << (int) sc.answerbuffer_[i] << ' ';
        }
        os << std::endl;
        return os;
    }

    const char* commanddescription_;
    POLL_REQUEST request_;
    char* answerbuffer_;
    int answersize_;
    char descriptor_;
};

inline unsigned short SensorCommand::crc_update(unsigned short crc, unsigned char data) {
    data ^= (crc & 0xff);
    data ^= data << 4;
    return ((((unsigned short) data << 8) | ((crc >> 8)&0xff))
            ^ (unsigned char) (data >> 4) ^ ((unsigned short) data << 3));
}

typedef struct _GPS_DATA {
    //latitude/longitude in deg * 10^7
    int latitude;
    int longitude;
    //GPS height in mm
    int height;
    //speed in x (E/W) and y(N/S) in mm/s
    int speed_x;
    int speed_y;
    //GPS heading in deg * 1000
    int heading;
    //accuracy estimates in mm and mm/s
    unsigned int horizontal_accuracy;
    unsigned int vertical_accuracy;
    unsigned int speed_accuracy;
    //number of satellite vehicles used in NAV solution
    unsigned int numSV;
    // GPS status information; 0x03 = valid GPS fix
    int status;
} __attribute__((packed)) GPS_DATA;

class GPSDataCommand : public SensorCommand {
public:

    GPSDataCommand() : SensorCommand("GPS Data Request", sizeof (GPS_DATA), PD_GPSDATA, RQ_GPSDATA) {
    }

    GPS_DATA* getData() {
        return (GPS_DATA*) getAnswer();
    }
};

typedef struct _IMU_CALCDATA {
    //angles derived by integration of gyro_outputs, drift compensated by data & !fusion; -90000..+90000 pitch(nick) and roll, 0..360000 yaw; 1000 = 1 & ! degree
    int angle_nick;
    int angle_roll;
    int angle_yaw;
    //angular velocities, raw values 16 bit but bias free
    int angvel_nick;
    int angvel_roll;
    int angvel_yaw;
    //acc-sensor outputs, calibrated: -10000..+10000 = -1g..+1g
    short acc_x_calib;
    short acc_y_calib;
    short acc_z_calib;
    //horizontal / vertical accelerations: -10000..+10000 = -1g..+1g
    short acc_x;
    short acc_y;
    short acc_z;
    //reference angles derived by accelerations only: -90000..+90000; 1000 = 1 & !degree
    int acc_angle_nick;
    int acc_angle_roll;
    //total acceleration measured (10000 = 1g)
    int acc_absolute_value;
    //magnetic field sensors output, offset free and scaled; units not & !determined, as only the direction of the field vector is taken into & ! account
    int Hx;
    int Hy;
    int Hz;
    //compass reading: angle reference for angle_yaw: 0..360000; 1000 = 1 degree
    int mag_heading;
    //pseudo speed measurements: integrated accelerations, pulled towards zero; & !units unknown; used for short-term position stabilization
    int speed_x;
    int speed_y;
    int speed_z;
    //height in mm (after data fusion)
    int height;
    //diff. height in mm/s (after data fusion)
    int dheight;
    //diff. height measured by the pressure sensor mm/s
    int dheight_reference;
    //height measured by the pressure sensor mm
    int height_reference;
} __attribute__((packed)) IMU_CALCDATA;

class IMUDataCommand : public SensorCommand {
public:

    IMUDataCommand() : SensorCommand("IMU Data Request", sizeof (IMU_CALCDATA), PD_IMUCALCDATA, RQ_IMUCALCDATA) {
    }

    IMU_CALCDATA* getData() {
        return (IMU_CALCDATA*) getAnswer();
    }
};

typedef struct _LL_STATUS {
    //battery voltages in mV
    short battery_voltage_1;
    short battery_voltage_2;
    //don't care
    short status;
    //Controller cycles per second (should be ?1000)
    short cpu_load;
    //don't care
    char compass_enabled;
    char chksum_error;
    char flying;
    char motors_on;
    short flightMode;
    //Time motors are turning
    short up_time;
} __attribute__((packed)) LL_STATUS;

class LLStatusCommand : public SensorCommand {
public:

    LLStatusCommand() : SensorCommand("LL Status Command", sizeof (LL_STATUS), PD_LLSTATUS, RQ_LLSTATUS) {
    }

    LL_STATUS* getData() {
        return (LL_STATUS*) getAnswer();
    }
};

typedef struct _CURRENT_WAY {
    // the only relevant values are navigation_status and distance_to_wp
    unsigned char dummy1;
    unsigned char properties;
    //total number of waypoints uploaded to the vehicle
    unsigned short nr_of_wp;
    //the waypoint to be reached next
    unsigned char current_wp;
    //memory location of the current wp (0 .. nr_of_wp)
    unsigned char current_wp_memlocation;
    //status of waypoint engine: 0: STANDBY, 1: Executing way from Flash, 2: PC controlled waypoint navigation
    unsigned char status;
    unsigned char dummy2;
    //see WP_NAVSTAT_... defines
    unsigned short navigation_status;
    //distance to WP in dm
    unsigned short distance_to_wp;
} __attribute__((packed)) CURRENT_WAY;

#define WP_NAVSTAT_REACHED_POS      0x01 //vehicle has entered a radius of WAYPOINT.pos_acc and time to stay is not neccessarily over
#define WP_NAVSTAT_REACHED_POS_TIME 0x02 //vehicle is within a radius of WAYPOINT.pos_acc and time to stay is over
#define WP_NAVSTAT_20M              0x04 //vehicle within a 20m radius of the waypoint
#define WP_NAVSTAT_PILOT_ABORT      0x08 //waypoint navigation aborted by safety pilot
#define WP_STATUS_STANDBY 0
#define WP_STATUS_FLASHCONTROLLED 1
#define WP_STATUS_PCCONTROLLED 2

class CurrentWayCommand : public SensorCommand {
public:

    CurrentWayCommand() : SensorCommand("Current Way COmmand", sizeof (CURRENT_WAY), PD_CURRENTWAY, RQ_CURRENTWAY) {
    }

    CURRENT_WAY* getData() {
        return (CURRENT_WAY*) getAnswer();
    }
};

#endif	/* SENSORCOMMAND_H */
