# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/aidan/esp/esp-idf/components/bootloader/subproject"
  "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader"
  "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader-prefix"
  "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader-prefix/tmp"
  "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader-prefix/src"
  "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/aidan/Documents/School/2023_Fall/SeniorDesign2/SD6_HARPS/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
