### How to connect to WIFI

#### 1. AT+CWMODE

    First of all it's required to put module in Station mode. To do this we need to execute next command: 

    ```
        AT+CWMODE=1,0
        AT+CWMODE=1,0

        OK
    ```

#### 2. AT+CWJAP

    Connect to WiFi

    ```
        AT+CWJAP="SSID","PWD"
        AT+CWJAP="SSID","PWD"
        WIFI CONNECTED
        WIFI GOT IP

        OK
    ```

#### 3. AT+CIPSTA

    Retrive IP address

```
    AT+CIPSTA?
    
    AT+CIPSTA?
    +CIPSTA:ip:"192.168.0.16"
    +CIPSTA:gateway:"192.168.0.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK
```