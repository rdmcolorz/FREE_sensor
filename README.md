# FREE Sensor

FREE Sensor is a device firefighters use to detect harmful gasses and to track status of individual firefighters. 

## IBM Cloud services

`IBM Watson IoT Platform` for streaming IoT data and managing IoT devices  
`Cloudant` database for storing history IoT data  
`IBM Watson Studio / Data refinery` for data transformation and parsing  
`IBM Watson Studio / Cognos Dashboard` for data visualization and analytics  

## Getting Started with IBM Cloud

Follow this [tutorial](https://developer.ibm.com/recipes/tutorials/run-an-esp8266arduino-as-a-iot-foundation-managed-device/) to connect ESP8266 with the IoT platform. For current setup, on the IBM Watson IoT Platform, change `Default Rule` in `Security` to `TLS Optional` to connect with ESP.

To setup history database for IoT data, go to `Extensions` > `Histroical Data Storage` and choose the `Cloudant` service you have created.
Open `IBM Watson Studio` to get access to the database and add connection. Now you will have access to the data and use data refinery to make tranform jobs on the historical data from devices.

To add Cognos dashboard to the project, on the top right click `Add to project` and select `Dashboard`, here you can setup your Cognos Dashboard embedded service and start adding data to it. Had a issue initiating the dashboard, but once you restart watson studio or the browser the dashboard will work.

Please message [Taylor](rainbowwww5@gmail.com) to be added to the Watson Studio project.

### Prerequisites

For Taylor's setup you will be using these hardware products:

* [NodeMCU Esp8266](https://www.handsontec.com/pdf_learn/esp8266-V10.pdf) - Main microprocessor
* [Beginner's Guide to ESP8266](https://tttapa.github.io/ESP8266/Chap01%20-%20ESP8266.html)
* [Adafruit SHT31-D Temperature & Humidity Sensor Breakout](https://learn.adafruit.com/adafruit-sht31-d-temperature-and-humidity-sensor-breakout/assembly) - Temperature / Humidity Sensor breakout board
* [MicroSD Shield Adapter](https://www.ebay.com/i/173320605763?chn=ps&norover=1&mkevt=1&mkrid=711-117182-37290-0&mkcid=2&itemid=173320605763&targetid=503482151964&device=c&adtype=pla&googleloc=9032020&poi=&campaignid=1669934825&adgroupid=65058350099&rlsatarget=pla-503482151964&abcId=1139296&merchantid=6296724&gclid=CjwKCAjw6vvoBRBtEiwAZq-T1Y8n93J3ntjJk5OjkONkjNmSt5mo_SL-G2_fFggtqFEAWhj9vSMNGxoCKKIQAvD_BwE) - MicroSD card reader
* [MQ-7 Carbon Monoxide Sensor](https://www.amazon.com/HiLetgo-Carbon-Monoxide-Detection-Sensor/dp/B00LSECY2Y/ref=sr_1_2?keywords=mq-7+sensor&qid=1564435192&s=gateway&sr=8-2)
* [Powered Carbon Monoxide Sensor](https://cdn.sparkfun.com/datasheets/Sensors/Biometric/MQ-7%20Ver1.3%20-%20Manual.pdf) - First prototype sensor
* [Low Power Carbon Monoxide Sensor](https://www.spec-sensors.com/wp-content/uploads/2016/04/3SP_CO_1000-C-Package-110-109.pdf) - Second prototype sensor
* [Adafruit GPS breakout board](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ultimate-gps.pdf) - GPS module

## NodeMCU upload code

`FREE_sensor.ino`  
For the device to store data on the microSD.  
Run `upload_cloud.py` when the chip is connected through serial port to upload the data to IBM db2 database.

`FREE_sensor_stream.ino`  
For the code to stream into IBM Watson IoT platform.