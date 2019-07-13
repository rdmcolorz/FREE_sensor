import serial

esp = serial.Serial('/dev/cu.SLAB_USBtoUART')

def get_data_from_sdcard():
    esp.write(b'1')
    while True:
        line = esp.readline()
        if line:
            #line = line.decode("utf-8")
            print(line)
            # esp.close()
            # break

get_data_from_sdcard()