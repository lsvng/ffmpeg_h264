/**
 * @file H264Decoder.hpp
 * @author lsvng
 * @brief H264 Video Stream Codec
 * 
 */

#ifndef H264Decoder_HPP
#define H264Decoder_HPP

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
  class H264Decoder
  {
    public:
      /**
       * @brief Construct a new Video Decoder object
       * 
       */
      H264Decoder();

      /**
       * @brief Destroy the Video Decoder object
       * 
       */
      ~H264Decoder();

      /**
       * @brief Disable copy constructor
       * 
       * @param iH264Decoder 
       */
      H264Decoder(H264Decoder& iH264Decoder) = delete;

      /**
       * @brief Disable assignment constructor
       * 
       * @param iH264Decoder 
       */
      void operator=(const H264Decoder& iH264Decoder) = delete;
      
      /**
       * @brief Decode the encoded video stream
       * 
       * @param iEncodedData 
       * @param iSize 
       * @return uint8_t 
       */
      uint8_t* decode(uint8_t* iEncodedData, ssize_t iSize);

    private:
      /**
       * @brief Fill vector with AVFrame
       * 
       * @param iFrame Represents a single video or audio frame
       * @return uint8_t 
       */
      uint8_t* decodedData(const AVFrame* iFrame);

    private:
      AVCodecContext* mContext; // Used to contain information about the codec used for encoding or decoding audio or video streams
      AVPacket* mPacket;        // Used to represent compressed audio or video data
      AVFrame* mFrame;          // Used to represent a single video or audio frame
  };
}

#endif /* H264Decoder_HPP */
