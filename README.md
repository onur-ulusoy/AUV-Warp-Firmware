# WARP Firmware 

WARP is the motor driver and battery monitoring board for the auv. 



## Pin Definitions
| Exported Pins |     Description     |
|---------------|---------------------|
| CONV1_SW_Pin  | Conv. 1 Switch Pin  |               
| ESC1_A_Pin    | ESC1 Current Pin    |             
| ESC2_A_Pin    | ESC2 Current Pin    |             
| ESC3_A_Pin    | ESC3 Current Pin    |             
| ESC4_A_Pin    | ESC4 Current Pin    |             
| ESC5_A_Pin    | ESC5 Current Pin    |             
| ESC6_A_Pin    | ESC6 Current Pin    |             
| ESC7_A_Pin    | ESC7 Current Pin    |             
| ESC8_A_Pin    | ESC8 Current Pin    |             
| BATT_V_Pin    | BATT Voltage Pin    |             
| BATT_A_Pin    | BATT Current Pin    |             
| CONV2_SW_Pin  | Conv. 2 Switch Pin  |               
| CONV1_V_Pin   | Conv. 1 Voltage Pin |              
| CONV1_A_Pin   | Conv. 1 Current Pin |              
| CONV2_V_Pin   | Conv. 2 Voltage Pin |              
| CONV2_A_Pin   | Conv. 2 Current Pin |              
| ESC1_PWM_Pin  | ESC1 Pulse Pin      |               
| ESC2_PWM_Pin  | ESC2 Pulse Pin      |               
| ESC3_PWM_Pin  | ESC3 Pulse Pin      |               
| ESC4_PWM_Pin  | ESC4 Pulse Pin      |               
| ESC5_PWM_Pin  | ESC5 Pulse Pin      |               
| ESC6_PWM_Pin  | ESC6 Pulse Pin      |               
| ESC7_PWM_Pin  | ESC7 Pulse Pin      |               
| ESC8_PWM_Pin  | ESC8 Pulse Pin      |


## Dependencies
```shell
sudo apt-get install gcc-arm-none-eabi
```

## Build 
```shell
make all
```

## LICENSE
MIT

See [LICENSE](LICENSE) for details.



