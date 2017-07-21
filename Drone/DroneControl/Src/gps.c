#include "common.h"

char buf[120];
uint8_t pos;
float latitude, longitude, altitude, vert_speed;
int latlng_age, alt_age;

//these units are in hundredths
//so a speed of 5260 means 52.60km/h
uint16_t speed, course, knots;
int speed_age, course_age, knots_age;

_fixtype fixtype; //0 = no fix, 1 = satellite only, 2 = differential fix
int fixtype_age;
_fix fix;
int fix_age;

float pdop, hdop, vdop; //positional, horizontal and vertical dilution of precision
int dop_age;

int8_t sats_in_use;
int8_t sats_in_view;

satellite sats[12];
int sats_age;

_datetime datetime;
int time_age, date_age;

_op_mode op_mode;

int process_buf();
int check_checksum();
uint8_t parse_hex(char c);
int next(Tokeniser tok, char* out, int len);
void read_gga();
void read_gsa();
void read_gsv();
void read_rmc();
void read_vtg();

int GPS_Encode(char c)
{
    buf[pos] = c;
    pos++;

    if (c == '\n')
    {
        bool ret = process_buf();
        memset(buf, '\0', 120);
        pos = 0;
        return ret;
    }

    if (pos >= 120)
    {
        memset(buf, '\0', 120);
        pos = 0;
    }

    return 0;
}

int process_buf()
{
    if(!check_checksum())
    {
        return 0;
    }

    if(strncmp(buf, "$GNGGA", 6) == 0)
    {
        read_gga();
    }

    if(strncmp(buf, "$GNGSA", 6) == 0)
    {
        read_gsa();
    }

    if(strncmp(buf, "$GPGSV", 6) == 0)
    {
        read_gsv();
    }

    if(strncmp(buf, "$GNRMC", 6) == 0)

    {
        read_rmc();
    }
    if(strncmp(buf, "$GNVTG", 6) == 0)
    {
        read_vtg();
    }

    return 1;
}

int check_checksum()
{
    if (buf[strlen(buf)-5] == '*')
    {
        uint16_t sum = parse_hex(buf[strlen(buf)-4]) * 16;
        sum += parse_hex(buf[strlen(buf)-3]);

        for (uint8_t i=1; i < (strlen(buf)-5); i++)
            sum ^= buf[i];
        if (sum != 0)
            return 0;

        return 1;
    }
    return 0;
}

uint8_t parse_hex(char c)
{
    if (c < '0')
        return 0;
    if (c <= '9')
        return c - '0';
    if (c < 'A')
        return 0;
    if (c <= 'F')
        return (c - 'A')+10;
    return 0;
}

void read_gga()
{
    int counter = 0;
    char token[20];
    Tokeniser tok;
    tok.str = buf;
    tok.token = ',';

    while (next(tok, token, 20))
    {
        switch(counter)
        {
        case 1: //time
        {
            float time = atof(token);
            int hms = (int)time;

            datetime.millis = time - hms;
            datetime.seconds = fmod(hms, 100);
            hms /= 100;
            datetime.minutes = fmod(hms, 100);
            hms /= 100;
            datetime.hours = hms;

            time_age = HAL_GetTick();
        }
        break;
        case 2: //latitude
        {
            float llat = atof(token);
            int ilat = llat/100;
            double mins = fmod(llat, 100);
            latitude = ilat + (mins/60);
        }
        break;
        case 3: //north/south
        {
            if(token[0] == 'S')
                latitude = -latitude;
        }
        break;
        case 4: //longitude
        {
            float llong = atof(token);
            int ilat = llong/100;
            double mins = fmod(llong, 100);
            longitude = ilat + (mins/60);
        }
        break;
        case 5: //east/west
        {
            if(token[0] == 'W')
                longitude = -longitude;
            latlng_age = HAL_GetTick();
        }
        break;
        case 6:
        {
            fixtype = (_fixtype) atoi(token);
        }
        break;
        case 7:
        {
            sats_in_use = atoi(token);
        }
        break;
        case 8:
        {
            hdop = atoi(token);
        }
        break;
        case 9:
        {
            float new_alt = atof(token);
            vert_speed = (new_alt - altitude)/((HAL_GetTick()-alt_age)/1000.0);
            altitude = atof(token);
            alt_age = HAL_GetTick();
        }
        break;
        }
        counter++;
    }
}

int next(Tokeniser tok, char* out, int len)
{
    uint8_t count = 0;

    if(tok.str[0] == 0)
        return 0;

    while(1)
    {
        if(tok.str[count] == '\0')
        {
            out[count] = '\0';
            tok.str = &tok.str[count];
            return 1;
        }

        if(tok.str[count] == tok.token)
        {
            out[count] = '\0';
            count++;
            tok.str = &tok.str[count];
            return 1;
        }

        if(count < len)
            out[count] = tok.str[count];

        count++;
    }
    return 0;
}

void read_gsa()
{
    int counter = 0;
    char token[20];
    Tokeniser tok;
    tok.str = buf;
    tok.token = ',';

    while (next(tok, token, 20))
    {
        switch(counter)
        {
        case 1: //operating mode
        {
            if(token[0] == 'A')
                op_mode = MODE_AUTOMATIC;
            if(token[0] == 'M')
                op_mode = MODE_MANUAL;
        }
        break;
        case 2:
        {
            fix = (_fix) atoi(token);
            fix_age = HAL_GetTick();
        }
        break;
        case 14:
        {
            pdop = atof(token);
        }
        break;
        case 15:
        {
            hdop = atof(token);
        }
        break;
        case 16:
        {
            vdop = atof(token);
            dop_age = HAL_GetTick();
        }
        break;
        }
        counter++;
    }
}

void read_gsv()
{
    char token[20];
    Tokeniser tok;
    tok.str = buf;
    tok.token = ',';

    next(tok, token, 20);
    next(tok, token, 20);

    next(tok, token, 20);
    int mn = atoi(token); //msg number

    next(tok, token, 20);
    sats_in_view = atoi(token); //number of sats

    int8_t j = (mn-1) * 4;
    int8_t i;

    for(i = 0; i <= 3; i++)
    {
        next(tok, token, 20);
        sats[j+i].prn = atoi(token);

        next(tok, token, 20);
        sats[j+i].elevation = atoi(token);

        next(tok, token, 20);
        sats[j+i].azimuth = atoi(token);

        next(tok, token, 20);
        sats[j+i].snr = atoi(token);
    }
    sats_age = HAL_GetTick();
}

void read_rmc()
{
    int counter = 0;
    char token[20];
    Tokeniser tok;
    tok.str = buf;
    tok.token = ',';

    while (next(tok, token, 20))
    {
        switch(counter)
        {
        case 1: //time
        {
            float time = atof(token);
            int hms = (int)time;

            datetime.millis = time - hms;
            datetime.seconds = fmod(hms, 100);
            hms /= 100;
            datetime.minutes = fmod(hms, 100);
            hms /= 100;
            datetime.hours = hms;

            time_age = HAL_GetTick();
        }
        break;
        case 2:
        {
            if(token[0] == 'A')
                datetime.valid = true;
            if(token[0] == 'V')
                datetime.valid = false;
        }
        break;
        case 8:
        {
            course = atof(token);
            course_age = HAL_GetTick();
        }
        break;
        case 9:
        {
            uint32_t date = atoi(token);
            datetime.year = fmod(date, 100);
            date /= 100;
            datetime.month = fmod(date, 100);
            datetime.day = date / 100;
            date_age = HAL_GetTick();
        }
        break;
        }
        counter++;
    }
}

void read_vtg()
{
    int counter = 0;
    char token[20];
    Tokeniser tok;
    tok.str = buf;
    tok.token = ',';

    while (next(tok, token, 20))
    {
        switch(counter)
        {
        case 1:
        {
            course = (atof(token)*100);
            course_age = HAL_GetTick();
        }
        break;
        case 5:
        {
            knots = (atof(token)*100);
            knots_age = HAL_GetTick();
        }
        break;
        case 7:
        {
            speed = (atof(token)*100);
            speed_age = HAL_GetTick();
        }
        break;
        }
        counter++;
    }
}

