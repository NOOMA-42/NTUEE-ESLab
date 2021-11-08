"""
Reference the report.pdf on the github for more details
"""
from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
import struct

# Notif Status. if indication needed, x02 rather than x01.
NOTIF_ON = struct.pack('<bb', 0x01, 0x00)
NOTIF_OFF = struct.pack('<bb', 0x00, 0x00)

# user config *** where you might need to change
DEVICE_NAME = "Button_Paul"
BUTTON_SERVICE_UUID = 0xA000
BUTTON_CHAR_UUID = 0xA001
ID_SERVICE_UUID = 0xA002
LED_SERVICE_UUID = 0xA004
LED1_STATE_CHARACTERISTIC_UUID = 0xA005
LED2_STATE_CHARACTERISTIC_UUID = 0xA006

class ScanDelegate(DefaultDelegate):
	"""
	DefaultDelegate receive Bluetooth message asynchronously, 
	Scanner is to scan for LE devices which are broadcasting data
	withDelegate stores a reference to a delegate object, which receives callbacks when broadcasts from devices are received
	"""
	def __init__(self):
		DefaultDelegate.__init__(self)
	def handleDiscovery(self, dev, isNewDev, isNewData):
		if isNewDev:
			print("Discovered device", dev.addr)
		elif isNewData:
			print("Received new data from", dev.addr)

class PeripheralDelegate(DefaultDelegate):
	"""
	Register a delegate as peripheral for notification 
	"""
	def __init__(self, handle):
		DefaultDelegate.__init__(self)
		print("handleNotification init")
		self.hndl = handle
		# ... initialise here

	def handleNotification(self, cHandle, data):
	# ... perhaps check cHandle
		print(cHandle)
		print(data)
	# ... process 'data'

scanner = Scanner().withDelegate(ScanDelegate()) 
devices = scanner.scan(3.0)
n = -1
number = 0
connection_success = False
for dev in devices:
	print ("%d: Device %s (%s), RSSI=%d dB" % (n, dev.addr, dev.addrType, dev.rssi))
	n += 1
	for (adtype, desc, value) in dev.getScanData():
		if (value == DEVICE_NAME):
			number = n
			print('===============================')
			print ("%d, desc is %s, value is %s" % (n, desc, value))
			print('===============================')
			connection_success = True
		else:
			print ("%s, %s" % (desc, value))

# automatically link the device to save time
if not connection_success:
	raise BaseException("Please Reset the peripheral device: Mbed")
print('Device', number)
print(devices[number].addr)
"""
# Or select devices yourself 
number = input('Enter your device number: ')
"""

print ("Connecting...")
dev = Peripheral(devices[number].addr, 'random')

print ("Services...")
for svc in dev.services:
	print (str(svc))
try:
	print ("Getting UUID...")
	ButtonService= dev.getServiceByUUID(UUID(BUTTON_SERVICE_UUID))
	IdService = dev.getServiceByUUID(UUID(ID_SERVICE_UUID))
	LedService = dev.getServiceByUUID(UUID(LED_SERVICE_UUID))
	
	# Button Service
	print ("Getting Button...")
	ch1 = ButtonService.getCharacteristics()[0]
	button_service_handle_cccd = ch1.valHandle + 1 # CCCD handle is in the last row of service
	button_peri = dev.withDelegate(PeripheralDelegate(ch1))
	dev.writeCharacteristic(button_service_handle_cccd, NOTIF_OFF)
	
	# ID Service
	print ("Getting ID...")
	ch2 = IdService.getCharacteristics()[0]
	
	# Active LED, LED1, read notify 
	print ("Getting LED1...")
	ch3 = LedService.getCharacteristics()[0] 
	LED1_service_handle_cccd = ch3.valHandle + 1 # CCCD handle is in the last row of service
	LED1_peri = dev.withDelegate(PeripheralDelegate(ch3))
	dev.writeCharacteristic(LED1_service_handle_cccd, NOTIF_OFF)
	# Actuated LED, LED2, read write 
	print ("Getting LED2...")
	ch4 = LedService.getCharacteristics()[1] 
	
	# Functionality Testing
	print("\nStudent ID: %s" % ch2.read())
	while(True):
		testWhich = input("Test Service, Select 1 for LED2 toggle, 2 for Button Notification, 3 for LED1 notification, Ctrl+C to switch, -1 or press Ctrl+D for leaving\n")
		if testWhich == 1: # LED2 toggle
			while(True):
				try:
					state = str(input('Enter led state: \n'))
					if state == '1':
						ch4.write(struct.pack('<B', 0x01))
					elif state=='0':
						ch4.write(struct.pack('<B', 0x00))
					else:
						print ('Invalid LED2 state num. Enter 1 or 0')
				except KeyboardInterrupt:
					print('Switch Current Testing!')
					break
		elif testWhich == 2: # Button Notification
			dev.writeCharacteristic(button_service_handle_cccd, NOTIF_ON)
			# Upon receiving notification, dev.waitForNotifications return true. 
			while True: 
				# Continuos receiving notification
				try:
					if button_peri.waitForNotifications(3.0):
						print("Test Button Notification")
						continue
					print ("Waiting Button...")
				except KeyboardInterrupt:
					print('Switch Current Testing!')
					break
		elif testWhich == 3: # LED1 notification
			dev.writeCharacteristic(LED1_service_handle_cccd, NOTIF_ON)
			# Upon receiving notification, dev.waitForNotifications return true. 
			while True: 
				try:
					if LED1_peri.waitForNotifications(3.0):
						print("Test LED1 Notification")
						continue
					print ("Waiting LED1...")
				except KeyboardInterrupt:
					print('Switch Current Testing!')
					break
		elif testWhich == -1:
			# Disconnect the peripheral	
			break
		else:
			print("Invalid Service Num")
			continue
finally:
	print("Disconnect the peripheral")
	dev.disconnect()