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

### How to send data

#### 1. AT+CIPSTART

    Estabilish UDP connection

```
    // Single connection (AT+CIPMUX=0):
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]
```

```
    AT+CIPSTART="UDP","192.168.101.110",1000,1002,2
    AT+CIPSTART="UDP","192.168.101.110",1000,1002,2

    CONNECT
    OK
```

#### 2. AT+CIPSEND

    `AT+CIPSEND=<length>`

```
    AT+CIPSEND=100

    OK
    >
    // Send 100 bytes here, no escaping needed
    SEND OK    
```