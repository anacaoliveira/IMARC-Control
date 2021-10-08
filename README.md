# Iowa Marine Autonomous Racing Club - Control/Software Group

List of resources used by the control/software group of the University of Iowa's Marine Autonomous Racing Club ⚓

"IMARC is a technical organization focused on designing and building autonomous boats for participation in national and international competitions, such as the Association of Unmanned Vehicle Systems International (AUVSI)'s RoboBoat competition."
To find more information and who to contact: [https://www.engineering.uiowa.edu/current-students/student-organizations/iowa-marine-autonomous-racing-club-imarc](https://www.engineering.uiowa.edu/current-students/student-organizations/iowa-marine-autonomous-racing-club-imarc)

## 2021 Rules & Task Description:
[https://robonation.org/app/uploads/sites/3/2021/04/RoboBoat-2021-Rules-and-Task-Description_V2.pdf](https://robonation.org/app/uploads/sites/3/2021/04/RoboBoat-2021-Rules-and-Task-Description_V2.pdf)

To find the up to date RobotBoat guideline and other resources:
[https://roboboat.org/resources/](https://roboboat.org/resources/)

## Resources
Considering that not everyone have access to a Linux computer we can use the ROS Toolbox within MATLAB. **The university offers a MATLAB license to all student** (in the same link you will find instructions on how to install it !): [https://its.uiowa.edu/matlab](https://its.uiowa.edu/matlab)


## ROS 
- ROS Tutorials
http://wiki.ros.org/ROS/Tutorials

- Free course without certificate
[https://www.coursera.org/projects/ros2-intro](https://www.coursera.org/projects/ros2-intro)


## VRX

Follow the tutorials to install the VRX simulation (https://github.com/osrf/vrx/wiki)

1. After instalation, build and run the VRX Platform

Source the ROS setup.bash file: 
```
source /opt/ros/noetic/setup.bash
```

Use catkin_make to build the software:
```
cd ~/vrx_ws
catkin_make
```

2. Run 
Source the created VRX setup.bash: 
```
source  ~/vrx_ws/devel/setup.bash
```
Launch the VRX simulation:
```
roslaunch vrx_gazebo sandisland.launch
```
This steps are necessary everytime you open a new terminal. 


--WSL Installation
https://jack-kawell.com/2019/06/24/ros-wsl1/
