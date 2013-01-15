#!python

# Imports
import earth
import math
import time
from bs4 import BeautifulSoup

#time.sleep(10)

## SETUP

# Get the iPad file to parse, read it, chuck in it BS
gpx_raw = open('iPadGPS.gpx').read()
gpx = BeautifulSoup(gpx_raw, "xml")

# Get the Phone file, etc
kml_raw = open('PhoneGPS.kml').read()
kml = BeautifulSoup(kml_raw, "xml")

# Initiate the graph API
graph = earth.Graph()

# Load up the datapoints from the KML data
kml_point_list = []
for coords in kml.find_all('coordinates'):
	for coord in coords.text.split('\n'):
		data = coord.strip().split(',')
		if len(data) != 3: continue
		kml_point_list.append(data)


## HELPER FUNCTONS
# Parse the GPX timestamp into something useable
def gpx_parse_timestamp(timestring):
	time_struct = [i for i in time.strptime(timestring,
		'%Y-%m-%dT%H:%M:%SZ')]
	time_struct[3] += 12 # Correct wierd time offset
	timestamp = time.mktime(tuple(time_struct))
	return timestamp

# Find the closest KML datapoint to a lat+lon
def get_dist(lat, lon, kmlpoint):
	#lat=neg
	dist = math.sqrt(
		abs(abs(float(lat))-abs(float(kmlpoint[1])))**2 +
		abs(abs(float(lon))-abs(float(kmlpoint[0])))**2
	)
	return dist



# Loop over all the GPS datapoints from the iPad
for trkpt in gpx.find_all('trkpt'):
	shortest = {'dist':-1,'data':None}
	# Find the closest datapoint from the phone data
	for i in range(len(kml_point_list)):
		kml_point = kml_point_list[i]
		dist = get_dist(trkpt['lat'], trkpt['lon'], kml_point)
		if shortest['dist'] == -1 or dist < shortest['dist']:
			shortest = {'dist':dist,'data':i}

	# Assign the closest KML datapoint the altitude and timestamp of the GPX datapoint
	kml_point_list[shortest['data']][2] = trkpt.ele.text
	kml_point_list[shortest['data']].append(gpx_parse_timestamp(trkpt.time.text))

# Loop through the kml data again (now with more info), and interpolate timestamps and altitudes
ts = 0
alt = 0
last = 0
for i in range(len(kml_point_list)):
	kmldata = kml_point_list[i]
	# If the altitude has been set already
	if len(kmldata) == 4 or i == len(kml_point_list)-1:
		# Check if we have no last data to start from
		if last == 0:
			alt = float(kmldata[2])
			ts = float(kmldata[3])
		# Stuff
		if i == len(kml_point_list)-1:
			kmldata[2] = kml_point_list[0][2]
			kmldata.append(kml_point_list[0][3])

		# Interpolate like a bawss
		diff = i - last
		#print(diff, alt, ts, i, last)
		for j in range(0 if last==0 else 1, diff):
			kml_point_list[j+last][2] = alt + ((float(kmldata[2])-alt)/diff)*j
			kml_point_list[j+last].append(ts + ((float(kmldata[3])-ts)/diff)*j)
			#print(alt - float(kmldata[2]))

		alt = float(kmldata[2])
		ts = float(kmldata[3])
		last = i

for kmldata in kml_point_list:
	#print(kmldata)
	point = earth.Point()
	point.latitude = kmldata[1]
	point.longitude = kmldata[0]
	point.altitude = kmldata[2]
	point.timestamp = kmldata[3]
	graph.add_point_old(point)
	#time.sleep(5)



print('about to save')
graph.save('interp.kml')
print('saved that nigger')