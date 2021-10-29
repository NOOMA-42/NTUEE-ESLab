from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
import struct
"""
DefaultDelegate receive Bluetooth message asynchronously, 

Scanner is to scan for LE devices which are broadcasting data
withDelegate stores a reference to a delegate object, which receives callbacks when broadcasts from devices are received
"""
class ScanDelegate(DefaultDelegate):
	def __init__(self):
		DefaultDelegate.__init__(self)
	def handleDiscovery(self, dev, isNewDev, isNewData):
		if isNewDev:
			print("Discovered device", dev.addr)
		elif isNewData:
			print("Received new data from", dev.addr)

class PeripheralDelegate(DefaultDelegate):
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
n=0
for dev in devices:
	print ("%d: Device %s (%s), RSSI=%d dB" % (n, dev.addr, dev.addrType, dev.rssi))
	n += 1
	for (adtype , desc , value) in dev.getScanData():
		print (" %s = %s" % (desc, value))
number = input('Enter your device number: ')
print('Device', number)
print(devices[number]. addr)

print ("Connecting...")
dev = Peripheral(devices[number].addr, 'random')

print ("Services...")
for svc in dev.services:
	print (str(svc))
try:
	print (str(dev.getServiceByUUID(UUID(0xfff0))))
	testService = dev.getServiceByUUID(UUID(0xfff0))
	for ch in testService.getCharacteristics():
		print (str(ch))
			
	ch = dev.getCharacteristics(uuid=UUID(0xfff1))[0] # = ch.valHandle
	#if (ch.supportsRead()):
	#	print (ch.read())
	print(ch.valHandle+1)
	notifOn = b"\x01\x00"
	notifOff = b"\x01\x00"
	print(dev.readCharacteristic(0x00b7)) #
	dev.writeCharacteristic(ch.valHandle, b"\x01\x00")
	#print(dev.readCharacteristic(0x00b8)) #
	#dev.writeCharacteristic(0x00b8, struct.pack('<bb', 0x01, 0x00))
	#dev.writeCharacteristic(0x00b8, notifOn)
	custom_service_handle_cccd = ch.valHandle + 1 #??? why 1 not 2
	print("len: ", len(dev.getCharacteristics(uuid=UUID(0xfff1))))
	dev = dev.withDelegate(PeripheralDelegate(custom_service_handle_cccd))
	dev.writeCharacteristic(custom_service_handle_cccd, notifOn)
	#ch.write(bytes("Hello world\n"))

	while True:
		if dev.waitForNotifications(3.0):
			print("test notification")
			continue
		print ("Waiting...")
finally:
	dev.disconnect()
