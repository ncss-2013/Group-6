#!python

# Imports
import serial
import simplekml

# Helper function to convert an RGB triplet to a hex string
def to_hex(rgb):
    return format((rgb[0]<<16)|(rgb[1]<<8)|rgb[2], '06x')

# Set up basic line style
linestyle = simplekml.LineStyle(width=4)
polystyle = simplekml.PolyStyle(fill=0, outline=0)
style = simplekml.Style(linestyle=linestyle, polystyle=polystyle)

# List of coords. This will be created by the reciever when complete.
coords = [
	(-112.2550785337791,36.07954952145647,2357),
	(-112.2549277039738,36.08117083492122,2357),
	(-112.2552505069063,36.08260761307279,2357),
	(-112.2564540158376,36.08395660588506,2357),
	(-112.2580238976449,36.08511401044813,2357),
	(-112.2595218489022,36.08584355239394,2357),
	(-112.2608216347552,36.08612634548589,2357),
	(-112.262073428656,36.08626019085147,2357),
	(-112.2633204928495,36.08621519860091,2357),
	(-112.2644963846444,36.08627897945274,2357),
	(-112.2656969554589,36.08649599090644,2357),
]

# Initiate the KML object that will be used as output
kml = simplekml.Kml()

# Loop over coords to create line segments that can be coloured individually
for i in range(1, len(coords)):
	lineseg = kml.newlinestring()
	lineseg.coords = [coords[i-1], coords[i]]
	lineseg.extrude = 1
	lineseg.altitudemode = simplekml.AltitudeMode.absolute
	lineseg.style = style






kml.save('Test3.kml')