#ifndef TOOLKIT_H
#define TOOLKIT_H

#include "mbed.h"
#include "map"
#include <string>

// Sensor
class SensorToolkit{
    
public:
    SensorToolkit();
    ~SensorToolkit();
    std::map<string, float> read_env_data(std::map<string, float>);

private:
    float sensor_value = 0;
    int16_t pDataXYZ[3] = {0};
    float pGyroDataXYZ[3] = {0};
};


// Socket 
#include "wifi_helper.h"
#include "mbed-trace/mbed_trace.h"

#if MBED_CONF_APP_USE_TLS_SOCKET
#include "root_ca_cert.h"

#ifndef DEVICE_TRNG
#error "mbed-os-example-tls-socket requires a device which supports TRNG"
#endif
#endif // MBED_CONF_APP_USE_TLS_SOCKET

// Define function pointer for flexible passing functions into socket instance 
typedef std::map<string, float> (SensorToolkit::*SensorFunctionPtr)(std::map<string, float>); 

class SocketToolkit{
public:
    SocketToolkit();
    ~SocketToolkit();
    void run(SensorToolkit*, SensorFunctionPtr);

private:
    bool resolve_hostname(SocketAddress &);

    // This is a infinite loop function
    bool send_http_request(SensorToolkit*, SensorFunctionPtr);
    bool receive_http_response();
    void wifi_scan();
    void print_network_info();
    

    static constexpr size_t MAX_NUMBER_OF_ACCESS_POINTS = 10;
    static constexpr size_t MAX_MESSAGE_RECEIVED_LENGTH = 100;

    #if MBED_CONF_APP_USE_TLS_SOCKET
        static constexpr size_t REMOTE_PORT = 443; // tls port
    #else
        static constexpr size_t REMOTE_PORT = 65431; // standard HTTP port
    #endif // MBED_CONF_APP_USE_TLS_SOCKET


    NetworkInterface *_net;

#if MBED_CONF_APP_USE_TLS_SOCKET
    TLSSocket _socket;
#else
    TCPSocket _socket;
#endif // MBED_CONF_APP_USE_TLS_SOCKET
};




#endif