#include "uav.h"

uav::uav(int port_in, int port_out, double kp, double ki, double kd, double target, double integralLimit)
{
    ret = new interface();
    pid_roll = new PID(kp, ki, kd, target, integralLimit);
    pid_pitch = new PID(kp, ki, kd, target, integralLimit);
    pid_yaw = new PID(kp, ki, kd, target, integralLimit);
    pid_alt = new PID(kp, ki, kd, target, integralLimit);

    ret->set_interface_ports("127.0.0.1", port_in, port_out);

    // inp.servos[0] = 1500;   // Left Elevon
    // inp.servos[1] = 1500;   // Right Elevon
    // inp.servos[2] = 1000;   // Throttle
    // inp.servos[3] = 1500;   // Elevator

    inp.servos[0] = 1000;   // mot_1
    inp.servos[1] = 1000;   // mot_2
    inp.servos[2] = 1000;   // mot_3
    inp.servos[3] = 1000;   // mot_4
}

void uav::update_AHRS()
{
    _roll = ret->gyro.getX() * 0.97  + ret->accel_body.getX() * 0.03;
    _pitch = ret->gyro.getY() * 0.97  + ret->accel_body.getY() * 0.03;
    _yaw = ret->gyro.getZ() * 0.97  + ret->accel_body.getY() * 0.03;
    _alt = ret->position.getZ();
}


void uav::update()
{
    update_AHRS();
    pid_roll->calculate(_roll);
    pid_pitch->calculate(_pitch);
    pid_yaw->calculate(_yaw);

  ret->send_servos(inp);
}