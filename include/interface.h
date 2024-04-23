#include "Socket.h"
#include "util.h"

/*
  structure passed in giving servo positions as PWM values in
  microseconds
*/
struct sitl_input {
    uint16_t servos[32];
    struct {
        float speed;      // m/s
        float direction;  // degrees 0..360
        float turbulence;
        float dir_z;	  //degrees -90..90
    } wind;
};

class interface
{
    public:
        interface();

        /*  Create and set in/out socket for Gazebo simulator */
        void set_interface_ports(const char* address, const int port_in, const int port_out);
        void update(const struct sitl_input &input);
        Vector3f accel_body, gyro, position;
        void send_servos(const struct sitl_input &input);

    private:

        void recv_fdm(const struct sitl_input &input);
        
        void drain_sockets();

        /*
        packet sent to Gazebo
        */
        struct servo_packet {
      // size matches sitl_input upstream
      float motor_speed[16];
    };

    /*
      reply packet sent
     */
    struct fdm_packet {
      double timestamp;  // in seconds
      double imu_angular_velocity_rpy[3];
      double imu_linear_acceleration_xyz[3];
      double imu_orientation_quat[4];
      double velocity_xyz[3];
      double position_xyz[3];
    };

    SocketAPM socket_sitl;
    const char *_gazebo_address = "127.0.0.1";
    int _gazebo_port = 9002;
    static const uint64_t GAZEBO_TIMEOUT_US = 5000000;
    

};