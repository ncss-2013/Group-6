#!python

# Imports
import Earth
import time

# List of coords. This will be created by the reciever when complete.
tempdata = [
	(-112.2656969554589,36.09049599090644,4000, 10),
	(-112.2656969554589,36.10049599090644,4000, 20),
	(-112.2656969554589,36.12049599090644,4000, 76),
	(-112.2656969554589,36.13049599090644,4000, 52),
	(-112.2656969554589,36.14049599090644,4000, 9),
	(-112.2656969554589,36.15049599090644,4000, 18),
	(-112.2656969554589,36.16049599090644,4000, 20),
	(-112.2656969554589,36.17049599090644,4000, 21),
	(-112.2656969554589,36.18049599090644,4000, 39),
	(-112.2656969554589,36.19049599090644,4000, 50),
	(-112.2656969554589,36.20049599090644,4000, 92),
	(-112.2656969554589,36.21049599090644,4000, 86),
	(-112.2656969554589,36.22049599090644,4000, 21),
	(-112.2656969554589,36.23049599090644,4000, 7),
	(-112.2656969554589,36.24049599090644,4000, 4),
	(-112.2656969554589,36.25049599090644,4000, 1),
	(-112.2656969554589,36.26019599090644,4000, 0),
	(-112.2656969554589,36.27029599090644,4000, 40),
	(-112.2656969554589,36.28049599090644,4000, 88),
	(-112.2656969554589,36.29049599090644,4000, 87),
	(-112.2656969554589,36.30049599090644,4000, 33),
	(-112.2656969554589,36.31049599090644,4000, 87),
	(-112.2656969554589,36.32049599090644,4000, 36),
	(-112.2656969554589,36.33049599090644,4000, 54),
	(-112.2656969554589,36.34049599090644,4000, 42),
	(-112.2656969554589,36.35049599090644,4000, 64),
	(-112.2656969554589,36.36049599090644,4000, 79),
	(-112.2656969554589,36.37049599090644,4000, 64),
	(-112.2656969554589,36.38049599090644,4000, 11),
	(-112.2656969554589,36.39049599090644,4000, 21),
	(-112.2656969554589,36.40049599090644,4000, 46),
	(-112.2656969554589,36.41049599090644,4000, 49),
	(-112.2656969554589,36.42049599090644,4000, 23),
	(-112.2656969554589,36.43049599090644,4000, 78),
	(-112.2656969554589,36.44049599090644,4000, 65),
	(-112.2656969554589,36.45049599090644,4000, 100)
]

graph = Earth.Graph()
for datapoint in tempdata:
	point = Earth.Point()
	point.longitude = datapoint[0]
	point.latitude = datapoint[1]
	point.altitude = datapoint[2]
	point.temperature = datapoint[3]
	point.timestamp = time.time()

	graph.add_point(point)

	graph.save()