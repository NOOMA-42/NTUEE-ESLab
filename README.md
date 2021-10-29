HW3 BLE Programming - Central
===========================
## Project brief
Rasberry Pi 3 serves as central and IPhone LightBlue as peripheral. Implement BLE
CCCD notification.

## Installation Instruction
1. Git pull
2. Change the config per your local set-up 
3. Run following code.

**change following code in ble_scan_connect.py**
```
# user config
DEVICE_NAME = "Heart Rate"
SERVICE_UUID = 0xfff0
CHAR_UUID = 0xfff1
```
**run this in command line**
```
sudo python ble_scan_connect.py
```

## Sample Output
Refer to report.pdf
