# ESP32-DHT11-WebServer ❄️
A simple thermoemeter and humidity meter created using an esp32, a DHT11 sensor, and an OLED screen

## component 🔌
- 1 x ESP32 DevKit (i used a *ESP32-WROOM-32D*)
- 1 x DHT11
- 2 x 128x64 OLED display
- 1 x push button
- 1 x 10K ohm resistor
- 2 x breadboard
- some cable

## pinout 📌
The pinout diagram of my DevKit is [this](https://land-boards.com/blwiki/images/6/61/ESP32-Pinout.jpg).
- D13 --> button (input)
- D22 --> OLED (SCL)
- D21 --> OLED (SDA)
- D5 --> DHT11 (input)

## photo 📸
<img src="https://github.com/pianoforte33/ESP32-DHT11-WebServer/assets/79628894/fefbb345-e0b5-4207-898a-21aa77068877" height=400 width=256/> <br>
<img src="https://github.com/pianoforte33/ESP32-DHT11-WebServer/assets/79628894/5d02c878-bfa0-4b7a-a89a-3598a21b85d9" height=256 width=400/>

## troubleshooting 🚩
- Rember to replace the SSID and the password with your credential.
- To display the ip address to connect to check the serial monitor.
- Use a pull down resistor
