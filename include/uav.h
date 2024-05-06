#include <stdio.h>
#include "interface.h"
#include <fstream>
#include <iomanip>
#include "Controller.h"

class uav
{
    public:
        void update();
        uav(int port_in, int port_out, double kp, double ki, double kd, double target, double integralLimit);
        PID *pid_roll, *pid_pitch, *pid_yaw, *pid_alt;

    private:
        interface* ret;
        sitl_input inp;
        float _roll, _pitch, _yaw, _alt;
        void update_AHRS();

};