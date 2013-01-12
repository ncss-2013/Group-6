import earth
import serial
import time

# Helper Functions
order_dict = ['d','m','s']
def parse_coord():
	print('pc')
	out = {}
	buf = ''
	for part in order_dict:
		while buf[-1] != '|':
			buf += ser.read().decode('utf-8')
		out[part] = float(buf[:-1])
		buf = ''
	out['s'] /= 10
	print(out)
	return out


def dms2dd(dms):
	return dms[d] + float(dms[m])/60 + float(dms[s])/3600

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

	point.latitude = parse_coord()
	print(point.latitude)

	time.sleep(30)
	break
	
