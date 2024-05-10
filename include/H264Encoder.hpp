/**
 * @file H264Encoder.hpp
 * @author lsvng
 * @brief H264 Video Stream Codec
 * 
 */

#ifndef H264Encoder_HPP
#define H264Encoder_HPP

#include <vector>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libavutil/hwcontext.h"
}

namespace Codec
{
  class H264Encoder
  {
    public:
      /**
       * @brief Construct a new Video Encoder object
       * 
       */
      H264Encoder();

      /**
       * @brief Destroy the Video Encoder object
       * 
       */
      ~H264Encoder();

      /**
       * @brief Disable copy constructor
       * 
       * @param iH264Encoder 
       */
      H264Encoder(H264Encoder& iH264Encoder) = delete;

      /**
       * @brief Disable assignment constructor
       * 
       * @param iH264Encoder 
       */
      void operator=(const H264Encoder& iH264Encoder) = delete;
      
      /**
       * @brief Encode the raw video stream
       * 
       * @param iRawData 
       * @return uint8_t 
       */
      uint8_t* encode(uint8_t* iRawData);

      /**
       * @brief Get the size of packet
       * 
       * @return int
      */
      int getPacketSize();

    protected:
      /**
       * @brief Send AVFrame to FFmpeg codec
       * 
       * @param iContext
       * @param iPacket
      */
      virtual void send(AVCodecContext* iContext, AVFrame* iFrame);
      
      /**
       * @brief Receive AVPacket from FFmpeg codec
       * 
       * @param iContext
       * @param iFrame
      */
      virtual void receive(AVCodecContext* iContext, AVPacket* iPacket);

    protected:
      AVCodecContext* mContext; // Used to contain information about the codec used for encoding or decoding audio or video streams
      AVPacket* mPacket;        // Used to represent compressed audio or video data
      AVFrame* mFrame;          // Used to represent a single video or audio frame
  };
}

#endif /* H264Encoder_HPP */
