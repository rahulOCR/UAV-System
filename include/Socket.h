#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <iostream>
#include <cstring>



class SocketAPM {
public:
    SocketAPM(bool _datagram);
    SocketAPM(bool _datagram, int _fd);
    ~SocketAPM();

    bool connect(const char *address, uint16_t port);
    bool bind(const char *address, uint16_t port);
    bool reuseaddress() const;
    bool set_blocking(bool blocking) const;
    bool set_cloexec() const;
    void set_broadcast(void) const;

    ssize_t send(const void *pkt, size_t size) const;
    ssize_t sendto(const void *buf, size_t size, const char *address, uint16_t port);
    ssize_t recv(void *pkt, size_t size, uint32_t timeout_ms);

    // return the IP address and port of the last received packet
    void last_recv_address(const char *&ip_addr, uint16_t &port) const;

    // return true if there is pending data for input
    bool pollin(uint32_t timeout_ms);

    // return true if there is room for output data
    bool pollout(uint32_t timeout_ms);

    // start listening for new tcp connections
    bool listen(uint16_t backlog) const;

    // accept a new connection. Only valid for TCP connections after
    // listen has been used. A new socket is returned
    SocketAPM *accept(uint32_t timeout_ms);

private:
    bool datagram;
    struct sockaddr_in in_addr {};

    int fd = -1;

    void make_sockaddr(const char *address, uint16_t port, struct sockaddr_in &sockaddr);
};