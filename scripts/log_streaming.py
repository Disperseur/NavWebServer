import serial
from time import sleep

port = serial.Serial("COM17", 460800)


file_log = open("scripts/log.txt", 'r')
file_log_list = file_log.readlines()
file_log.close()


while(True):
    for i in range(len(file_log_list)):
        port.write(file_log_list[i].encode())
        sleep(0.5)