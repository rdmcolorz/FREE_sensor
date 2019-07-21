import serial
import time

 # the port connecting to ESP8266
PORT = '/dev/cu.SLAB_USBtoUART'
TABLE = 'new_table3'
COLUMNS = '(temperature, humidity)'

insert_data_head = "INSERT INTO {}{} VALUES ".format(TABLE, COLUMNS)

# TODO: Make a function that takes all / N data and 
# returns a sql command that uploads it to the IBM db
# have to know when to stop reading from serial port.
def serial2sql(port, sql_insert):

    esp = serial.Serial(PORT)
    esp.write(b'1')
    time.sleep(1)
    while esp.is_open:
        byte_line = esp.readline()[:-2]
        line = byte_line.decode('utf-8')
        if line == "END":
            break
        value = '(' + line + '),'
        sql_insert += value
        print(line)
    return sql_insert

sql = serial2sql(PORT, insert_data_head)
print(sql)
print(len(sql) - 53)