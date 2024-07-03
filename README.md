# Evo-Bourne
This repository focuses on building an open-source morphobot.
Autonomous human-friendly Multi-Terrain Scaling Vehicle

## Abstract
With the increase in pollution and our way of handling nature, the situation of disasters arises unexpectedly. The average cost of natural disasters over the past 25 years stands at over $87 billion a year alone in India. While the reason for loss of life is mostly the direct impact of the disaster, a significant number of lives are lost just because proper communication isn’t established in areas like tunnels and not-so-easily navigable areas. The incident in November of 2023 involving 40 workers being trapped in a tunnel located in the Himalayas was of grave concern. The gap in communication between humans and the bot also makes it hard to be employable in the case of an emergency. We present a way to establish communication by navigating through challenging environments that us human beings can’t scale. The ability of the robot to identify the point of interest itself (based on gesture and audio input from the user) and navigate to that point autonomously is one of the biggest challenges in robotics and we are working to address this problem. Using human interactions we present a system that can navigate through points of interest in any environment.

## Introduction
We are building a multi-terrain modular robot that can adapt to various surface and aerial environments and morph its structure accordingly, it can navigate as a wheeled robot as well as morph into a drone. We will be using a camera for visual SLAM to perform mapping in a dry run over a laboratory scenario, perform path planning, and control each of its links through reinforcement learning. We will use the 3D A* algorithms to perform path planning. Localization will be done through vision-based methods. The bot will be user-friendly as it will ask the user for voice input and gestures to find the direction and position required while navigating toward it autonomously.


## Motivation
Most existing solutions have a fixed design, limiting their adaptability to different terrains. Whereas, the Morphobot allows it to morph its physical structure based on specific terrain needs.
Developing algorithms such as the 3D A-star algorithm allows the robot to learn from its environment and adjust its behavior accordingly for adaptive locomotion.

## Use Case
It has multiple applications, one of its most useful applications is its usefulness in search and rescue operations, providing a robust way of identifying and getting information through areas that are hard to navigate or have uneven terrain. It can adapt to failures due to the redundancy in its joints (i.e. it has an extra degree of freedom, one more than necessary), this provides it with strength and helps it in locomotion even in cases of joint malfunctions.
How does your solution work? What are the main features? Please specify how you will use the AMD AI hardware in your solution.  
Our solution will have 2 DOF for each arm and we will have four such arms, thus giving it the capability to act as a limb. The bot comes with a thruster attached to the end of the limbs which provides it with the maneuvering capabilities of a drone. 
The main feature of the solution is that our bot can change its use case as per the conditions. It can be driven on the ground for ground-based tasks and if the path planner requires its movement in the vertical axis, it morphs into a drone and hovers to a given height.		
The AMD Kria KR260 will act as an AI accelerator and be used for path planning for our solution. Our bot will make proper use of the robust capabilities of SoC on the KRIA KR 260. Our solution will be equipped with a camera that will be used to sense the environment, thus with the help of this vision data the bot will ask the user where it wants to go, wait for the user's description of the given target, and direction, confirm the direction and head towards it automatically.

## Bill of Materials
- KR260 Vision AI Kit
- 4 in 1 ESCs for propeller control
- Propellers with suitable pitch
- RealSense camera
- Laser-cut chassis
- 12 MG995 servos
- 4 BLDC 1700KV 2807 motors
- A 1200 mAh 24 Volt Battery
- 4 n20s for wheel control
- Ubuntu 22.04
- PyNQ
- Microphone module

<hr/>

## Who are we?
We are a student club, the Society of Robotics and Automation at VJTI Mumbai. 
We spend most of the time preparing for workshops, study sessions, and cool projects in the domains of robotics and embedded systems. We love to build our robotic system from the ground up, design our very own PCBs, tinker and build our circuitries on FPGAs and overall have fun pursuing things that rev our hearts.
We have worked on various projects ranging from bots with creep gait, to full-fledged navigator bots with SLAM capabilities. We also have a digital electronics team familiar with HDL which will allow us to push the Kria to its full capabilities. Our prior experience with technologies such as ROS, Mechanical design, Computer Vision techniques, HDLs, and Zynq SoC will enable us to execute this project successfully. We have a motivated team of five members who have previously worked in their respective fields and can now build this project from scratch.
