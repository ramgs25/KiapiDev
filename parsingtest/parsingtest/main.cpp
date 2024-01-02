#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

typedef struct gnrmcdata{
	char gnrmcdataName[20];
	float gnrmcdataUTCtime;
	char gnrmcdataStatus;
	float gnrmcdataLatitude;
	char gnrmcdataNSindicator;
	float gnrmcdataLongitude;
	char gnrmcdataEWindicator;
	float gnrmcdataSpeed;
	float gnrmcdataCourse;
	int gnrmcdataDate;
	float gnrmcdataMVvalue;
	char gnrmcdataMVEWindicator;
	char gnrmcdataModeindicator[50];
}data;

int main() {
	data bufferdata;
	char buffer[256] = "$GNRMC,233843.698,A,4830.5778,N,04433.5867,E,0.972006,,270719,,,D*6F";
	
	char bufferCopy[256];
	strcpy(bufferCopy, buffer);

	char* ptr = strtok(bufferCopy, ",");
	int i = 0;
	int j = 0;

	while (ptr != NULL) {
		switch (i) {
		case 0:
			strcpy(bufferdata.gnrmcdataName, ptr); break;
		case 1:
			bufferdata.gnrmcdataUTCtime = atof(ptr);
			printf("UTC time: %f\n", bufferdata.gnrmcdataUTCtime); break;
		case 2:
			bufferdata.gnrmcdataStatus = *ptr;
			printf("Status: %c\n", bufferdata.gnrmcdataStatus); break;
		case 3:
			bufferdata.gnrmcdataLatitude = atof(ptr);
			printf("Latitude: %f\n", bufferdata.gnrmcdataLatitude); break;
		case 4:
			bufferdata.gnrmcdataNSindicator = *ptr;
			printf("North/South Indicator: %c\n", bufferdata.gnrmcdataNSindicator); break;
		case 5:
			bufferdata.gnrmcdataLongitude = atof(ptr);
			printf("Longitude: %f\n", bufferdata.gnrmcdataLongitude); break;
		case 6:
			bufferdata.gnrmcdataEWindicator = *ptr;
			printf("East/West Indicator: %c\n", bufferdata.gnrmcdataEWindicator); break;
		case 7:
			bufferdata.gnrmcdataSpeed = atof(ptr);
			printf("Speed: %f\n", bufferdata.gnrmcdataSpeed); break;
		case 8:
			bufferdata.gnrmcdataCourse = atof(ptr);
			printf("Course: %f\n", bufferdata.gnrmcdataCourse); break;
		case 9:
			bufferdata.gnrmcdataDate = atoi(ptr);
			printf("Date: %d\n", bufferdata.gnrmcdataDate); break;
		case 10:
			bufferdata.gnrmcdataMVvalue = atof(ptr);
			printf("Magnetic Variation value: %f\n", bufferdata.gnrmcdataMVvalue); break;
		case 11:
			bufferdata.gnrmcdataMVEWindicator = *ptr;
			printf("Magnetic Variation East/West Indicator: %c\n", bufferdata.gnrmcdataMVEWindicator); break;
		case 12:
			strncpy(bufferdata.gnrmcdataModeindicator, ptr, sizeof(bufferdata.gnrmcdataModeindicator)); break;
		}

		ptr = strtok(NULL, ",");
		i++;
	}
}