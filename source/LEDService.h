/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __BLE_LED_SERVICE_H__
#define __BLE_LED_SERVICE_H__

class LEDService {
public:
    const static uint16_t LED_SERVICE_UUID              = 0xA004;
    const static uint16_t LED1_STATE_CHARACTERISTIC_UUID = 0xA005; //alive
    const static uint16_t LED2_STATE_CHARACTERISTIC_UUID = 0xA006; //actuated

    LEDService(BLE &_ble, bool initialValueForLEDCharacteristic) :
        ble(_ble), 
        //alive
        LED1State(LED1_STATE_CHARACTERISTIC_UUID, &initialValueForLEDCharacteristic, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        //actuated
        LED2State(LED2_STATE_CHARACTERISTIC_UUID, &initialValueForLEDCharacteristic, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE)
    {
        GattCharacteristic *charTable[] = {&LED1State, &LED2State};
        GattService         ledService(LED_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.gattServer().addService(ledService);
    }

    GattAttribute::Handle_t getLED2ValueHandle() const
    {
        return LED2State.getValueHandle();
    }

    void updateLED1State(bool newState) {
        ble.gattServer().write(LED1State.getValueHandle(), (uint8_t *)&newState, sizeof(bool));
    }
private:
    BLEDevice                         &ble;
    ReadOnlyGattCharacteristic<bool> LED1State;
    ReadWriteGattCharacteristic<bool> LED2State;
};

#endif /* #ifndef __BLE_LED_SERVICE_H__ */
