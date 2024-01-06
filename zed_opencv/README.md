## ZED work with OpenCV

Based on [ZED-OpenCV](https://github.com/stereolabs/zed-opencv).

## Build
### Requirements
- [ZED SDK](https://www.stereolabs.com/developers/release)
- CUDA (can be automatically installed during the SDK installation)
- OpenCV
- cmake


## Run the program

- Navigate to the build directory and launch the executable file
- Or open a terminal in the build directory and run the sample :

        ./ZED\ with\ OpenCV [path to SVO file]

You can optionally provide an SVO file path (recorded stereo video of the ZED).

### Keyboard shortcuts

This table lists keyboard shortcuts that you can use in the sample application.

Parameter             | Description                   |   Hotkey
---------------------|------------------------------------|-------------------------------------------------
Save Side by Side      | Save side by side image.       |   's'                             
Save Depth             | Save depth image.              |   'p'                              
Save Point Cloud       | Save 3D point cloud.        |   'd'
Switch cloud format    | Toggle between point cloud formats.    |   'm'
Switch depth format    | Toggle between depth image formats. |   'n'                                                      
Exit         | Quit the application.             | 'q'
