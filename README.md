# ESP32 Air Quality Monitor (MQ‑135 + DHT22)
Please be kind to me as this is my first start to finish project 🤣

This is a simple air quality monitor that I built with simple and cheap parts

This was all built with the Waveshare ESP32-S3-Zero as a backbone.  But any similar ESP32 with the same footprint, power\ground pin location, and analog on pin 6 will work!

The arduino code was written completely by me at first but was well over 700 lines of code.  With the assistance of an LLM we compressed the code down to just over 300 lines.  For the most part the arduino code is for testing and proof of concept the YAML is the code intended to be used.  Instructions for the YAML are here https://github.com/MakeOrBreakSociety/ESP32-air-quality-monitor/tree/main#esphome-yaml-instructions.

3D Printable case for the PCB can be found here: https://www.printables.com/model/1749242-air-quality-monitor-case

### What it does it do currently? 
### Arduino: 
Using the MQ135 and DHT-22 Modules the ESP creates an internal webpage that tracks Temperature, Humidity, and Air Quality. It then charts it on a 60 min and 24 hour graph.  It also saves the charts, so they survive a restart.  However, it does not track time so if it is down for 20 min that gap does not show up it just continues where it left off. 
### ESPHome: 
Fully intrigrates with Home Assistant and tracks Temperature, Humidity, and Air Quality giving you the ablility to run automations against the data it provides

The PCB design is funtcional but there is a change that needs to be made in the YAML's math depending on if you are using PCB or breadboard. Please see the ESPHome/Home Assistant section for instructions

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

I use the Arduino IDE so the instructions are for that.

<img width="1148" height="617" alt="image" src="https://github.com/user-attachments/assets/727b0872-7ca6-4591-9c24-f6e8bddf395b" />

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

## 📊 Air Quality Levels

| Raw Value | Level    |
|-----------|----------|
| < 1200    | GREAT    |
| 1200–1500 | GOOD     |
| 1500–1800 | MODERATE |
| 1800–2200 | POOR     |
| > 2200    | WARNING  |


## 💾 Data Persistence

Sensor history is saved to flash memory
Data survives:
- Power loss
- Reboots
- Reflashing firmware

## ⚠️ Notes & Limitations

1. ESP32 ADC is not precision‑grade so values are relative
2. MQ‑135 requires warm‑up (~24h for best stability)
3. .local hostname may not work reliably on Windows Use IP address if needed

## 🔮 Future Improvements

- ~~ESPHome + Home Assistant integration~~ <ins>**Done**</ins>
- ~~Alerts / notifications~~ <ins>**Included in ESPHome**</ins>
- ~~OTA updates~~ <ins>**Included in ESPHome**</ins>
~~- Enclosure design~~ https://www.printables.com/model/1749242-air-quality-monitor-case

## ESPHome YAML instructions

I have made an ESPH YAML to integrate this in to Home Assistant.  Here's the best way to get it all imported:

1. In home assistant goto settings > Apps > Install app > the search for "ESPHome Device Builder" and click install.  Don't install the ones that say experimental.  Once it's installed click all 4 sliders to on.  This is just my preference but read them and decide for yourself <img width="1280" height="713" alt="image" src="https://github.com/user-attachments/assets/116cb37e-f5a0-46e3-8965-ab058c7d40af" />

2. Once installed open the web UI and click "+ New Device" in the bottom right then click continue and click new device setup. ****You may be asked to set up your wifi ssid and password if you are do that as well****. Name the device "Air Quality" I named mine "Air Quality 02" because I already have 1 so I want to be able to tell them apart. Now this part is important!  Un check the box at the bottom that says "Use recommended settings" then click ESP32-S3. go all the way to the bottom and click on "Waveshare ESP32-S3-Zero" and click next.  Finally click skip.<img width="1388" height="182" alt="image" src="https://github.com/user-attachments/assets/da86f320-3ea7-487b-a419-ba5a18872340" />

3. Now that you have the device click edit and goto the bottom of the file where it says "captive_portal:" click the line below that and hit enter to give us an extra row of space.  Now in GITHUB click the file at the top that says AiQuality-v01.yaml Copy everything under "captive_portal:" and paste it into the open space at the bottom of your ESPH YAML.  the "captive_portal:" part should now look something like this:<img width="538" height="585" alt="image" src="https://github.com/user-attachments/assets/3c1e6db7-711f-4015-81ec-f88f6574ac63" />

4. Now look in the code you pasted for this line "Filters: # Uncomment ONE of the following..." If you are setting all this up on a breadboard remove the # infrount of "- multiply: 1000" If you are using the PCB I designed remove the # infrount of "- multiply: 16300" <img width="606" height="360" alt="image" src="https://github.com/user-attachments/assets/a58acb4c-d048-4493-9da8-e47f7e7908ca" />

5. Now Click save at the top right and then the X at the top left.  Now click the 3 dots and click Validate if all is good you can now click install!

6. To install this, I have had the best luck with Manual download.  This will take a while, for me it was about 12 min when it's done click Factory format and the file will download and you can click close.<img width="1392" height="180" alt="image" src="https://github.com/user-attachments/assets/f7cbb648-f506-469e-af94-a773ccce2115" />

7. Now that the file is downloaded plug in your ESP and goto https://web.esphome.io/ click connect and select your ESP and port. in my case it was "USB JTAG/serial debug unit (COM10)" then click instal then choose file and browse to the file downloaded from ESPH then click install and wait for it to finish flashing

8. Once it's done click logs and make sure it shows that it connected to your network if like me you haven’t connected all your sensors yet then you will get communication failed warnings, that's ok.  If you already have everything connected you won't see anything like that.  once you see boot seems successful your done here<img width="720" height="472" alt="image" src="https://github.com/user-attachments/assets/90df8f23-fc21-431d-b496-5b35170c4fb6" />

9. Next go back into ESPHome and if your Home Assistant and your ESP are on the same network you should see it say ONLINE in the top right.  If it does click LOGS then wirelessly and confirm that you are getting live data.<img width="706" height="456" alt="image" src="https://github.com/user-attachments/assets/2a9f2f7b-aa7c-47c6-af36-5ba3316dc348" />
 while you're in here note down the IP address it connected to.

10. Now you're on the home stretch.  In Home Assistant click settings then devices and services at the top click devices then look for Air Quality or whatever you named it in step 2 if like me it doesn’t show there click add device at the bottom right type in ESPHome and enter the IP address where it says host and you can leave the port alone.  once it finds the device tell it where it's located if you have different rooms set up.

11. Now in the middle where it shows Sensors click Add to dashboard and add it to where you want to see it. Now you're done! Congratulations<img width="1028" height="498" alt="image" src="https://github.com/user-attachments/assets/e98cbcb2-6e1e-4f83-8106-e53af3f1ffe2" />

12. There are a ton of stats and charting and automations you can do now that you have it added in to Home Assistant but that is beyond this tutorial.

## 📟 PCB Design 

The PCB was designed in EasyEDA and passed all testing.  I have 4 currently running in my home!

If you would like to order this board From JLCPCB you can do it here:
https://jlcpcb.com/?from=BGJR
<img width="601" height="772" alt="image" src="https://github.com/user-attachments/assets/fd5a052f-93bc-454d-9974-ee183d0997d5" />
<img width="597" height="763" alt="image" src="https://github.com/user-attachments/assets/61829956-e4e9-4b4a-b369-dfafed91b19f" />
<img width="606" height="772" alt="image" src="https://github.com/user-attachments/assets/c0077005-39e2-4c1b-9b50-5e90d34dd646" />


Let me know if you'd like to see something changed and i'll see what I can do.
- https://www.youtube.com/@MakeOrBreakSociety
- https://x.com/MoBS1337
- https://www.facebook.com/MakeOrBreakSociety
- https://linktr.ee/makeorbreaksociety

