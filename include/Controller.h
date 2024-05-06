#include <cmath>

class PID {
private:
    double kp;  // Proportional gain
    double ki;  // Integral gain
    double kd;  // Derivative gain
    double target;  // Target set-point
    double integral;  // Integral term accumulator
    double integralLimit;  // Maximum value for the integral term
    double lastError;  // Last error for derivative term

public:
    PID(double kp, double ki, double kd, double target, double integralLimit);
    double calculate(double actual);

     // Setter methods
    void set_Kp(float newKp) { kp = newKp; }
    void set_Ki(float newKi) { ki = newKi; }
    void set_Kd(float newKd) { kd = newKd; }
    void set_Ilimit(float newIlimit) { integralLimit = newIlimit; }


    // Getter methods
    float get_Kp() const { return kp; }
    float get_Ki() const { return ki; }
    float get_Kd() const { return kd; }
    float get_Ilimit() const { return integralLimit; }

};