/**
 * @file Camera.cpp
 * @author lsvng
 * @brief H264 Video Stream Codec example using openCV
 * 
 */

#include <H264Encoder.hpp>
#include <H264Decoder.hpp>

#include <chrono>
#include <memory>
#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>

using namespace Codec;

int main()
{
  int wPacketSize = 0;
  uint8_t* wEncodedData = nullptr;
  uint8_t* wDecodedData = nullptr;

  auto encoder = std::make_unique<H264Encoder>();
  auto decoder = std::make_unique<H264Decoder>();

  // Init OpenCV objects
  cv::Mat image;
  cv::namedWindow("Display window");
  cv::VideoCapture cap(0);
  
  if (!cap.isOpened())
  {
    std::cout << "Cannot open camera" << std::endl;
    return EXIT_FAILURE;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

  // Main loop
  for (;;)
  {
    cap >> image;
    
    wEncodedData = encoder->encode(image.data);

    if (nullptr != wEncodedData)
    {
      wPacketSize = encoder->getPacketSize();
    }

    // ... _____________      _____________
    // ... | Machine A | ---> | Machine B |
    // ...
    
    wDecodedData = decoder->decode(wEncodedData, wPacketSize);

    // Set to cv::Mat
    cv::Mat mat(720, 1280, CV_8UC3);

    if (nullptr != wEncodedData && nullptr != wDecodedData)
    {
      mat.data = (uchar*)wDecodedData;
    }
    
    cv::imshow("Display window", mat);

    if (cv::waitKey(10) == 'q')
    {
      // Press 'q' to exit the program.
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return EXIT_SUCCESS;
}
