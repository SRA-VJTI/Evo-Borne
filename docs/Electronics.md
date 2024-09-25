---
title: Electronics

---

# Electronics


 - Electronics is a critical part of the EvoBorne robot. All the processing & control applications for the morphobot are performed with the electronics. The electronics for the morphobot has two major components : 
    1. Power Circuitry
    2. Controlling System

## Components used

### **SRA Development Board**
- The [SRA board](https://github.com/SRA-VJTI/sra-board-hardware-design) is our in-house development board based on the ESP32 microcontroller, featuring on-board peripherals such as programmable LEDs, switches, sensor ports for Line Sensor Array and MPU-6050, an over-current and reverse voltage protection circuit, and motor drivers. 
- ![SRA Board v2.5](![sra-board-24-image](../assets/SRA_Board.jpg)


- The SRA board is used to connect to & control the servos and the N20 motors through the ESP32. The board has dedicated ports to connect servos as well as normal DC motors. 



### **ESP32-WROOM-32E MCU**
- The ESP32 is primarily being used for controlling the servos and the N20 DC motors for morphing/unmorphing the robot from a wheeled vehicle into a drone. 


### SpeedyBee F405 V3 flight controller
- A flight controller is necessary for controlling the drone effectively. The SpeedyBee F405 V3 has a STM32F405 MCU with an ARM-Cortex M4 core 
- The flight controller’s compact size of 30x30mm makes it compatible with a wide range of frames, from smaller micro quadcopters to larger racing drones. It is based on the popular STM32F405 MCU (Microcontroller Unit) with an ARM Cortex-M4 core, ensuring smooth and efficient processing of data. 
 ![Screenshot from 2024-04-24 10-28-56](../assets/SpeedyBee%20F405%20Flight_Controller.png)


### Power Circuitry
- The morphobot uses a 24V, 1400 mAh battery to power all of its electronics. Since each component has unique power requirements, an additional circuit is used to manage the input voltage to each component.
- The voltage drop is controlled as required using Buck Converters
- 

### Realsense D435i depth camera
- The Realsense depth camera, specifically the Intel Realsense D435i model, is a stereo vision camera system designed to capture high-resolution depth images. This capability is critical for applications requiring 3D spatial recognition and depth estimation.
- For the morphobot, the Realsense camera provides essential depth information that helps in environment mapping. It allows the morphobot to perceive its surroundings in three dimensions, which is crucial for the drone mode, where understanding the altitude and clearance around obstacles directly influences flight safety and stability.
 ![realsense d435i img](../assets/Realsense%20Camera.jpg)

- The camera features a wide field of view and can generate depth data at a high frame rate, which is beneficial for the dynamic environments in which the morphobot operates. The integration of this camera enhances the morphobot's ability to interact intelligently with its environment, contributing significantly to its autonomous capabilities.

### Jetson Nano Developer Kit
- The Jetson Nano Developer Kit from NVIDIA is a small, powerful computer designed specifically for learning and development of robotics projects. It is based on the NVIDIA Jetson Nano SoC, which combines a quad-core ARM Cortex-A57 MPCore processor and a 128-core Maxwell GPU. This combination makes it highly effective for tasks,such as image processing & deep learning.
    
![jetson_nano](../assets/Jetson%20Nano%20.jpg)

- In the context of the morphobot, the Jetson Nano Developer Kit is utilized for high-level processing tasks such as image and video analysis, obstacle detection, and navigation intelligence. Its powerful processing capabilities enable the morphobot to perform complex computations required for autonomous decision-making and environment interaction, particularly in scenarios where rapid response to sensor inputs is crucial.

### 4-in-1 ESC
- Electronic Speed Controllers (ESC's) are integral to controlling the power supplied to the motors of drones. A 4-in-1 ESC consolidates four individual ESCs into one compact module, which simplifies the wiring and reduces the overall weight and complexity of the drone assembly in the morphobot.
- Each of the integrated ESCs can independently manage one of the drone's brushless BLDC motors, providing precise control over the speed and direction of each motor which is crucial for maintaining stability and maneuverability during the drone's flight.
![4-in1 esc kit](https://hackmd.io/_uploads/H1dgJ0PWA.jpg)

    The use of a 4-in-1 ESC not only optimizes the electrical efficiency but also improves the reliability and responsiveness of the drone system, which are vital characteristics during both flight and morphing transitions.

Overall, the morphobot's electronics system is a coordinated assembly of different components that work together to deliver impressive capabilities in both its drone and wheeled configurations. From power management and motor control to high-level  depth perception, each component plays an important role in ensuring the morphobot's functionality and performance across various operational scenarios.
