import serial
import earth
import time

ORIGIN_LAT = -121998
ORIGIN_LONG = 544299
FILENAME = "log.txt"

ser = serial.Serial(input("Enter COM port: "), 9600)

graph = earth.Graph()
while 1:
    data = [0 for i in range(3)]
    commas = 0
    cur = 0
    while commas < 3:
        while !ser.available:
            pass
        char = ser.read()
        if char == ',':
            data[commas] = cur
            commas += 1
        else:
            cur *= 10
            cur += int(chr(ser.read()))
    f = open(FILENAME, 'a').write(','.join(data)+',\n')
    del f
    
    point = earth.Point()
    point.longitude = data[0]
    point.latitude = data[1]
    point.altitude = data[2]
    #point.temperature = data[3]
    point.timestamp = time.time()

    graph.add_point(point)
    
    graph.save()
