# Tracking

This respository contains ROS packages for autonomous trakcing of an object using Kernalized Centralized Filter(KCF) tracking algorithm.

### Dependencies

* ``` sudo apt-get install ros-melodic-husky-gazebo ros-melodic-teleop-twist-keyboard``` 
* ``` sudo apt-get install ros-melodic-cv-bridge ros-melodic-image-transposrt ros-melodic-eigen-conversions ``` 

### Other Dependencies

OpenCV C++ version atleast 3.x.x required

Procedure to install openCV 3.4.2 from source

```
git clone https://github.com/opencv/opencv.git
cd opencv && git checkout 3.4.2
cd ..
git clone https://github.com/opencv/opencv_contrib.git
cd opencv_contrib && git checkout 3.4.2
cd ..
cd opencv 
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D INSTALL_C_EXAMPLES=ON \
      -D INSTALL_PYTHON_EXAMPLES=ON \
      -D WITH_TBB=ON \
      -D WITH_V4L=ON \
      -D WITH_QT=ON \
      -D WITH_OPENGL=ON \
      -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
      -D BUILD_EXAMPLES=ON ..
make -j4
sudo make install
sudo sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig
```
### Installation Instructions: 

```
mkdir -p ~/tracking_ws/src
cd ~/tracking_ws/src
catkin init
git clone <repo_link>
wstool init
wstool merge tracking/install/install_ssh.rosinstall
wstool update
catkin build
echo "source ~/traking_ws/devel/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

### How to run the simulation
```
roslaunch simulator simulation.launch
```
## Different packages
### pose_estimate:
This package is used to find the pose of the tracked object in world frame.
### tracking:
This package is used to track the object using KCF. A ROI is selected manually by the user and the node tracks the object (husky) inside the ROI.
### simulation: 
This package contains all the urdf, world and the launch files used to run the simulation environment.

### Video Demo:

The video demo of this project can be found on this [link](https://drive.google.com/file/d/1Zbe3OWqVA89yEYJfSTbyKQV7BlBUXbkC/view)

### Contributors
* [Ujwal Jyot Panda](https://github.com/ujwaljp)
* [Rahul Rustagi](https://github.com/rrustagi20)
* [Saransh Shivhare](https://github.com/saranshg20)
* [Akshit Verma](https://github.com/vakshit)
* [B. Anshuman](https://github.com/ba-13)
