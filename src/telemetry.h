#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_

#define TELEM_ERROR_TIME 5000
#define TELEM_NUM_ALARMS 6

#ifdef MODULAR
    #define HAS_DSM_EXTENDED_TELEMETRY 0
#else
    #define HAS_DSM_EXTENDED_TELEMETRY 1
#endif

enum {
    TELEM_FRSKY,
    TELEM_DEVO,
    TELEM_DSM,
};

enum {
    TELEM_DEVO_VOLT1 = 1,
    TELEM_DEVO_VOLT2,
    TELEM_DEVO_VOLT3,
    TELEM_DEVO_TEMP1,
    TELEM_DEVO_TEMP2,
    TELEM_DEVO_TEMP3,
    TELEM_DEVO_TEMP4,
    TELEM_DEVO_RPM1,
    TELEM_DEVO_RPM2,
    TELEM_DEVO_LAST,
};
enum {
    TELEM_DSM_FLOG_FADESA = 1,
    TELEM_DSM_FLOG_FADESB,
    TELEM_DSM_FLOG_FADESL,
    TELEM_DSM_FLOG_FADESR,
    TELEM_DSM_FLOG_FRAMELOSS,
    TELEM_DSM_FLOG_HOLDS,
    TELEM_DSM_FLOG_VOLT1,
    TELEM_DSM_FLOG_VOLT2,
    TELEM_DSM_FLOG_RPM1,
    TELEM_DSM_FLOG_TEMP1,
    TELEM_DSM_AMPS1,
    TELEM_DSM_PBOX_VOLT1,
    TELEM_DSM_PBOX_VOLT2,
    TELEM_DSM_PBOX_CAPACITY1,
    TELEM_DSM_PBOX_CAPACITY2,
    TELEM_DSM_PBOX_ALARMV1,
    TELEM_DSM_PBOX_ALARMV2,
    TELEM_DSM_PBOX_ALARMC1,
    TELEM_DSM_PBOX_ALARMC2,
    TELEM_DSM_AIRSPEED,
    TELEM_DSM_ALTITUDE,
    TELEM_DSM_ALTITUDE_MAX,
    TELEM_DSM_GFORCE_X,
    TELEM_DSM_GFORCE_Y,
    TELEM_DSM_GFORCE_Z,
    TELEM_DSM_GFORCE_XMAX,
    TELEM_DSM_GFORCE_YMAX,
    TELEM_DSM_GFORCE_ZMAX,
    TELEM_DSM_GFORCE_ZMIN,
    TELEM_DSM_JETCAT_STATUS,
    TELEM_DSM_JETCAT_THROTTLE,
    TELEM_DSM_JETCAT_PACKVOLT,
    TELEM_DSM_JETCAT_PUMPVOLT,
    TELEM_DSM_JETCAT_RPM,
    TELEM_DSM_JETCAT_TEMPEGT,
    TELEM_DSM_JETCAT_OFFCOND,
    TELEM_DSM_RXPCAP_AMPS,
    TELEM_DSM_RXPCAP_CAPACITY,
    TELEM_DSM_RXPCAP_VOLT,
    TELEM_DSM_FPCAP_AMPS,
    TELEM_DSM_FPCAP_CAPACITY,
    TELEM_DSM_FPCAP_TEMP,
    TELEM_DSM_VARIO_ALTITUDE,
    TELEM_DSM_VARIO_CLIMBRATE1,
    TELEM_DSM_VARIO_CLIMBRATE2,
    TELEM_DSM_VARIO_CLIMBRATE3,
    TELEM_DSM_VARIO_CLIMBRATE4,
    TELEM_DSM_VARIO_CLIMBRATE5,
    TELEM_DSM_VARIO_CLIMBRATE6,
    TELEM_DSM_HYPOTHETIC_AMPS1,
    TELEM_DSM_HYPOTHETIC_AMPS2,
    TELEM_DSM_HYPOTHETIC_VOLT1,
    TELEM_DSM_HYPOTHETIC_VOLT2,
    TELEM_DSM_HYPOTHETIC_TEMP1,
    TELEM_DSM_HYPOTHETIC_TEMP2,
    TELEM_DSM_HYPOTHETIC_RPM,
    TELEM_DSM_HYPOTHETIC_THROTTLE,
    TELEM_DSM_HYPOTHETIC_OUTPUT,
    TELEM_DSM_LAST,
};

enum {
    TELEM_FRSKY_VOLT1 = 1,
    TELEM_FRSKY_VOLT2,
    TELEM_FRSKY_VOLT3,
    TELEM_FRSKY_RSSI,
    TELEM_FRSKY_TEMP1,
    TELEM_FRSKY_TEMP2,
    TELEM_FRSKY_RPM,
    TELEM_FRSKY_ALTITUDE,
    TELEM_FRSKY_LAST
};
#define TELEM_VALS        (((int)TELEM_DSM_LAST > (int)TELEM_DEVO_LAST)            \
                               ? (((int)TELEM_DSM_LAST > (int)TELEM_FRSKY_LAST)    \
                                   ? (int)TELEM_DSM_LAST : (int)TELEM_FRSKY_LAST)  \
                               : (((int)TELEM_DEVO_LAST > (int)TELEM_FRSKY_LAST)   \
                                   ? (int)TELEM_DEVO_LAST : (int)TELEM_FRSKY_LAST) \
                          )
#define NUM_TELEM   TELEM_VALS - 1
enum {
    TELEM_GPS_LAT = TELEM_VALS,
    TELEM_GPS_LONG,
    TELEM_GPS_ALT,
    TELEM_GPS_SPEED,
    TELEM_GPS_TIME,
    TELEM_GPS_SATCOUNT,
    TELEM_GPS_HEADING,
};
enum {
    TELEMFLAG_ALARM1 = 0x01,
    TELEMFLAG_ALARM2 = 0x02,
    TELEMFLAG_ALARM3 = 0x04,
    TELEMFLAG_ALARM4 = 0x08,
    TELEMFLAG_ALARM5 = 0x10,
    TELEMFLAG_ALARM6 = 0x20,
};

enum {
    TELEMUNIT_FEET   = 0x40,
    TELEMUNIT_FAREN  = 0x80,
};

struct gps {
    s32 latitude;
    s32 longitude;
    s32 altitude;
    u32 velocity;
    u32 time;
    u16 heading;
    u8 satcount;
};

#define TELEM_UPDATE_SIZE (((TELEM_VALS + 7) + 31) / 32)
struct Telemetry {
    struct gps gps;
    u16 value[TELEM_VALS];
    u16 capabilities;
    volatile u32 updated[TELEM_UPDATE_SIZE];
};

enum {
    PROTO_TELEM_UNSUPPORTED = -1,
    PROTO_TELEM_OFF = 0,
    PROTO_TELEM_ON  = 1,
};

extern struct Telemetry Telemetry; 
s32 TELEMETRY_GetValue(int idx);
s32 _TELEMETRY_GetValue(struct Telemetry *t, int idx);
const char * TELEMETRY_GetValueStr(char *str, int idx);
const char * TELEMETRY_GetValueStrByValue(char *str, int idx, s32 value);
const char * TELEMETRY_Name(char *str, int idx);
const char * TELEMETRY_ShortName(char *str, int idx);
s32 TELEMETRY_GetMaxValue(int idx);
s32 TELEMETRY_GetMinValue(int idx);
void TELEMETRY_Alarm();
void TELEMETRY_ResetAlarm(int i);
void TELEMETRY_MuteAlarm();
int TELEMETRY_HasAlarm(int src);
u32 TELEMETRY_IsUpdated(int val);
void TELEMETRY_SetUpdated(int telem);
int TELEMETRY_Type();
void TELEMETRY_SetType(int type);
void TELEMETRY_SetTypeByProtocol(enum Protocols protocol);
int TELEMETRY_GetNumTelemSrc();
#endif
