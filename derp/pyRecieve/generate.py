import simplekml
import time

def rgb2hex(rgb):
	"Returns an RGB string from an (r, g, b) tuple"
	return format((rgb[2]<<16)|(rgb[1]<<8)|rgb[0], '06x')

class Graph(Object):
	def __init__(self, linewidth=4):
		# Base KML object
		self.kml = simplekml.Kml()

		# Basic styles
		self.linestyle = simplekml.LineSyle(width=linewidth)
		self.polystyle = simplekml.PolyStyle(fill=0, outline=0)

		# Keep track of previous segment
		self.previous = None
		self.logfile = open('kmlgen.log', 'a')

		# Log an opening statement to logfile
		self.logfile.write('New KML generation started at {0}.\n'.format(time.time()))

		# Create a general style to apply to the points		
		linestyle = simplekml.LineStyle(width=4)
		polystyle = simplekml.PolyStyle(fill=0, outline=0)
		stylemap = simplekml.StyleMap()
		stylemap.normalstyle.polystyle = polystyle
		stylemap.normalstyle.linestyle = linestyle
		stylemap.normalstyle.iconstyle.scale = 0
		stylemap.normalstyle.labelstyle.scale = 0.5
		stylemap.highlightstyle.polystyle = polystyle
		stylemap.highlightstyle.linestyle = linestyle
		stylemap.highlightstyle.iconstyle.scale = 0
		self.stylemap = stylemap
		# ^ yay ugly code

	def add_point(self, point):
		pass

	def save(self, filename='balloon.kml'):
		self.kml.save(filename)

	def get_temp_colour(self, pos):
		if pos>100 or pos<0:
			raise ValueError("`pos` value passed ({0}) is out of boundaries (0-100).".format(pos))
		b = math.sin(math.radians(((100-pos)/100)*90))*255
		r = math.sin(math.radians((pos/100)*90))*255
		return (int(r), 0, int(b))

class Point(Object):
	def __init__(self):
		# In decimal degrees
		self.longitude = 0
		self.latitude = 0
		# Absolute above sea level
		self.altitude = 0
		# 0 - 100
		self.temperature = 0
		# *NIX timestamp (time.time())
		self.timestamp = 0