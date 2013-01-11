#!python

# Imports
import copy
import math
import serial
import simplekml

# Helper function to convert an RGB triplet to a hex string
def to_hex(rgb):
	return format((rgb[2]<<16)|(rgb[1]<<8)|rgb[0], '06x')

# Convert 0-100 range to pretty blue-red grad BecauseWhyNot
def get_grad_colour(pos):
	if pos>100 or pos<0:
		raise ValueError("`pos` value passed ({0}) is out of boundaries (0-100).".format(pos))
	b = math.sin(math.radians(((100-pos)/100)*90))*255
	r = math.sin(math.radians((pos/100)*90))*255
	return (int(r), 0, int(b))

# Set up basic line style
linestyle = simplekml.LineStyle(width=4)
polystyle = simplekml.PolyStyle(fill=0, outline=0)

# List of coords. This will be created by the reciever when complete.
coords = [
	(-112.2656969554589,36.09049599090644,4000),
	(-112.2656969554589,36.10049599090644,4000),
	(-112.2656969554589,36.12049599090644,4000),
	(-112.2656969554589,36.13049599090644,4000),
	(-112.2656969554589,36.14049599090644,4000),
	(-112.2656969554589,36.15049599090644,4000),
	(-112.2656969554589,36.16049599090644,4000),
	(-112.2656969554589,36.17049599090644,4000),
	(-112.2656969554589,36.18049599090644,4000),
	(-112.2656969554589,36.19049599090644,4000),
	(-112.2656969554589,36.20049599090644,4000),
	(-112.2656969554589,36.21049599090644,4000),
	(-112.2656969554589,36.22049599090644,4000),
	(-112.2656969554589,36.23049599090644,4000),
	(-112.2656969554589,36.24049599090644,4000),
	(-112.2656969554589,36.25049599090644,4000),
	(-112.2656969554589,36.26019599090644,4000),
	(-112.2656969554589,36.27029599090644,4000),
	(-112.2656969554589,36.28049599090644,4000),
	(-112.2656969554589,36.29049599090644,4000),
	(-112.2656969554589,36.30049599090644,4000),
	(-112.2656969554589,36.31049599090644,4000),
	(-112.2656969554589,36.32049599090644,4000),
	(-112.2656969554589,36.33049599090644,4000),
	(-112.2656969554589,36.34049599090644,4000),
	(-112.2656969554589,36.35049599090644,4000),
	(-112.2656969554589,36.36049599090644,4000),
	(-112.2656969554589,36.37049599090644,4000),
	(-112.2656969554589,36.38049599090644,4000),
	(-112.2656969554589,36.39049599090644,4000),
	(-112.2656969554589,36.40049599090644,4000),
	(-112.2656969554589,36.41049599090644,4000),
	(-112.2656969554589,36.42049599090644,4000),
	(-112.2656969554589,36.43049599090644,4000),
	(-112.2656969554589,36.44049599090644,4000),
	(-112.2656969554589,36.45049599090644,4000)
]

# Initiate the KML object that will be used as output
kml = simplekml.Kml()

# Loop over coords to create line segments that can be coloured individually
for i in range(1, len(coords)):
	lineseg = kml.newlinestring()
	lineseg.coords = [coords[i-1], coords[i]]
	lineseg.extrude = 1
	lineseg.altitudemode = simplekml.AltitudeMode.absolute
	lineseg.polystyle = polystyle
	lineseg.linestyle = copy.deepcopy(linestyle)
	c = get_grad_colour(int((100/len(coords))*i))
	lineseg.linestyle.color = 'ff' + to_hex(c)






kml.save('balloon.kml')