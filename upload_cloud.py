import pandas as pd
import ibm_db
import requests
import serial
import config # config.py file that contains the cloud credentials
from read_serial import *

db2_id = config.db2_id
conn = ibm_db.connect(db2_id['dsn'], db2_id['username'], db2_id['password']) 

PORT = '/dev/cu.SLAB_USBtoUART'
TABLE = 'new_table8'
TABLE_COLUMNS = '(id INT, temp FLOAT, humidity FLOAT, co FLOAT, timestamp TIMESTAMP)'
INSERT_COLUMNS = '(id, temperature, humidity, co, timestamp)'
TEST_TABLE_COLUMNS = '(temperature FLOAT, humidity FLOAT)'
TEST_INSERT_COLUMNS = '(temperature, humidity)'

create_table = "CREATE TABLE {}{}".format(TABLE, TEST_TABLE_COLUMNS)
insert_data_head = "INSERT INTO {}{} VALUES ".format(TABLE, TEST_INSERT_COLUMNS)

insert_sql = serial2sql(PORT, insert_data_head)
stmt = ibm_db.exec_immediate(conn, create_table)
stmet2 = ibm_db.exec_immediate(conn, insert_sql)