import earth
import serial
import time

# Helper Functions
def get_value():
	# Get the value from serial, stopping on pipe.
	buff = ''
	b = ser.read().decode('utf-8')
	while b != '|':
		buff += b
		b = ser.read().decode('utf-8')
	return float(buff)

#hardcoded because wew
lat = -33.888174
lon = 151.194025

order_dict = ['d','m','s']
def parse_coord(islat=True):
	out = {}
	buf = ''
	# Loop through D, M , S
	for part in order_dict:
		# Get the next value, store in the whatsit
		out[part] = get_value()
	# seconds are being passed *10, bring back teh decimal point
	out['s'] /= 10
	# Convert the DM dict to a GE decimal degree float
	delta = dms2dd(out)
	return (lat if islat else lon) + delta

# Convert a dict of DMS to decimal degrees
def dms2dd(dms):
	return dms['d'] + float(dms['m'])/60 + float(dms['s'])/3600

# Create Graph instance we can use to do stuff
graph = earth.Graph()

# Open serial port
ser = serial.Serial(input("Enter COM port: "), 9600)

# Wait for arduino bootup to complete
time.sleep(2)

# Listen on serial untill program is quit
while True:
	# Wait for start char
	while ser.read() != b'~': pass

	# Create a Point object to hold the data
	point = earth.Point()

	# Make sure we have a latitude header
	if ser.read(3) != b'LAT':
		print('Error: LAT not recieved')
		break
	# Grab the latitude
	point.latitude = parse_coord()
	print("Latitude:",point.latitude)
	
	# Do the same for the longitude
	if ser.read(3) != b'LON':
		print('Error: LON not recieved')
		break
	point.longitude = parse_coord()
	print("Longitude:",point.longitude)
	
	# Aaaaaand the altitude. wew.
	if ser.read(3) != b'ALT':
		print('Error: ALT not recieved')
		break
	point.altitude = get_value()
	print("Altitude:",point.altitude)
	
	# Temperature. I should probably make a loop for this or something
	if ser.read(3) != b'TMP':
		print('Error: ALT not recieved')
		break
	point.temperature = get_value()
	print("Temperature:",point.temperature)
	
	if ser.read(3) != b'END':
		print('Error: END not recieved')
		break
	print("END recieved\n")	

	#break
	
