import earth
import serial
import time

ser = serial.Serial(input("Enter COM port: "), 9600)

# Wait for arduino bootup to complete
time.sleep(2)

