# ESP32 Air Quality Monitor (MQ‑135 + DHT22)
This is a simple air quality monitor that I built with simple and cheap parts

This was all built with the Waveshare ESP32-S3-Zero as a backbone.  but any similar ESP32 with the same footprint and analog on pin 6 will work!

The code was written by me compleetly at first but was well over 700 lines of code.  With the assistance of an LLM we compressed the code down to just over 300 lines.  For the most part the code is for testing and proof of concept as it will all be pulled into Home Assistant with ESPhome after I get the PCB's in to valadate.  Once I do that I will also share my Yaml here.

What it does currently.  Using the MQ135 and DHT-22 Moduals the ESP creates an internal webpage that tracks tempature, humidity, and Air Quality. It then charts it on a 60 min and 24 hour graph.  It also saves the charts so they survive a restart.  however it does not track time so if it is down for 20 min that gap does not show up it just continues where it left off.  

I had a heck of a time trying to get the hostname to work on a computer browser and it still dosn't work concistantly, but it allways works from my phone or using the IP directly.  It reports the ip and internal URL in the serial monitor. 

The PCB design is currently untested but I have placed an order for them and will update this if it all works.

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
