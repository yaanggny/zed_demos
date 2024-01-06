///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2023, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


#include <sl/Camera.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace sl;


// Mapping between MAT_TYPE and CV_TYPE
int getOCVtype(sl::MAT_TYPE type) {
    int cv_type = -1;
    switch (type) {
        case MAT_TYPE::F32_C1: cv_type = CV_32FC1; break;
        case MAT_TYPE::F32_C2: cv_type = CV_32FC2; break;
        case MAT_TYPE::F32_C3: cv_type = CV_32FC3; break;
        case MAT_TYPE::F32_C4: cv_type = CV_32FC4; break;
        case MAT_TYPE::U8_C1: cv_type = CV_8UC1; break;
        case MAT_TYPE::U8_C2: cv_type = CV_8UC2; break;
        case MAT_TYPE::U8_C3: cv_type = CV_8UC3; break;
        case MAT_TYPE::U8_C4: cv_type = CV_8UC4; break;
        default: break;
    }
    return cv_type;
}

/**
* Conversion function between sl::Mat and cv::Mat
**/
cv::Mat slMat2cvMat(sl::Mat& input) {
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), getOCVtype(input.getDataType()), input.getPtr<sl::uchar1>(MEM::CPU), input.getStepBytes(sl::MEM::CPU));
}

int main(int argc, const char *const *argv) 
{
    if(argc < 2)
    {
        printf("Usage: %s <mode[=0,1,2]>\n", argv[0]);
        return -1;
    }

    int mode = atoi(argv[1]);

    // Create a ZED camera object
    Camera zed;

    // Set configuration parameters
    InitParameters init_parameters;
    init_parameters.camera_resolution = RESOLUTION::HD720; // Use HD720 opr HD1200 video mode, depending on camera type.
    init_parameters.camera_fps = 30; // Set fps at 30

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != ERROR_CODE::SUCCESS) {
        cout << "Error " << returned_state << ", exit program." << endl;
        return EXIT_FAILURE;
    }

    // Capture 50 frames and stop
    int i = 0;
    sl::Mat imgL, imgR, imgLR;
    cv::Mat cimgL, cimgR, cimgLR;
    while (i < 50) {
        // Grab an image
        returned_state = zed.grab();
        // A new image is available if grab() returns ERROR_CODE::SUCCESS
        if (returned_state == ERROR_CODE::SUCCESS) 
        {
            switch (mode) 
            {
                case 0:
                {
                    // Get the left,right image
                    zed.retrieveImage(imgL, VIEW::LEFT);  // LAST, SIDE_BY_SIDE, LEFT_UNRECTIFIED
                    zed.retrieveImage(imgL, VIEW::RIGHT);

                    cimgL = slMat2cvMat(imgL);
                    cimgR = slMat2cvMat(imgR);
                    cv::Mat tmp[2] = {cimgL, cimgR};
                    cv::hconcat(tmp, 2, cimgLR);
                    break;
                }
                case 1:
                {
                    zed.retrieveImage(imgLR, VIEW::SIDE_BY_SIDE);
                    cimgLR = slMat2cvMat(imgLR);
                    break;
                }
                default:
                {
                    zed.retrieveImage(imgLR, VIEW::LAST);
                    cimgLR = slMat2cvMat(imgLR);
                }
            }

            cv::imshow("Rectified left-right", cimgLR);         
            cv::waitKey(5);   
            // Display the image resolution and its acquisition timestamp
            cout<<"Image resolution: "<< imgL.getWidth()<<"x"<<imgL.getHeight() <<" || Image timestamp: "<<imgL.timestamp.data_ns<<endl;
            i++;
        }
    }

    // Close the camera
    zed.close();
    return EXIT_SUCCESS;
}
