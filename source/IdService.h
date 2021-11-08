#ifndef __BLE_ID_SERVICE_H__
#define __BLE_ID_SERVICE_H__


class IdService {
public:
    const static uint16_t ID_SERVICE_UUID              = 0xA002;
    const static uint16_t ID_STATE_CHARACTERISTIC_UUID = 0xA003;

    IdService(BLE &_ble, char* IdInitial) :
        ble(_ble), idState(ID_STATE_CHARACTERISTIC_UUID, IdInitial, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
    {
        GattCharacteristic *charTable[] = {&idState};
        GattService         idService(IdService::ID_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.gattServer().addService(idService);
    }

    void updateIdState(char* id) {
        uint16_t length;
        length = strlen(id)+1;
        ble.gattServer().write(idState.getValueHandle(), (uint8_t *)id, length);
    }

    // void readFirstValue(char* buffer) {
    //     // uint8_t buffer[10];
    //     int i;
    //     uint16_t lenght=sizeof(uint8_t);
    //     ble.gattServer().read(idState.getValueHandle(), (uint8_t*)buffer, &lenght); 
    //     std::cout<< buffer[0] <<std::endl;
    // }

private:
    BLE                              &ble;
    ReadOnlyArrayGattCharacteristic	<char,10>  idState;
};

#endif /* #ifndef __BLE_ID_SERVICE_H__ */

