HW3 BLE Programming - Central
===========================
## Project brief
Rasberry Pi 3 serves as central and Mbed as peripheral. Implement BLE button, LED1 status notification, writing to toggle LED2.

## Installation Instruction
1. Git pull
2. Download mbed-os-example-ble-Button on Arm Mbed Website and put the source folder in project root folder
2. (Optional) Change the python config and mbed variables, for example, UUID, device name per your local set-up 
4. Run following code.

## Device Requirements
1. DISCO-L4755VG-IOT01A
2. Raspberry Pi 3

**change following code**
```
# in ble_scan_connect.py
# user config *** where you might need to change
DEVICE_NAME = "Button_Paul"
BUTTON_SERVICE_UUID = 0xA000
BUTTON_CHAR_UUID = 0xA001
ID_SERVICE_UUID = 0xA002
LED_SERVICE_UUID = 0xA004
LED1_STATE_CHARACTERISTIC_UUID = 0xA005
LED2_STATE_CHARACTERISTIC_UUID = 0xA006

# in XXService.h, LEDService.h for example
const static uint16_t LED_SERVICE_UUID              = 0xA004;
const static uint16_t LED1_STATE_CHARACTERISTIC_UUID = 0xA005; //alive
const static uint16_t LED2_STATE_CHARACTERISTIC_UUID = 0xA006; //actuated
```
**plug in Mbed board and run this in command line**
```
sudo python ble_scan_connect.py
```

## Sample Output
Refer to report.pdf
