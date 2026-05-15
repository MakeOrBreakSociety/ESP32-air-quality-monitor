# ESP32 Air Quality Monitor (MQ‑135 + DHT22)
This is a simple air quality monitor that I built with simple and cheap parts

This was all built with the Waveshare ESP32-S3-Zero as a backbone.  but any similar ESP32 with the same footprint and analog on pin 6 will work!

The code was written by me completely at first but was well over 700 lines of code.  With the assistance of an LLM we compressed the code down to just over 300 lines.  For the most part the code is for testing and proof of concept as it will all be pulled into Home Assistant with ESPhome after I get the PCB's in to validate.  Once I do that, I will also share my Yaml here.

What it does currently.  Using the MQ135 and DHT-22 Modules the ESP creates an internal webpage that tracks Temperature, Humidity, and Air Quality. It then charts it on a 60 min and 24 hour graph.  It also saves the charts, so they survive a restart.  However, it does not track time so if it is down for 20 min that gap does not show up it just continues where it left off.  

I had a heck of a time trying to get the hostname to work on a computer browser and it still doesn’t work consistently, but it always works from my phone or using the IP directly.  It reports the IP and internal URL in the serial monitor. 

The PCB design is currently untested, but I have placed an order for them and will update this if it all works.

## 📸 Features
- ✅ ESP32‑S3 web server (no external services required)
- ✅ Live sensor readings
- ✅ Air quality classification (Great → Warning)
- ✅ Interactive graphs:
  - Last 60 minutes
  - Last 24 hours
- ✅ Data persistence (survives reboot/power outage)
- ✅ Optional hostname (`.local`)
- ✅ Fully self-contained solution


## 🧰 Hardware Required

- ESP32‑S3‑Zero (Waveshare or similar) - $8.29 CAD - www.aliexpress.com/item/1005009800317016.html
- MQ‑135 Air Quality Sensor Module (4‑pin) - $1.39 CAD - www.aliexpress.com/item/1005007710261547.html
- DHT22 / AM2302 Temperature & Humidity Sensor (3‑pin module) - $2.89 CAD - www.aliexpress.com/item/1005007188394422.html
- 10k resistor - Large pack $5.49 CAD - www.aliexpress.com/item/1005011772534173.html
- 20k resistor - Included in above pack
- (Optional) 0.1µF/100nF ceramic capacitor - $4.41 CAD https://www.aliexpress.com/item/1005003979789534.html
- Breadboard or PCB


## 🔌 Wiring

### DHT22

| Pin  | ESP32  |
|------|--------|
| VCC  | 3.3V   |
| DATA | GPIO 5 |
| GND  | GND    |


### MQ‑135 (IMPORTANT)

| Pin | Connection               |
|-----|--------------------------|
| VCC | 5V                       |
| GND | GND                      |
| A0  | Voltage Divider → GPIO 6 |
| DO  | Not used                 |


### Voltage Divider (REQUIRED)

```
MQ135 A0 ── 20kΩ ──+── GPIO 6 (ESP32 ADC)
                   |
                 10kΩ
                   |
                  GND
```


### Optional: Noise Filtering Capacitor

(GPIO 6 junction) ──|0.1µF|── GND



## 🔌 Wiring Diagram
<img width="512" height="691" alt="image" src="https://github.com/user-attachments/assets/9793f6ce-34d4-44a0-8a93-345cac12e789" />


## 💻 Software Setup

I use the Arduino IDE so the instructions are for that

### 1. Install ESP32 Board Support

- Open Arduino IDE
- Go to: Tools → Board → Boards Manager
- Install: ESP32 by Espressif Systems


### 2. Install Required Libraries

In Arduino IDE goto: Sketch → Include Library → Manage Libraries

Install:
- **DHT sensor library** by Adafruit
- **Adafruit Unified Sensor**


### 3. Built‑in Libraries (included automatically)

No need to install:

- WiFi
- WebServer
- Preferences
- ESPmDNS

These come with the ESP32 board package.

---

## ⚙️ Configuration

At the top of the sketch, set the following:
```
const char* ssid = "YOUR_WIFI_NAME";            // Wifi Name
const char* password = "YOUR_WIFI_PASSWORD";    // Wifi Password

const char* hostName = "air-monitor";           // optional network name
const char* pageTitle = "My Air Monitor";       // dashboard title
```

## 🌐 Usage

1. Upload the sketch
2. Open Serial Monitor (115200 baud)
 - Note the IP address
 - Open in browser: http://#.#.#.#
3. Optional (if supported by your network):
 - http://air-monitor.local replace "air-monitor" with what you set as the network name


## 📟 PCB Design 

The PCB was designed in EasyEDA and should be correct as is but because I have not revived my order from JLCPCB I can not confirm that it is working.  I will report back after testing
<img width="601" height="772" alt="image" src="https://github.com/user-attachments/assets/fd5a052f-93bc-454d-9974-ee183d0997d5" />
<img width="597" height="763" alt="image" src="https://github.com/user-attachments/assets/61829956-e4e9-4b4a-b369-dfafed91b19f" />
<img width="606" height="772" alt="image" src="https://github.com/user-attachments/assets/c0077005-39e2-4c1b-9b50-5e90d34dd646" />

Let me know if you'd like to see something differant and i'll see what I can do

https://www.youtube.com/@MakeOrBreakSociety
https://x.com/MoBS1337
https://www.facebook.com/MakeOrBreakSociety
https://linktr.ee/makeorbreaksociety

