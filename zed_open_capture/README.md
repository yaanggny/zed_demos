## Open Capture Camera API
From [zed-open-capture, commit=5cf66ff](https://github.com/stereolabs/zed-open-capture).

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#build-and-install">Build and install</a> •
  <a href="#run">Run</a> • 
  <a href="#documentation">Documentation</a> •
  <a href="#running-the-examples">Examples</a> •
  <a href="#known-issues">Known issues</a> •
  <a href="#license">License</a>
</p>
<br>

## Key Features

 * Open source C++ capture library compatible with C++11 standard
 * Video Capture
    - raw YUV 4:2:2 data format
    - Camera controls
    - HIGH-RESOLUTION mode with USB3
    - compatibility VGA mode with USB2
 * Sensor Data Capture
    - 6-DOF IMU (3-DOF accelerometer + 3-DOF gyroscope)
    - 3-DOF Magnetometer (Only ZED 2 and ZED 2i)
    - Barometer (Only ZED 2 and ZED 2i)
    - Sensors temperature (Only ZED 2 and ZED 2i)
 * Sensors/video Synchronization
 * Portable
    - Tested on Linux
    - Tested on x64, ARM
 * Small Size
    - ~100KB library size
    - libusb, hidapi dependencies
 * Complete set of examples
    - Video capture
    - Camera control
    - Stereo rectification
    - IMU, magnetometer, and barometer data capture
    - Video and sensor synchronization
    - Disparity/Depth/Point Cloud extraction using OpenCV Transparent API
    - Depth tuning using OpenCV control GUI

## Description

The ZED Open Capture is a multi-platform, open-source C++ library for low-level camera and sensor capture for the ZED stereo camera family. It doesn't require CUDA and therefore can be used on many desktop and embedded platforms.

The open-source library provides methods to access raw video frames, calibration data, camera controls, and raw data from the USB3 camera sensors (on ZED 2, ZED 2i, and ZED Mini). A synchronization mechanism is provided to get the correct sensor data associated with a video frame.

**Note:** While in the ZED SDK all output data is calibrated and compensated, here the extracted raw data is not corrected by the camera and sensor calibration parameters. You can retrieve camera and sensor calibration data using the [ZED SDK](https://www.stereolabs.com/docs/video/camera-calibration/) to correct your camera data [see `zed_open_capture_rectify_example` example](#running-the-examples).

## Build and install

### Prerequisites

 * USB3 Stereolabs Stereo camera: [ZED 2i](https://www.stereolabs.com/zed-2i/), [ZED 2](https://www.stereolabs.com/zed-2/), [ZED](https://www.stereolabs.com/zed/), [ZED Mini](https://www.stereolabs.com/zed-mini/)
 * Linux OS
 * GCC (v7.5+)
 * CMake (v3.1+)
 * OpenCV (v3.4.0+) -Optional for the examples- 

### Install prerequisites

* Install GCC compiler and build tools

    `sudo apt install build-essential`

* Install CMake build system

    `sudo apt install cmake`

* Install HIDAPI and LIBUSB libraries:

    `sudo apt install libusb-1.0-0-dev libhidapi-libusb0 libhidapi-dev`

* Install OpenCV to build the examples (optional)

    `sudo apt install libopencv-dev libopencv-viz-dev`

### Clone the repository

```bash
git clone https://github.com/stereolabs/zed-open-capture.git
cd zed-open-capture
```

### Add udev rule

Stereo cameras such as ZED 2 and ZED Mini have built-in sensors (e.g. IMU) that are identified as USB HID devices.
To be able to access the USB HID device, you must add a udev rule contained in the `udev` folder:

```bash
cd udev
bash install_udev_rule.sh
cd ..
```

### Build

#### Build library and examples

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

#### Build only the library

```bash
mkdir build
cd build
cmake .. -DBUILD_EXAMPLES=OFF
make -j$(nproc)
```

#### Build only the video capture library

```bash
mkdir build
cd build
cmake .. -DBUILD_SENSORS=OFF -DBUILD_EXAMPLES=OFF
make -j$(nproc)
```

#### Build only the sensor capture library

```bash
mkdir build
cd build
cmake .. -DBUILD_VIDEO=OFF -DBUILD_EXAMPLES=OFF
make -j$(nproc)
```

### Install

To install the library, go to the `build` folder and launch the following commands:

```bash
sudo make install
sudo ldconfig
```

## Run

### Get video data

Include the `videocapture.hpp` header, declare a `VideoCapture` object, and retrieve a video frame (in YUV 4:2:2 format) with `getLastFrame()`:

```C++
#include "videocapture.hpp"

sl_oc::video::VideoCapture cap;
cap.initializeVideo();
const sl_oc::video::Frame frame = cap.getLastFrame();
```
    
### Get sensor data

Include the `SensorCapture` header, declare a `SensorCapture` object, get a list of available devices, initialize the first one, and finally retrieve sensor data:

```C++
#include "sensorcapture.hpp"

sl_oc::sensors::SensorCapture sens;
std::vector<int> devs = sens.getDeviceList();
sens.initializeSensors( devs[0] );
const sl_oc::sensors::data::Imu imuData = sens.getLastIMUData(5000);
const sl_oc::sensors::data::Magnetometer magData = sens.getLastMagnetometerData(100);
const sl_oc::sensors::data::Environment envData = sens.getLastEnvironmentData(100);
const sl_oc::sensors::data::Temperature tempData = sens.getLastCameraTemperatureData(100);
```
    
## Running the examples

After installing the library and examples, you will have the following sample applications in your `build` directory:

* [zed_open_capture_video_example](https://github.com/stereolabs/zed-open-capture/blob/master/examples/zed_oc_video_example.cpp): This application captures and displays video frames from the camera.
* [zed_open_capture_multicam_video_example](https://github.com/stereolabs/zed-open-capture/blob/master/examples/zed_oc_multi_video_example.cpp): This application captures and displays video frames from two cameras.
* [zed_open_capture_control_example](https://github.com/stereolabs/zed-open-capture/blob/master/examples/zed_oc_control_example.cpp): This application captures and displays video frames from the camera and provides runtime control of camera parameters using keyboard shortcuts.
* [zed_open_capture_rectify_example](https://github.com/stereolabs/zed-open-capture/blob/master/examples/zed_oc_rectify_example.cpp): This application downloads factory stereo calibration parameters from Stereolabs server, performs stereo image rectification and displays original and rectified frames.
* [zed_open_capture_sensors_example](https://github.com/stereolabs/zed-open-capture/blob/master/examples/zed_oc_sensors_example.cpp): This application creates a `SensorCapture` object and displays on the command console the values of camera sensors acquired at full rate.
* [zed_open_capture_sync_example](https://github.com/stereolabs/zed-open-capture/blob/master/examples/zed_oc_sync_example.cpp): This application creates a `VideoCapture` and a `SensorCapture` object, initialize the camera/sensors synchronization and displays on screen the video stream with the synchronized IMU data.
* [zed_open_capture_depth_example](https://github.com/stereolabs/zed-open-capture/blob/master/examples/zed_oc_depth_example.cpp): This application captures and displays video frames, calculates disparity map, then extracts the depth map and the point cloud displaying the result and the estimation of the performance.
* [zed_open_capture_depth_tune_stereo](https://github.com/stereolabs/zed-open-capture/blob/master/examples/tools/zed_oc_tune_stereo_sgbm.cpp): This application captures the first available stereo frames and provides GUI Controls to tune the disparity map results and save them to be used in the `zed_open_capture_depth_example` example

To run the examples, open a terminal console and enter one of the following commands:

```bash
zed_open_capture_video_example
zed_open_capture_multicam_video_example
zed_open_capture_control_example
zed_open_capture_rectify_example
zed_open_capture_sensors_example
zed_open_capture_sync_example
zed_open_capture_depth_example
zed_open_capture_depth_tune_stereo
```

**Note:** OpenCV is used in the examples for controls, display, and depth extraction.


## Documentation

The API is documented in the Include.h files. It is also generated as a Doxygen for simpler navigation: https://stereolabs.github.io/zed-open-capture

You can also generate the documentation locally in HTML format (with Doxygen) using the commands below. Access the docs by opening `doc/html/index.html` in your web browser.

```bash
sudo apt-get install -y doxygen # if not previously installed
cd doc
./generate_doc.sh
```

## Coordinates system

The coordinate system is only used for sensor data. The given IMU and Magnetometer data are expressed in the RAW coordinate system as shown below

![](./images/imu_axis.jpg)

## Known issues

### OpenGL version
On some embedded devices, like Raspberry Pi 4, the depth extraction example can crash with the following error:

`vtkShaderProgram.cxx:438    ERR| vtkShaderProgram (0x23a611c0): 0:1(10): error: GLSL 1.50 is not supported. Supported versions are: 1.10, 1.20, 1.00 ES, and 3.00 ES`

to correctly execute the example application it is necessary to change the default OpenGL version:

```
export MESA_GL_VERSION_OVERRIDE=3.2
```

you can permanently add this configuration by adding the above command as the last line of the `~/.bashrc` file.

## License

This library is licensed under the MIT License.

