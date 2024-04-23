#include "interface.h"
#include <fstream>

interface::interface():

    socket_sitl{true}
    {
        fprintf(stdout, "Starting ROS Interface for Robotics\n");
    }


/*
  Create and set in/out socket
*/
void interface::set_interface_ports(const char* address, const int port_in, const int port_out)
{
    // try to bind to a specific port so that if we restart ArduPilot
    // Gazebo keeps sending us packets. Not strictly necessary but
    // useful for debugging
    if (!socket_sitl.bind(address, port_in)) {
        fprintf(stderr, "SITL: socket in bind failed on sim in : %d  - %s\n", port_in, strerror(errno));
        fprintf(stderr, "Aborting launch...\n");
        exit(1);
    }
    printf("Bind %s:%d for INTERFACE in\n", "127.0.0.1", port_in);
    socket_sitl.reuseaddress();
    socket_sitl.set_blocking(false);

    _gazebo_address = address;
    _gazebo_port = port_out;
    printf("Setting Gazebo interface to %s:%d \n", _gazebo_address, _gazebo_port);
}


/*
  decode and send servos
*/
void interface::send_servos(const struct sitl_input &input)
{
    servo_packet pkt;
    // should rename servo_command
    // 16 because struct sitl_input.servos is 16 large in SIM_Aircraft.h
    for (unsigned i = 0; i < 16; ++i)
    {
      pkt.motor_speed[i] = (input.servos[i]-1000) / 1000.0f;
    }
    socket_sitl.sendto(&pkt, sizeof(pkt), _gazebo_address, _gazebo_port);
}

/*
  receive an update from the FDM
  This is a blocking function
 */
void interface::recv_fdm(const struct sitl_input &input)
{
    fdm_packet pkt;

    /*
      we re-send the servo packet every 0.1 seconds until we get a
      reply. This allows us to cope with some packet loss to the FDM
     */
    while (socket_sitl.recv(&pkt, sizeof(pkt), 100) != sizeof(pkt)) {
        send_servos(input);
    }

    accel_body = Vector3f(static_cast<float>(pkt.imu_linear_acceleration_xyz[0]),
                          static_cast<float>(pkt.imu_linear_acceleration_xyz[1]),
                          static_cast<float>(pkt.imu_linear_acceleration_xyz[2]));

    
    gyro = Vector3f(static_cast<float>(pkt.imu_angular_velocity_rpy[0]),
                    static_cast<float>(pkt.imu_angular_velocity_rpy[1]),
                    static_cast<float>(pkt.imu_angular_velocity_rpy[2]));

    position = Vector3f(static_cast<float>(pkt.position_xyz[0]),
                    static_cast<float>(pkt.position_xyz[1]),
                    static_cast<float>(pkt.position_xyz[2]));

    // printf("GYRO  = %f  %f  %f\n", gyro.getX(), gyro.getY(), gyro.getZ());
    // printf("Position  = %f  %f  %f\n", position.getX(), position.getY(), position.getZ());
}

/*
  Drain remaining data on the socket to prevent phase lag.
 */
void interface::drain_sockets()
{
    const uint16_t buflen = 1024;
    char buf[buflen];
    ssize_t received;
    errno = 0;
    do {
        received = socket_sitl.recv(buf, buflen, 0);
        if (received < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK && errno != 0) {
                fprintf(stderr, "error recv on socket in: %s \n",
                        strerror(errno));
            }
        } else {
            // fprintf(stderr, "received from control socket: %s\n", buf);
        }
    } while (received > 0);

}

/*
  update the Gazebo simulation by one time step
 */
void interface::update(const struct sitl_input &input)
{
    send_servos(input);
    recv_fdm(input);
    // update_position();
    drain_sockets();
}
