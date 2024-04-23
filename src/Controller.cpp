#include "Controller.h"

PID::PID(double kp, double ki, double kd, double target, double integralLimit)
        : kp(kp), ki(ki), kd(kd), target(target), integral(0), integralLimit(integralLimit), lastError(0) {}


double PID::calculate(double actual) 
{
        double error = target - actual;

        // Proportional term
        double P = kp * error;

        // Integral term
        integral += error;
        
        // Anti-wind-up: Limit the integral term
        if (integral > integralLimit) {
            integral = integralLimit;
        } else if (integral < -integralLimit) {
            integral = -integralLimit;
        }

        double I = ki * integral;

        // Derivative term
        double derivative = error - lastError;
        double D = kd * derivative;

        // PID output
        double output = P + I + D;

        // Update last error for the next iteration
        lastError = error;

        return output;
}