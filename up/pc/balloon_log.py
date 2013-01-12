import serial

ORIGIN_LAT = -121998
ORIGIN_LONG = 544299
COM = 'COM9'
FILENAME = "log.txt"

ser = serial.Serial(COM, 9600)


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
    # TODO: Write to kml

    
