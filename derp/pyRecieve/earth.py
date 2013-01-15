import math
import simplekml
import time
import random

def rgb2hex(rgb):
	"Returns an RGB string from an (r, g, b) tuple"
	return format((rgb[2]<<16)|(rgb[1]<<8)|rgb[0], '06x')

def get_dist(lat1, lon1, lat2, lon2):
	#lat=neg
	dist = math.sqrt(
		abs(abs(float(lat1))-abs(float(lat2)))**2 +
		abs(abs(float(lon1))-abs(float(lon2)))**2
	)
	return dist

class Graph(object):
	def __init__(self, linewidth=4):
		# Base KML object
		self.kml = simplekml.Kml()

		# Basic styles
		self.linestyle = simplekml.LineStyle(width=linewidth)
		self.polystyle = simplekml.PolyStyle(fill=0, outline=0)

		# Keep track of previous segments
		self.points = []
		self.logfile = open('kmlgen.log', 'a')

		# Log an opening statement to logfile
		self.logfile.write('!! New KML generation started at {0}. !!\n'.format(time.time()))

		# Create a general style to apply to the points		
		linestyle = simplekml.LineStyle(width=4)
		polystyle = simplekml.PolyStyle(fill=0, outline=1)
		stylemap = simplekml.StyleMap()
		stylemap.normalstyle.polystyle = polystyle
		stylemap.normalstyle.linestyle = linestyle
		stylemap.normalstyle.iconstyle.scale = 0
		stylemap.normalstyle.labelstyle.scale = 0
		stylemap.highlightstyle.polystyle = polystyle
		stylemap.highlightstyle.linestyle = linestyle
		stylemap.highlightstyle.iconstyle.scale = 0
		self.stylemap = stylemap
		# ^ yay ugly code

		tour = self.kml.newgxtour(name="Flight Sim Whatsit")
		self.playlist = tour.newgxplaylist()

		self.colourstep = 0

		self.slowangle = 0.

	def add_point(self, point):
		if len(self.points) != 0:
			prev = self.points[-1]
			flyto = self.playlist.newgxflyto(gxduration=3.0)#200.0*get_dist(prev.latitude, prev.longitude, point.latitude, point.longitude))
			flyto.camera.longitude = prev.longitude
			flyto.camera.latitude  = prev.latitude
			flyto.camera.altitude  = prev.altitude
			_w = float(point.longitude) - float(prev.longitude)
			_h = float(point.latitude) - float(prev.latitude)
			flyto.camera.heading = math.degrees(math.atan2(_w, _h))
			flyto.camera.tilt = 90 + math.degrees(math.atan(
				(float(point.altitude) - float(prev.altitude)) / 111000.
				/ math.sqrt(_w**2 + _h**2)));
			self.slowangle=(self.slowangle-flyto.camera.heading+180.)%360.+flyto.camera.heading-180.
			#tempangle = (flyto.camera.heading-self.slowangle+180)%360 + self.slowangle-180
			self.slowangle = self.slowangle*0.6 + flyto.camera.heading*0.4
			flyto.camera.roll = -math.degrees(math.atan((flyto.camera.heading-self.slowangle)/90))

			flyto.gxflytomode = simplekml.GxFlyTo.smooth


		self.points.append(point)
		self.logfile.write('{0} {1} {2} {3}\n'.format(
			point.longitude,
			point.latitude,
			point.altitude,
			#point.temperature,
			point.timestamp
		))
		self.logfile.flush()

		# self.colourstep += 4
		# self.colourstep %= 360

	def add_point_old(self, point):
		#Create container for the point
		ftime = self.get_time_formatted(point.timestamp)
		geom = self.kml.newmultigeometry(name=ftime) #TODO: add name (number/temp?)
		geom.stylemap = self.stylemap

		# Get the previous point so line can be drawn
		if len(self.points) != 0:
			prev = self.points[-1]

			# Create new line segment
			lineseg = geom.newlinestring()
			pointcoord = (point.longitude, point.latitude, point.altitude)
			lineseg.coords = [
				(prev.longitude, prev.latitude, prev.altitude),
				pointcoord
			]
			lineseg.extrude = 1
			lineseg.altitudemode = simplekml.AltitudeMode.absolute
			
			rgbtemp = self.fancy_rainbowz()
			lineseg.linestyle.color = 'ff'+rgb2hex(rgbtemp)

			# Create a new point (for label, etc)
			p = geom.newpoint()
			p.coords = [pointcoord]
			p.altitudemode = simplekml.AltitudeMode.absolute

		self.points.append(point)
		self.logfile.write('{0} {1} {2} {3}\n'.format(
			point.longitude,
			point.latitude,
			point.altitude,
			#point.temperature,
			point.timestamp
		))
		self.logfile.flush()

		self.colourstep += 4
		self.colourstep %= 360

	def fancy_rainbowz(self):
		r = max(0, int(math.sin(math.radians(self.colourstep))*255))
		g = max(0, int(math.sin(math.radians(self.colourstep+120))*255))
		b = max(0, int(math.sin(math.radians(self.colourstep+240))*255))
		return (r,g,b)

	def save(self, filename='balloon.kml'):
		self.kml.save(filename)

	def get_time_formatted(self, timestamp, format="%H:%M:%S"):
		lt = time.localtime(timestamp)
		return time.strftime(format, lt)

class Point(object):
	def __init__(self):
		# In decimal degrees
		self.longitude = 0
		self.latitude = 0
		# Absolute above sea level
		self.altitude = 0
		# 0 - 100
		self.temperature = None
		# *NIX timestamp (time.time())
		self.timestamp = 0