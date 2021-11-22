DHT11 one-wire serial protocol and device drivers
===========================
## Lab Instruction
About using DHT11 sensors, Compare the IIO kernel driver implementation to that of Adafruit DHT11 python module. 
•Ref: https://github.com/adafruit/Adafruit_Python_DHT/tree/master/source/Raspberry_Pi_2

Submit a report including the following two parts and some discussions:
* Observe the DHT11 signal using a Logic  Analyzer. 
* Explain or discuss the following terms:
    1) What is Linux IIO subsystem?
    (reference: https://www.kernel.org/doc/html/latest/driver-api/iio/index.html (連結到外部網站。))
    (To access IIO devices, Linux application can use ibiio: https://analogdevicesinc.github.io/libiio/v0.20/index.html (連結到外部網站。) )
    2) What is the memory-map IO?
    3) How is the efficiency difference when compared between interrupt-driven I/O and polling I/O?
    4) in pi_2_mmio.h, why pointer operation (pi_2_mmio_gpio+7)  (pi_2_mmio_gpio+10)?

## Project brief
We observed the DHT11 sensor data, humidity and temperature, with logic analyzer
and corresponded to the output data from Python.

## Sample Output
For further detail, please refer to report.pdf
