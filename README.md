# SmartParno
## SmartParno is a project for the Embedded Microcomputer Systems school subject at ELSYS.

SmartParno allows you to control your central heating system (a.k.a. "parno") from any point of the world. The device mounts to your "parno", by simply unscrewing the old attachment and screwing in the new one. A bit of Wi-Fi configuration is needed in the application and you are good to go.

SmartParno is a two-part project: a physical device and a mobile application. The device uses a NodeMCU ESP8266 to connect to the local network and communicate with the application via Wi-Fi. For physically turingn ON and OFF the "parno",  we use a custom-made mechanism and a 6V DC motor. The motor is controlled by the ESP via an L293D shield.
