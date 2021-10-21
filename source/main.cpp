#include "mbed.h"
#include "map"
#include "string"
#include "toolkit.h"



// main() runs in its own thread in the OS
int main()
{
    printf("\r\nStarting socket-sensor demo\r\n\r\n");
    SensorToolkit *sensorObj = new SensorToolkit();
    MBED_ASSERT(sensorObj);


    printf("\r\nStarting socket init\r\n\r\n");
#ifdef MBED_CONF_MBED_TRACE_ENABLE
    mbed_trace_init();
#endif    
    SensorFunctionPtr readEnvDataPtr = &SensorToolkit::read_env_data;
    SocketToolkit *socketObj = new SocketToolkit();
    MBED_ASSERT(socketObj);
    socketObj->run(sensorObj, readEnvDataPtr);

}

