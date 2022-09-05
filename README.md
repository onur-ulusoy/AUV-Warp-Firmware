# WARP Driver Board Firmware 

WARP is the dedicated motor driver and power distribution board that powers our Autonomous Underwater Vehicle (AUV). The firmware is specifically developed for the Cortex series STM32F401 microcontroller. With the integration of the real-time operating system, FreeRTOS, it coherently and responsively organizes complex tasks such as motor driving and power monitoring.

This repository focuses Warp's firmware. For more details on the hardware, visit the [Warp Hardware Repository](https://github.com/onur-ulusoy/AUV-Warp-Driver-Board).


<picture>   <img alt="dash" src="Images/logos/dash.png"> </picture>

<p align="center">
    <a href="https://auv.itu.edu.tr/">
        <img width="180" src="Images/logos/auv-electronics.png">
    </a>
</p>


<p align="center"><em >AUV Electronics 2022</em></p>


<picture>   <img alt="dash" src="Images/logos/dash.png"> </picture>


<p align="center">
    <a href="https://auv.itu.edu.tr/">
        <img width="600" src="Images/logos/auv.png">
    </a>
</p>


<picture>   <img alt="dash" src="Images/logos/dash.png"> </picture>

<p align="center">
    <img width="1500" src="Images/logos/logos.png">
</p>

<picture>   <img alt="dash" src="Images/logos/dash.png"> </picture>


<div style="display: flex; align-items: center; justify-content: center; margin-top: 30px; margin-bottom: 0px;">
    <img src="Images/logos/stm32f401cortex.png" alt="Image 2" style="width: 250px;"></a>
</div>
<br clear="both">

<p align="center"><em >STM32F401 Cortex</em></p>

<picture>   <img alt="dash" src="Images/logos/dash.png"> </picture>

<p style="margin-bottom: 1em;"></p>

[![License](https://img.shields.io/badge/License-Apache%202.0-yellow.svg?style=flat-square)](https://www.apache.org/licenses/LICENSE-2.0.html)
[![CPU](https://img.shields.io/badge/CPU-STM32F401-blue.svg?style=flat-square)](https://www.st.com/en/microcontrollers-microprocessors/stm32f401.html)
[![OS](https://img.shields.io/badge/OS-FreeRTOS-orange.svg?style=flat-square)](https://www.freertos.org)
[![Language](https://img.shields.io/badge/Language-C-red.svg?style=flat-square)](https://en.wikipedia.org/wiki/C_(programming_language))
[![make](https://github.com/onur-ulusoy/auv-warp-firmware/actions/workflows/make.yml/badge.svg)](https://github.com/onur-ulusoy/auv-warp-firmware/actions/workflows/make.yml)


## Table of Contents
- [WARP Driver Board Firmware](#warp-driver-board-firmware)
  - [Table of Contents](#table-of-contents)
  - [Board Description](#board-description)
  - [Architecture](#architecture)

## Board Description

The Warp Driver Board manages the direction movements of the underwater vehicle. It controls the brushless AC motors, connected to the propellers known as Thrusters, with PWM signals via integrated circuits called ESCs, based on commands received from the vehicle's onboard computer NVDIA Jetson Xavier. The commands are transmitted from the computer to the main board, then to the driver board, with the conversion of the UART signal to RS232 signal in the form of protobuf encrypted data.

The secondary, but equally crucial, function of the Warp Driver Board is to manage power distribution. It stabilizes the 14-16 Volt input from the battery and provides steady 12V and 5V outputs to other electronic components, the onboard computer, and various peripherals. The Warp Driver Board also includes a secondary 5V regulator to control the voltage supplied to its onboard microcontroller. 

The board is designed with safety and efficiency in mind. It includes a 100A fuse at the battery input and can measure current/voltage in various areas using integrated ADCs, thereby shutting down the vehicle in case of danger. To accurately measure voltages, it utilizes shunt resistors and voltage dividers. Critical data about current, voltage, and temperatures at key points are displayed on an LCD screen. 


<p align="center">
    <img width="1200" src="Images/Warp_PCB.png" alt="Warp PCB">
</p>

## Architecture

The firmware is built on a layered architecture, employing the FreeRTOS kernel on top of other layers like hardware abstraction layer (HAL) and more.


<p align="center">
    <img width="500" src="Images/architecture.png" alt="Architecture">
</p>


_Image and the below info are taken from [PiEmbsysTech](https://piembsystech.com/free-rtos/)_

The architecture of the firmware based on FreeRTOS consists of the following layers:

- **Hardware Abstraction Layer (HAL):** The HAL layer serves as an interface between the hardware and the operating system. This layer comprises device drivers, interrupt handlers, and other functions that are specific to the hardware.

- **Kernel:** The kernel layer, the core of the operating system, provides the scheduling and task management functions. This layer consists of the task scheduler, task management, memory management, synchronization mechanisms, and interrupt handling.

- **Application Programming Interface (API):** The API layer offers a collection of functions that can be used by the application for the creation, deletion, and management of tasks. Additionally, it also manages shared resources and synchronizes tasks.

- **Application Code:** This layer contains the user application code that operates atop the operating system. The development of this layer is possible using standard programming languages such as C and C++.


