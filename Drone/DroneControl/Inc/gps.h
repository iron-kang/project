#ifndef __GPS_H
#define __GPS_H

//https://github.com/toskyRocker/Arduino_Parser_Ublox_Neo_M8N
#pragma pack(push)
#pragma pack(1)
typedef struct {
	float latitude;
	float longitude;
	float altitude;
	int8_t sats_in_use;
} GPS_M8N;

typedef struct
{
	uint8_t prn;
	int16_t elevation;
	int16_t azimuth;
	uint8_t snr; //signal to noise ratio
} satellite;

typedef struct
{
	uint8_t day, month, year;
	uint8_t hours, minutes, seconds;
	uint16_t millis;
	bool valid; //1 = yes, 0 = no
} _datetime;

typedef struct
{
	char* str;
	char token;
} Tokeniser;
#pragma pack(pop)

typedef enum { FIX_TYPE_NONE, FIX_TYPE_GPS, FIX_TYPE_DIFF } _fixtype;
typedef enum { FIX_NONE = 1, FIX_2D, FIX_3D } _fix;
typedef enum { MODE_MANUAL, MODE_AUTOMATIC } _op_mode;

int GPS_Encode(char c);

#endif 
