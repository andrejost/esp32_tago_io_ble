# ESP32 BLE Tagio Upload

Simple project to upload data to the tago.io Dashboard based on inputs received from and ESP-32.

## Description

The project is very simple. But Involves some interesting components using Tago.io and ESP32, BLE and Wifi and sending data to the Tago.io cloud.

## Components

This project leverages the following components:

- ESP32
- USB Cable (Micro USB to USB)
- Tago.io Platform and Dashboard ([See Tago.io Here](https://tago.io/))

### Simplified Flow

1. ESP32 Starts and tries to connect to the defined Wifi
2. After connecting to the Wifi, it creates a BLE definition.
3. Awaits for input from the Application in the Source Device (Your Cell Phone)
4. The input is sent to the ESP32 using BLE
5. The input is sent from the ESP32 to the Tago.io API
6. An analysis created in the Tago.io API is used to transform the data from Number into text
7. Data received is shown in a dashboard

## What to watch when building the project

- This project is based on BLE (Bluetooth Low Energy) which is different from the normal Bluetooth. So use speficic BLE Apps in your cell phone to interact. In my case, I used **nrf Connect** from Nordic Semmiconductor
- Please put all the necessary credentials in the **Credentials_example.h** file (Tago.io Device Token, Wifi Password and BLE Necessary UUIDs)
- File **analysis_example.js** contains the example to be used in Tago.io as an analysis example so you can transform data when showing it in a dashboard.
- Please look at the mentioned file to resize the partitions of the ESP32 because the standard configuration might not support the size of the program (See file **huge_app.csv**)
- IMO, I did not find the Image Feature in the Tago.io Dashboard very straightforward. I did inclue some example PNG which you can use for the example in the folder **Images_traffic_light**

## Useful links to support used during the project

- [Instrutables Link with good explanation on how to use the ESP32 BLE](https://www.instructables.com/ESP32-Bluetooth-Low-Energy/)
- [nrfConnect Mobile App for iOs - Used for connecting the iPhone to the ESP32 using BLE](https://apps.apple.com/us/app/nrf-connect-bluetooth-app/id1054362403)
- [How to resize the ESP32 Partitions](https://docs.platformio.org/en/latest/platforms/espressif32.html#external-ram-psram)

## DISCLAIMER

This project was created for educational purposes and I do not recommend or endorse on its current form to be used in a live environment. Use at your own discretion.