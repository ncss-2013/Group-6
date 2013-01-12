import math
import simplekml
import time

def rgb2hex(rgb):
	"Returns an RGB string from an (r, g, b) tuple"
	return format((rgb[2]<<16)|(rgb[1]<<8)|rgb[0], '06x')

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
		polystyle = simplekml.PolyStyle(fill=0, outline=0)
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

	def add_point(self, point):
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
			if point.temperature is not None:
				rgbtemp = self.get_temp_colour(point.temperature)
			else:
				rgbatemp = (0, 255, 0)
			lineseg.linestyle.color = 'ff'+rgb2hex(rgbtemp)

			# Create a new point (for label, etc)
			p = geom.newpoint()
			p.coords = [pointcoord]
			p.altitudemode = simplekml.AltitudeMode.absolute

		self.points.append(point)
		self.logfile.write('{0} {1} {2} {3} {4}\n'.format(
			point.longitude,
			point.latitude,
			point.altitude,
			point.temperature,
			point.timestamp
		))
		self.logfile.flush()

	def save(self, filename='balloon.kml'):
		self.kml.save(filename)

	def get_temp_colour(self, pos):
		if pos>100 or pos<0:
			raise ValueError("`pos` value passed ({0}) is out of boundaries (0-100).".format(pos))
		b = math.sin(math.radians(((100-pos)/100)*90))*255
		r = math.sin(math.radians((pos/100)*90))*255
		return (int(r), 0, int(b))

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