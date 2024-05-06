#include <stdio.h>
#include <fstream>
#include <iomanip>
#include "uav.h"

const int SIM_IN_PORT = 9003;
const int SIM_OUT_PORT = 9002;

// PID controller parameters (adjust these values based on your system)
const double kp = 1.0;
const double ki = 0.2;
const double kd = 0.01;
const double ILimit = 300.0;

// Target set-point (desired gyro reading)
double target = 0.0;

int main()
{

    uav *uav_system = new uav(SIM_IN_PORT, SIM_OUT_PORT, kp, ki, kd, target, ILimit);

    std::ofstream outputFile("log.txt");

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1; // Return an error code
    }

    uav_system->pid_alt->set_target(100.0f);
    uav_system->pid_alt->set_Kd(0.0f);
    uav_system->pid_alt->set_Ki(0.00f);
    uav_system->pid_alt->set_Kp(16.0f);

    uav_system->pid_roll->set_target(0.0f);
    uav_system->pid_roll->set_Kd(1.0f);
    uav_system->pid_roll->set_Ki(0.00f);
    uav_system->pid_roll->set_Kp(1.0f);

    uav_system->pid_pitch->set_target(0.0f);
    uav_system->pid_pitch->set_Kd(4.0f);
    uav_system->pid_pitch->set_Ki(0.00f);
    uav_system->pid_pitch->set_Kp(1.0f);

    
    
    while (1)
    {
        uav_system->update(outputFile);
        
    }
    

    outputFile.close();
    return 0;
}
