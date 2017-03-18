// ESP8266 module


#ifndef _ESP8266_H_
#define _ESP8266_H_

class ESP8266 {

  public:
    ESP8266();
    void begin();
    void parse();
    bool getOk();
    bool getError();
    void doCmd(char *);
    char *getAccess();
    char *getSsid();
    void setSsid(char *);
    char *getPassword();
    void setPassword(char *);
    int getStatus();
    int getMode();
    int getIpMode();
    void getMac(char *);
    char *getAccessIP();
    char *getStationIP();
    bool getTransparent();
    bool doTransparent(char *);
    void doConnect(char *, char *);
    int isConnected();

  private:
    int doParm(int);
    char Buffer[256];
    int Head;
    int Tail;
    bool Ok;
    int Error;
    int Mode;
    char Access[32];
    char Ssid[15];
    char Password[15];
    int Channel;
    int Ecn;
    int Connections;
    char StationI[15];
    char StationM[15];
    int Dhcp;
    char Mac[20];
    int Status;
    int Mux;
    int IpMode;
    int IpTimeOut;
    char AIp[16];
    char SIp[16];
    bool Transparent;
    int Connected;
};

#endif

