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
    _roll = ret->gyro.getX();
    _pitch = ret->gyro.getY();
    _yaw = ret->gyro.getZ();
    _alt = ret->position.getZ() * (-1);
    

}


void uav::update(std::ofstream &outputFile)
{
    ret->update(inp);
    update_AHRS();
    double pid_output_roll = pid_roll->calculate(_roll);
    double pid_output_pitch = pid_pitch->calculate(_pitch);
    pid_yaw->calculate(_yaw);

    double pid_output_alt = pid_alt->calculate(_alt);
    // printf("%f %f\n", ret->gyro.getX(), ret->gyro.getY());
    
    inp.servos[0] = 1580 + (int16_t)pid_output_alt;// - (int16_t)pid_output_roll + (int16_t)pid_output_pitch;
    inp.servos[1] = 1580 + (int16_t)pid_output_alt;// + (int16_t)pid_output_roll - (int16_t)pid_output_pitch;
    inp.servos[2] = 1580 + (int16_t)pid_output_alt ;//+ (int16_t)pid_output_roll + (int16_t)pid_output_pitch;
    inp.servos[3] = 1580 + (int16_t)pid_output_alt ;//- (int16_t)pid_output_roll - (int16_t)pid_output_pitch;
        

    
    
    
    
    outputFile << std::fixed << std::setprecision(3) << _alt <<" " << pid_output_alt <<std::endl;

  ret->send_servos(inp);
}