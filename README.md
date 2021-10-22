HW2 Mbed Socket-Sensor
===========================
## Installation Instruction
1. Install library mbed-os, wifi-ism43362, BSP_B-L475E-IOT01
2. Change the HOST name in mbed_app.json and hw2_server.py respectively. For testing server without Mbed, run hw2_client.py
3. Run following code. Note hw2_server.py must execute before the Mbed program. You may press the black (reset) button on the Mbed board.

```
python hw2_server.py
python hw2_client.py (for testing without Mbed)
running Mbed
```

## Sample Output
### server output
![alt text](https://i.ibb.co/MVnQrCC/demo3.png)

### mbed output
![alt text](https://i.ibb.co/Wc731GJ/demo2.png)