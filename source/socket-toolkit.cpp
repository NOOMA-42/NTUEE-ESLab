#include "mbed.h"
#include "toolkit.h"

SocketToolkit::SocketToolkit() 
: _net(NetworkInterface::get_default_instance())
{

}

SocketToolkit::~SocketToolkit()
{
    if (_net) {
        _net->disconnect();
    }
}

void SocketToolkit::run(SensorToolkit* sensorObj, SensorFunctionPtr read_env_data)
{
    if (!_net) {
        printf("Error! No network interface found.\r\n");
        return;
    }

    /* if we're using a wifi interface run a quick scan */
    if (_net->wifiInterface()) {
        /* the scan is not required to connect and only serves to show visible access points */
        wifi_scan();

        /* in this example we use credentials configured at compile time which are used by
            * NetworkInterface::connect() but it's possible to do this at runtime by using the
            * WiFiInterface::connect() which takes these parameters as arguments */
    }

    /* connect will perform the action appropriate to the interface type to connect to the network */

    printf("Connecting to the network...\r\n");

    nsapi_size_or_error_t result = _net->connect();
    if (result != 0) {
        printf("Error! _net->connect() returned: %d\r\n", result);
        return;
    }

    print_network_info();

    /* opening the socket only allocates resources */
    result = _socket.open(_net);
    if (result != 0) {
        printf("Error! _socket.open() returned: %d\r\n", result);
        return;
    }
#if MBED_CONF_APP_USE_TLS_SOCKET
        result = _socket.set_root_ca_cert(root_ca_cert);
        if (result != NSAPI_ERROR_OK) {
            printf("Error: _socket.set_root_ca_cert() returned %d\n", result);
            return;
        }
        _socket.set_hostname(MBED_CONF_APP_HOSTNAME);
#endif // MBED_CONF_APP_USE_TLS_SOCKET

    /* now we have to find where to connect */

    SocketAddress address;

    if (!resolve_hostname(address)) {
        return;
    }

    address.set_port(REMOTE_PORT);

    /* we are connected to the network but since we're using a connection oriented
        * protocol we still need to open a connection on the socket */

    printf("Opening connection to remote port %d\r\n", REMOTE_PORT);

    result = _socket.connect(address);
    if (result != 0) {
        printf("Error! _socket.connect() returned: %d\r\n", result);
        return;
    }

    /* exchange an HTTP request and response */

    if (!send_http_request(sensorObj, read_env_data)) {
        return;
    }

    if (!receive_http_response()) {
        return;
    }

    printf("Demo concluded successfully \r\n");
}


bool SocketToolkit::resolve_hostname(SocketAddress &address)
{
    const char hostname[] = MBED_CONF_APP_HOSTNAME;

    /* get the host address */
    printf("\nResolve hostname %s\r\n", hostname);
    nsapi_size_or_error_t result = _net->gethostbyname(hostname, &address);
    if (result != 0) {
        printf("Error! gethostbyname(%s) returned: %d\r\n", hostname, result);
        return false;
    }

    printf("%s address is %s\r\n", hostname, (address.get_ip_address() ? address.get_ip_address() : "None") );

    return true;
}

bool SocketToolkit::send_http_request(SensorToolkit* sensorObj, SensorFunctionPtr read_env_data)
{
    /* loop until whole request sent */
    const char buffer[] = "GET / HTTP/1.1\r\n"
                            "Host: ifconfig.io\r\n"
                            "Connection: close\r\n"
                            "\r\n";

    std::map <string, float> env_data;
    int sample_num = 0;
    nsapi_size_or_error_t bytes_sent = 0;

    /* continuous sending surrounding data */
    while(1){
        ++sample_num;
        char acc_json[100];

        printf("read data in socket function\r\n");
        env_data = sensorObj->read_env_data(env_data);
        nsapi_size_t bytes_to_send  = sprintf(
                acc_json,
                "{\"x\":%f, \"y\":%f, \"z\":%f, \"s\":%d}",
                (float)((int)(env_data["ACCELERO_X"]*10000)) / 10000,
                (float)((int)(env_data["ACCELERO_Y"]*10000)) / 10000, 
                (float)((int)(env_data["ACCELERO_Z"]*10000)) / 10000, 
                sample_num
            );
        printf("\r\nSending message: \r\n%s", acc_json);

        while (bytes_to_send) {
            printf("\r\nbytes to send: %d, bytes_sent: %d\r\n", bytes_to_send, bytes_sent);
            bytes_sent = _socket.send(acc_json, bytes_to_send);
            if (bytes_sent < 0) {
                printf("Error! _socket.send() returned: %d\r\n", bytes_sent);
                return false;
            } else {
                printf("sent %d bytes\r\n", bytes_sent);
            }

            bytes_to_send -= bytes_sent;
        }
    }

    printf("Complete message sent\r\n");

    return true;
}

bool SocketToolkit::receive_http_response()
{
    char buffer[MAX_MESSAGE_RECEIVED_LENGTH];
    int remaining_bytes = MAX_MESSAGE_RECEIVED_LENGTH;
    int received_bytes = 0;

    /* loop until there is nothing received or we've ran out of buffer space */
    nsapi_size_or_error_t result = remaining_bytes;
    while (result > 0 && remaining_bytes > 0) {
        result = _socket.recv(buffer + received_bytes, remaining_bytes);
        if (result < 0) {
            printf("Error! _socket.recv() returned: %d\r\n", result);
            return false;
        }

        received_bytes += result;
        remaining_bytes -= result;
    }

    /* the message is likely larger but we only want the HTTP response code */

    printf("received %d bytes:\r\n%.*s\r\n\r\n", received_bytes, strstr(buffer, "\n") - buffer, buffer);

    return true;
}

void SocketToolkit::wifi_scan()
{
    WiFiInterface *wifi = _net->wifiInterface();

    WiFiAccessPoint ap[MAX_NUMBER_OF_ACCESS_POINTS];

    /* scan call returns number of access points3
    
        found */
    int result = wifi->scan(ap, MAX_NUMBER_OF_ACCESS_POINTS);

    if (result <= 0) {
        printf("WiFiInterface::scan() failed with return value: %d\r\n", result);
        return;
    }

    printf("%d networks available:\r\n", result);

    for (int i = 0; i < result; i++) {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n",
                ap[i].get_ssid(), get_security_string(ap[i].get_security()),
                ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
                ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5],
                ap[i].get_rssi(), ap[i].get_channel());
    }
    printf("\r\n");
}

void SocketToolkit::print_network_info()
{
    /* print the network info */
    SocketAddress a;
    _net->get_ip_address(&a);
    printf("IP address: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
    _net->get_netmask(&a);
    printf("Netmask: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
    _net->get_gateway(&a);
    printf("Gateway: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
}