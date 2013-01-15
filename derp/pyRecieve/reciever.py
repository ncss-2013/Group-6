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



# Initiate the graph API
graph = earth.Graph()




## HELPER FUNCTONS
# Parse the GPX timestamp into something useable
def gpx_parse_timestamp(timestring):
	time_struct = [i for i in time.strptime(timestring,
		'%Y-%m-%dT%H:%M:%SZ')]
	time_struct[3] += 12 # Correct wierd time offset
	timestamp = time.mktime(tuple(time_struct))
	return timestamp



# Loop over all the GPS datapoints from the iPad
for trkpt in gpx.find_all('trkpt'):
	point = earth.Point()
	point.latitude = trkpt['lat']
	point.longitude = trkpt['lon']
	point.altitude = trkpt.ele.text

	graph.add_point(point)



print('about to save')
graph.save('flyover.kml')
print('saved that nigger')