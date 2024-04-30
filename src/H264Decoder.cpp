/**
 * @file H264Decoder.cpp
 * @author lsvng
 * @brief H264 Decoder
 * 
 */

#include <H264Decoder.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace Codec
{
  H264Decoder::H264Decoder()
    : mContext(nullptr)
    , mPacket(nullptr)
    , mFrame(nullptr)
  {
    AVCodec* wCodec;

    int wBitRate;
    int wImageWidth;
    int wImageHeight;
    int wTimeBase;
    int wGOPSize;
    int wMaxBFrame;
    int wStreamRate;

    //Paese ini file
    boost::property_tree::ptree wPt;
    boost::property_tree::ini_parser::read_ini("../settings.ini", wPt);

    wBitRate     = wPt.get<int>("FFmpeg.BitRate");
    wImageWidth  = wPt.get<int>("FFmpeg.ImageWidth");
    wImageHeight = wPt.get<int>("FFmpeg.ImageHeight");
    wTimeBase    = wPt.get<int>("FFmpeg.TimeBase");
    wGOPSize     = wPt.get<int>("FFmpeg.GOPSize");
    wMaxBFrame   = wPt.get<int>("FFmpeg.MaxBFrame");
    wStreamRate  = wPt.get<int>("FFmpeg.StreamRate");

    // Configure FFmpeg
    wCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!wCodec < 0)
    {
      printf("Error: Cannot find decoder.\n");
      exit(1);
    }

    mContext = avcodec_alloc_context3(wCodec); // decoder plug in
    if (!mContext)
    {
      printf("ERROR: Cannot allocate codec context.\n");
      exit(1);
    }

    av_opt_set(mContext->priv_data, "preset", "ultrafast", 0);
    av_opt_set(mContext->priv_data, "tune", "zerolatency", 0);

    mPacket = av_packet_alloc();
    mPacket->data = nullptr;
    mPacket->size = 0;
    
    mFrame = av_frame_alloc();
    if (!mFrame) 
    {
      printf("Could not allocate video frame\n");
      exit(1);
    }

    mFrame->format = mContext->pix_fmt;
    mFrame->width = mContext->width;
    mFrame->height = mContext->height;

    // avcodec_open2(mContext, wCodec, nullptr);
    if (avcodec_open2(mContext, wCodec, nullptr) < 0)
    {
      printf("ERROR: Cannot opening codec.\n");
      exit(1);
    }

    printf("FFmpeg Decoder is ready.\n");
  }

  H264Decoder::~H264Decoder()
  {
    av_frame_free(&mFrame);
    av_packet_free(&mPacket);
    avcodec_free_context(&mContext);

    printf("End of Decoder program.\n");
  }

  uint8_t* H264Decoder::decode(uint8_t* iEncodedData, ssize_t iSize)
  {
    if (!iEncodedData)
    {
      return nullptr;
    }

    mPacket->data = iEncodedData;
    mPacket->size = iSize; // packet length
    // R: mFrame->data[0][(3 * i * mImageWidth) + (j * 3) + 0]
    // G: mFrame->data[0][(3 * i * mImageWidth) + (j * 3) + 1]
    // B: mFrame->data[0][(3 * i * mImageWidth) + (j * 3) + 2]
    
    int wRet;
    wRet = avcodec_send_packet(mContext, mPacket);
    if (wRet < 0)
    {
      printf("Cannot send packets. Error: %d\n", wRet);
      return nullptr;
    }

    wRet = avcodec_receive_frame(mContext, mFrame);
    if (wRet < 0)
    {
      printf("Cannot receive frame. Error: %d\n", wRet);
      return nullptr;
    }
    
    // Return decoded video data
    return mFrame != nullptr ? &mFrame->data[0][0] : nullptr;
  }
}
