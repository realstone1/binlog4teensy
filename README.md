
# BinLog4teensy
Sample code of the fast binary data logging for __Teensy 4.1__.

Binary file saved in micro SD card is read by MATLAB code in "./load binary/"


## Format

Data is written as short int (2 Bytes) in "DATA.LOG".

![image](https://user-images.githubusercontent.com/114337358/206899316-ff17f5a5-9f0d-494a-b434-a7b967445486.png)



## Usage

* __Compile code__

Compile source code by Arduino IDE and write to Teensy 4.1 board.

"DATA.LOG" is generated in micro SD card on Teensy 4.1 board.

* __Read binary file__

Read the generated file by "./load binary/plot_data.m"
