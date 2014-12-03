#ifndef _DEF_H_
#define _DEF_H_

#define CONFIG_DIRPATH "/etc/sysconfig/config"
//#define CONFIG_FILEPATH CONFIG_DIRPATH"/fanctl.conf"
#define CONFIG_FILEPATH "./fanctl.conf"

#define CMD_GET_TEMPERATURE "smartctl -a -d sat /dev/sda1 | grep -i temp | awk '{print $10}'"

#define TEMPERATURE_HIGH    49
#define TEMPERATURE_MID     46
#define TEMPERATURE_LOW     43

#define MAX_STRING_SIZE     1<<8

enum FAN_CONTROL_MODE {
    eMode1 = 0,
    eMode2,
    eMode3
};

#endif
