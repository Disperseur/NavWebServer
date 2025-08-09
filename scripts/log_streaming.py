import serial
from time import sleep

FILE_DUMMY1         = "log.txt"
FILE_NAV1_TEYCHAN   = "nav1.txt"
FILE_TEST           = "test.txt"

used_file = FILE_DUMMY1

port = serial.Serial("COM17", 460800)


file_log = open("scripts/"+used_file, 'r')
file_log_list = file_log.readlines()
file_log.close()


while(True):
    for i in range(len(file_log_list)):
        print(file_log_list[i].encode())
        port.write(file_log_list[i].encode())
        sleep(0.1)