#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

typedef struct gnrmcdata{
	char gnrmcdataName[20];
	float gnrmcdataUTCtime;
	char gnrmcdataStatus[10];
	float gnrmcdataLatitude;
	char gnrmcdataNSindicator[10];
	float gnrmcdataLongitude;
	char gnrmcdataEWindicator[10];
	float gnrmcdataSpeed;
	float gnrmcdataCourse;
	int gnrmcdataDate;
	float gnrmcdataMVvalue;
	char gnrmcdataMVEWindicator[10];
	char gnrmcdataModeindicator[50];
}data;

int main() {
	data bufferdata;
	char buffer[256] = "$GNRMC,233843.698,A,4830.5778,N,04433.5867,E,0.972006,,270719,,,D*6F";
    char bufferInsertBlank[256];
    int blankIndex = 0;

    for (int i = 0; i < strlen(buffer); i++) {
        bufferInsertBlank[blankIndex++] = buffer[i];

        if (buffer[i] == ',' && buffer[i + 1] == ',') {
            bufferInsertBlank[blankIndex++] = '@';
        }
    }
	//strcpy(bufferCopy, buffer);

	char* ptr = strtok(bufferInsertBlank, ",");
	int k = 0;

    while (ptr != NULL) {
        switch (k) {
        case 0:
            strcpy(bufferdata.gnrmcdataName, ptr); break;
        case 1:
            bufferdata.gnrmcdataUTCtime = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("UTC time: %f\n", bufferdata.gnrmcdataUTCtime); break;
        case 2:
            //bufferdata.gnrmcdataStatus = (ptr[0] == '@') ? 'none' : *ptr;
            //printf("Status: %c\n", bufferdata.gnrmcdataStatus); break;
            strncpy(bufferdata.gnrmcdataStatus, (ptr[0] == '@') ? "none" : ptr, sizeof(bufferdata.gnrmcdataStatus));
            printf("Status: %s\n", ptr); break;
        case 3:
            bufferdata.gnrmcdataLatitude = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Latitude: %f\n", bufferdata.gnrmcdataLatitude); break;
        case 4:
            //bufferdata.gnrmcdataNSindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //printf("North/South Indicator: %s\n", bufferdata.gnrmcdataNSindicator); break;
            strncpy(bufferdata.gnrmcdataNSindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(bufferdata.gnrmcdataNSindicator));
            printf("North/South Indicator: %s\n", ptr); break;
        case 5:
            bufferdata.gnrmcdataLongitude = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Longitude: %f\n", bufferdata.gnrmcdataLongitude); break;
        case 6:
            //bufferdata.gnrmcdataEWindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //printf("East/West Indicator: %s\n", bufferdata.gnrmcdataEWindicator); break;
            strncpy(bufferdata.gnrmcdataEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(bufferdata.gnrmcdataEWindicator));
            printf("East/West Indicator: %s\n", ptr);  break;
        case 7:
            bufferdata.gnrmcdataSpeed = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Speed: %f\n", bufferdata.gnrmcdataSpeed); break;
        case 8:
            bufferdata.gnrmcdataCourse = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Course: %f\n", bufferdata.gnrmcdataCourse); break;
        case 9:
            bufferdata.gnrmcdataDate = (ptr[0] == '@') ? 0 : atoi(ptr);
            printf("Date: %d\n", bufferdata.gnrmcdataDate); break;
        case 10:
            bufferdata.gnrmcdataMVvalue = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Magnetic Variation value: %f\n", bufferdata.gnrmcdataMVvalue); break;
        case 11:
            //bufferdata.gnrmcdataMVEWindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //printf("Magnetic Variation East/West Indicator: %s\n", bufferdata.gnrmcdataMVEWindicator); break;
            strncpy(bufferdata.gnrmcdataMVEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(bufferdata.gnrmcdataMVEWindicator));
            printf("Magnetic Variation East/West Indicator: %s\n", ptr); break;
        case 12:
            strncpy(bufferdata.gnrmcdataModeindicator, (ptr[0] == '@') ? "NULL" : ptr, sizeof(bufferdata.gnrmcdataModeindicator)); break;
        }

        ptr = strtok(NULL, ",");
        k++;
    }
}