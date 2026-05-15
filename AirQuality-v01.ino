#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <DHT.h>
#include <Preferences.h>

// ================= USER SETTINGS =================
const char* ssid = "YOUR_WIFI_NAME";            // Wifi Name
const char* password = "YOUR_WIFI_PASSWORD";    // Wifi Password

const char* hostName = "air-monitor";       // optional network name
const char* pageTitle = "My Air Monitor";   // dashboard title

// ================= HARDWARE =====================
#define DHTPIN 5
#define DHTTYPE DHT22
#define MQ135_PIN 6

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);
Preferences prefs;

// ================= DATA =========================
float temp60[60], hum60[60];
int air60[60];

float temp24[24], hum24[24];
int air24[24];

int idx60 = 0;
int idx24 = 0;

// ================= TIMING =======================
unsigned long lastSample = 0;
unsigned long lastMinute = 0;
unsigned long lastHour   = 0;
unsigned long lastSave   = 0;
unsigned long lastMDNS   = 0;

float tAcc = 0, hAcc = 0;
int aAcc = 0, samples = 0;

// ================= AIR QUALITY ==================
String getAirQualityLabel(int air) {
  if (air < 1200) return "GREAT";
  if (air < 1500) return "GOOD";
  if (air < 1800) return "MODERATE";
  if (air < 2200) return "POOR";
  return "WARNING";
}

// ================= SAVE / LOAD ==================

void saveData() {
  // 60-minute history
  prefs.putBytes("t60", temp60, sizeof(temp60));
  prefs.putBytes("h60", hum60, sizeof(hum60));
  prefs.putBytes("a60", air60, sizeof(air60));

  // 24-hour history
  prefs.putBytes("t24", temp24, sizeof(temp24));
  prefs.putBytes("h24", hum24, sizeof(hum24));
  prefs.putBytes("a24", air24, sizeof(air24));

  // indices
  prefs.putUInt("i60", idx60);
  prefs.putUInt("i24", idx24);
}

void loadData() {
  // 60-minute history
  prefs.getBytes("t60", temp60, sizeof(temp60));
  prefs.getBytes("h60", hum60, sizeof(hum60));
  prefs.getBytes("a60", air60, sizeof(air60));

  // 24-hour history
  prefs.getBytes("t24", temp24, sizeof(temp24));
  prefs.getBytes("h24", hum24, sizeof(hum24));
  prefs.getBytes("a24", air24, sizeof(air24));

  // indices
  idx60 = prefs.getUInt("i60", 0);
  idx24 = prefs.getUInt("i24", 0);
}

// ================= HTML =========================
String getHTML(float t, float h, int air) {
  String quality = getAirQualityLabel(air);
  String color = (quality=="GREAT")?"green":
                 (quality=="GOOD")?"darkgreen":
                 (quality=="MODERATE")?"orange":
                 (quality=="POOR")?"red":"darkred";

  String html = "<!DOCTYPE html><html><head>";
  html += "<title>"+String(pageTitle)+"</title>";
  html += "<meta http-equiv='refresh' content='30'>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;}";
  html += ".grid{display:grid;grid-template-columns:repeat(3,1fr);gap:10px;max-width:900px;margin:auto;}";
  html += "canvas{border:1px solid #888;}";
  html += "</style></head><body>";

  html += "<h1>"+String(pageTitle)+"</h1>";
  html += "<h2 style='color:"+color+";'>Air Quality: "+quality+"</h2>";
  html += "<p>Air Quality(raw): "+String(air)+"</p>";
  html += "<p>Temp: "+String(t,1)+" C | Humidity: "+String(h,1)+" %</p>";

  html += "<div class='grid'>";
  html += "<div>Air Quality (60m)<br><canvas id='air60' width='280' height='120'></canvas></div>";
  html += "<div>Temp (60m)<br><canvas id='temp60' width='280' height='120'></canvas></div>";
  html += "<div>Humidity (60m)<br><canvas id='hum60' width='280' height='120'></canvas></div>";
  html += "<div>Air Quality (24h)<br><canvas id='air24' width='280' height='120'></canvas></div>";
  html += "<div>Temp (24h)<br><canvas id='temp24' width='280' height='120'></canvas></div>";
  html += "<div>Humidity (24h)<br><canvas id='hum24' width='280' height='120'></canvas></div>";
  html += "</div>";

  html += R"rawliteral(
<script>
async function drawAll(){
  let r=await fetch('/history');
  let d=await r.json();

  plot("air60",d.air60,800,3000);
  plot("air24",d.air24,800,3000);

  plot("temp60",d.temp60,0,40);
  plot("temp24",d.temp24,0,40);

  plot("hum60",d.hum60,0,100);
  plot("hum24",d.hum24,0,100);
}

function plot(id,data,min,max){
  const c=document.getElementById(id);
  if (!data || data.length === 0) return;
  const ctx=c.getContext('2d');

  const w=c.width,h=c.height;
  const L=35,R=5,T=5,B=20;

  ctx.clearRect(0,0,w,h);
  ctx.font="10px Arial";

  // Y axis
  for(let i=0;i<=4;i++){
    let y=T+(h-T-B)*i/4;
    let v=Math.round(max-(max-min)*i/4);
    ctx.beginPath();
    ctx.moveTo(L,y); ctx.lineTo(w-R,y); ctx.strokeStyle="#ccc"; ctx.stroke();
    ctx.fillText(v,2,y+3);
  }

  // X axis
  ctx.beginPath();
  ctx.moveTo(L,h-B); ctx.lineTo(w-R,h-B);
  ctx.strokeStyle="#000"; ctx.stroke();

  let step=(data.length==60)?5:3;
  for(let i=0;i<data.length;i+=step){
    let x=L+i*(w-L-R)/(data.length-1);
    ctx.beginPath();
    ctx.moveTo(x,h-B); ctx.lineTo(x,h-B+5);
    ctx.stroke();
    ctx.fillText(i,x-5,h-5);
  }

  // line
  ctx.beginPath();
  ctx.strokeStyle="#000";
  data.forEach((v,i)=>{
    let x=L+i*(w-L-R)/(data.length-1);
    let y=T+(max-v)*(h-T-B)/(max-min);
    i?ctx.lineTo(x,y):ctx.moveTo(x,y);
  });
  ctx.stroke();

  // hover
  c.onmousemove = (e) => {
    const rect = c.getBoundingClientRect();

    const left = 35;
    const right = 5;

    const usableWidth = c.width - left - right;

    let x = e.clientX - rect.left - left;

    if (x < 0) x = 0;
    if (x > usableWidth) x = usableWidth;

    const index = Math.round(x / usableWidth * (data.length - 1));

    const value = data[index];

    if (value !== undefined) {
      c.title = "Value: " + value.toFixed(1);
    }
  };
}

drawAll();
setInterval(drawAll,10000);
</script>
)rawliteral";

  html += "</body></html>";
  return html;
}

// ================= ROUTES =======================
void handleRoot() {
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  int a=analogRead(MQ135_PIN);

  if(isnan(t)||isnan(h)){
    server.send(500,"text/plain","Sensor Error");
    return;
  }

  server.send(200,"text/html",getHTML(t,h,a));
}

void handleHistory() {
  String j = "{";

  j += "\"air60\":[";
  for (int i=0;i<60;i++) { j += String(air60[i]); if(i<59) j+=","; }
  j += "],\"temp60\":[";

  for (int i=0;i<60;i++) { j += String(temp60[i],1); if(i<59) j+=","; }
  j += "],\"hum60\":[";

  for (int i=0;i<60;i++) { j += String(hum60[i],1); if(i<59) j+=","; }
  j += "],\"air24\":[";

  for (int i=0;i<24;i++) { j += String(air24[i]); if(i<23) j+=","; }
  j += "],\"temp24\":[";

  for (int i=0;i<24;i++) { j += String(temp24[i],1); if(i<23) j+=","; }
  j += "],\"hum24\":[";

  for (int i=0;i<24;i++) { j += String(hum24[i],1); if(i<23) j+=","; }
  j += "]}";

  server.send(200, "application/json", j);
}

// ================= SETUP ========================
void setup() {
  Serial.begin(115200);
  dht.begin();
  analogReadResolution(12);

  prefs.begin("hist", false);
  loadData();


WiFi.mode(WIFI_STA);
WiFi.setSleep(false);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}

Serial.println("\nWiFi connected");
Serial.print("IP: ");
Serial.println(WiFi.localIP());

delay(1000);  // let network settle

if (!MDNS.begin(hostName)) {
  Serial.println("Error starting mDNS");
} else {
  Serial.println("mDNS started");
  Serial.print("Access: http://");
  Serial.print(hostName);
  Serial.println(".local");

  MDNS.addService("http", "tcp", 80);
}

  Serial.println(WiFi.localIP());

  server.on("/",handleRoot);
  server.on("/history",handleHistory);
  server.begin();
}

// ================= LOOP =========================
void loop() {
  server.handleClient();

  unsigned long now = millis();

  if(now-lastSample>10000){
    lastSample=now;
    float t=dht.readTemperature();
    float h=dht.readHumidity();
    int a=analogRead(MQ135_PIN);

    if(!isnan(t)&&!isnan(h)){
      tAcc+=t; hAcc+=h; aAcc+=a;
      samples++;
    }
  }

  if(now-lastMinute>60000 && samples){
    lastMinute=now;
    temp60[idx60]=tAcc/samples;
    hum60[idx60]=hAcc/samples;
    air60[idx60]=aAcc/samples;
    idx60=(idx60+1)%60;
    tAcc=hAcc=aAcc=samples=0;
  }

  if(now-lastHour>3600000){
    lastHour=now;
    float ts=0,hs=0; int as=0;
    for(int i=0;i<60;i++){ts+=temp60[i];hs+=hum60[i];as+=air60[i];}
    temp24[idx24]=ts/60;
    hum24[idx24]=hs/60;
    air24[idx24]=as/60;
    idx24=(idx24+1)%24;
  }

  if(now-lastSave>300000){ // every 5 min
    lastSave=now;
    saveData();
  }
}