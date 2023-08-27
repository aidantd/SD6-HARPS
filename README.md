# Senior Design -- HARPS Project

| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- |

## SD6-HARPS folder contents

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is an explanation of the files in the project folder.

```
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   └── main.c
│   └── peripherals/
│       └── bme280.c
│       └── bme280.h
│   └── tasks/
│       └── basicTask.c
│       └── pt-task.c
│   └── userHAL/
│       └── i2c.c
│       └── i2c.h
│       └── uart.c
│       └── uart.h
└── README.md                  This is the file you are currently reading
```
