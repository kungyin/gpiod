#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "def.h"

using namespace std;

void usage()
{

	fprintf(stderr, "gpiod, "
		"usage:\n"
        //"gpiod add $(filename)\n"
		//"gpiod remove $(filename)\n"
        "\n"    
	);
}

void setFanSpeed(FAN_CONTROL_MODE mode, int iTemperature, bool bIsRising) {
    switch (mode) {
    case eMode1:
        if (bIsRising) {
            if (iTemperature < TEMPERATURE_MID) {
                cout << "mode1: " << bIsRising << " " << iTemperature << " fan off" << endl; 
            }
            else if (iTemperature >= TEMPERATURE_MID && iTemperature < TEMPERATURE_HIGH)
                cout << "mode1: " << bIsRising << " " << iTemperature << " low speed" << endl; 
            else 
                cout << "mode1: " << bIsRising << " " << iTemperature << " high speed" << endl; 
        }
        else {
            //Get GPIO speed
            int iLowSpeed = rand() % 1;
            if (iLowSpeed) {
                if (iTemperature >= TEMPERATURE_LOW) {
                    cout << "mode1: " << bIsRising << " " << iTemperature << " low speed" << endl; 
                }
                else
                    cout << "mode1: " << bIsRising << " " << iTemperature << " fan off" << endl; 
            }
            else {
                if (iTemperature >= TEMPERATURE_LOW) {
                    cout << "mode1: " << bIsRising << " " << iTemperature << " high speed" << endl; 
                }
                else
                    cout << "mode1: " << bIsRising << " " << iTemperature << " fan off" << endl; 
            }
        }
        break;
    case eMode2:
        if (bIsRising) {
            if (iTemperature < TEMPERATURE_HIGH) {
                cout << "mode2: " << bIsRising << " " << iTemperature << " low speed" << endl; 
            }
            else 
                cout << "mode2: " << bIsRising << " " << iTemperature << " high speed" << endl; 
        }
        else {
            if (iTemperature >= TEMPERATURE_MID) {
                cout << "mode2: " << bIsRising << " " << iTemperature << " high speed" << endl; 
            }
            else 
                cout << "mode2: " << bIsRising << " " << iTemperature << " low speed" << endl; 
        }
        break;
    case eMode3:
        {
            //Get GPIO speed
            int iSpeedOff = rand() % 1;
            if (iSpeedOff) {
                cout << "mode3: " << iTemperature << " low speed" << endl;
                sleep(2);
                cout << "mode3: " << iTemperature << " high speed" << endl;
            }
            else 
                cout << "mode3: " << iTemperature << " high speed" << endl;

        }
        break;
    default:
        break;
    }
}


int createFanConfFile() {

    fstream fanConfFile; 
    fanConfFile.open(CONFIG_FILEPATH, fstream::out);
    int mode = 0;
    if (fanConfFile.is_open())
    {
        fanConfFile << mode << endl;
        fanConfFile.close();
        return mode;
    }
    return -1;

}

int getCurMode() {

    if (access(CONFIG_FILEPATH, F_OK) == 0) {
        ifstream fanConfFile(CONFIG_FILEPATH);
        if (fanConfFile.is_open())
        {
            string line;
            if( getline (fanConfFile, line) ) {
                if (!line.empty()) {
                    string::size_type sz = 0; 
                    int mode = stoi(line, &sz);
                    if (sz > 0) 
                        return mode; 
                }
            }
        }
    }

    return createFanConfFile();
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int mode = getCurMode();
    if (mode == -1) {
        cout << "Could not get correct fan conntrol mode." << endl;
        return 0;
    }
    int count = 0; 

    int iPreTemp = -1;

    while (1) {
        FILE *file; 
        char cmd[100];
        int iTemp = rand() % 14 + 39;
        sprintf(cmd, "echo %d", iTemp);

        count++;
        if (count % 5 == 0) {
            mode++;
            if (mode == 3) {
                mode = 0;
            }
        }

        char buf[100];

        if ((file = popen(cmd, "r")) != NULL)
        {
            if (fgets(buf, sizeof(buf), file) != NULL) {
                bool bIsRising = (iPreTemp > iTemp) ? false : true;
                setFanSpeed((FAN_CONTROL_MODE)mode, iTemp, bIsRising);
                iPreTemp = iTemp;
            }
            pclose(file);
        } 
        sleep(2);
    }

	return 0;
}


