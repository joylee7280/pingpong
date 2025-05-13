#include <SoftwareSerial.h>
#define sensitivity 70
#define sensor1 A0
#define sensor2 A1
#define sensor3 A2
#define sensor4 A3
#define sensor5 A4
#define sensor6 A5
int sensors[7] = {0, sensor1, sensor2, sensor3, sensor4, sensor5, sensor6};

int is_hit(int target, int mode)
{
    if (mode == 10 || mode == 30)
    {
        if (analogRead(sensors[target]) >= sensitivity)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (mode == 20)
    {
        for (int i = 1; i < 7; i++)
        {
            if (analogRead(sensors[i]) >= sensitivity)
            {
                return i;
            }
        }
    }
}
