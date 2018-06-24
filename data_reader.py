##############
## Script listens to serial port and writes contents into a file
##############

import serial

serial_port = 'COM4';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "example.txt";

ser = serial.Serial(serial_port, baud_rate)
while True:
    output_file = open(write_to_file_path, "a");
    line = ser.readline();
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line);
    output_file.write(line);
    output_file.close();
