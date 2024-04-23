#include <stdio.h>
#include "interface.h"
#include <fstream>
#include <iomanip>
#include "Controller.h"


const int SIM_IN_PORT = 9003;
const int SIM_OUT_PORT = 9002;

int main()
{
    // time_t start_time = time(NULL);
    // time_t current_time;

    interface* h = new interface();
    sitl_input inp;
    // inp.servos[0] = 1500;   // Left Elevon
    // inp.servos[1] = 1500;   // Right Elevon
    // inp.servos[2] = 1000;   // Throttle
    // inp.servos[3] = 1500;   // Elevator

    inp.servos[0] = 1500;   // mot_1
    inp.servos[1] = 1500;   // mot_2
    inp.servos[2] = 1500;   // mot_3
    inp.servos[3] = 1500;   // mot_4

    // PID controller parameters (adjust these values based on your system)
    double kp = 1.0;
    double ki = 0.2;
    double kd = 0.01;
    double ILimit = 300.0;

    // Target set-point (desired gyro reading)
    double target = 0.0;

    // int roll_cd, pitch_cd, yaw_cd, throttle_cd;
    PID pid_roll(kp, ki, kd, target, ILimit);

    h->set_interface_ports("127.0.0.1", SIM_IN_PORT, SIM_OUT_PORT);

    std::ofstream outputFile("example.txt");

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1; // Return an error code
    }

    int idx = 0;
    while(1)
    {
        h->update(inp);
        // sleep(1);
        // current_time = time(NULL);
        // if (current_time - start_time >= 1) {
        //     idx += 10;
        //     for(int i = 0; i < 4; i++)
        //     {
        //     inp.servos[i] = 1000 + idx;
        //     if(inp.servos[i] > 1600)
        //         inp.servos[i] = 1600;
        //     // inp.servos[idx] = 1100;
        //     // inp.servos[idx-1] = 1000;
        //     // if (idx > 3)
        //     //     idx = 0;
        //     // printf("%d %d\n", idx, inp.servos[idx]);
        //     }
        //     start_time = current_time; // Reset start time
        // }
        
        // double out = pid_roll.calculate(h->gyro.getX());

        // outputFile << std::fixed << std::setprecision(3) << h->position.getX() <<" " << h->position.getY() <<" " << h->position.getZ() <<" " <<out << std::endl;
        // std::cout << "<PID >" << out << std::endl;

        h->send_servos(inp);
    }

    outputFile.close();
    return 0;
}
