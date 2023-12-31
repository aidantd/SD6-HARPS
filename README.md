# Senior Design -- HARPS Project

## Overview
The Hurricane Automated Refuge Protection System will be utilized to protect indivudals during severe weather events. This repository contains the code for the SD6-HARPS project.

| Supported Targets |   ESP32   | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | --------- | -------- | -------- | -------- | -------- | -------- | -------- |
|                   |  &#10003; | &#10003; | &#10003; | &#10003; | &#10003; | &#10003; | &#10003; |

## Repository Struture
```
main (branch) - contains the most stable version of the code

programmerInitials-featureName (branch) - contains the code for a specific feature that is being worked on
    Ex. atd-implementTemperatureSensor
```

## SD6-HARPS folder contents

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is an explanation of the files in the project folder.

```
├── CMakeLists.txt
├── main/                           Contains the code for the system's firmware
│   └── CMakeLists.txt
│   └── main.c
│   └── peripherals/                Contains the code for the system's peripherals
│       └── bme280.c
│       └── bme280.h
│       └── l289.c
│       └── l289.h
│       └── sen0170.c
│       └── sen0170.h
│   └── tasks/                      Contains the system's tasks
│       └── anemometer-task.c
│       └── decisionMaking-task.c
│       └── motor-task.c
│       └── pt-task.c
│       └── screen-task.c
│       └── weatherAPI-task.c
│   └── userHAL/                    Contains the code for the HAL
│       └── adc.c
│       └── adc.h
│       └── dac.c
│       └── dac.h
│       └── i2c.c
│       └── i2c.h
│       └── uart.c
│       └── uart.h
│   └── utility/                    Contains the code for the system's utility functions
│       └── api/                    Contains the code for interfacing with external APIs
│           └── geolocationAPI.h
│           └── weatherAPI.h
│       └── haglGraphics/           Contains the submodule for the haglGraphics library
│           └── hagl
│           └── hagl_hal
│       └── sensorBoard/            Contains the code for interfacing with external APIs
│           └── led.c
│           └── led.h
│       └── timers/                 Contains the code for the system's timers
│           └── timers.c
│           └── timers.h
└── README.md 
```

## Getting Started
1. Install ESP-IDF Through VSCode Extension (Recommended) or Eclipse Plugin
    - [ESP-IDF Installation Instructions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
    - Follow the instructions for your IDE
2. Clone this repository
    - `git clone [insert url here]`
3. Open the project in your IDE and begin working


## How to Contribute
1. Create a new branch off of the main branch
2. Implement your feature
3. Create a pull request to merge your branch into the main branch
4. Wait for the pull request to be reviewed before being merged
5. Once reviewed, the pull request will be merged into the main branch

