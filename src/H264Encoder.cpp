/**
 * @file H264Encoder.cpp
 * @author lsvng
 * @brief H264 VEncoder
 * 
 */

#include <H264Encoder.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace Codec
{
  H264Encoder::H264Encoder()
    : mContext(nullptr)
    , mPacket(nullptr)
    , mFrame(nullptr)
  {
    av_register_all();

    AVCodec* wCodec;

    int wSize;
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
    wCodec = avcodec_find_encoder_by_name("libx264rgb");
    if (!wCodec)
    {
      printf("Error: Cannot find encoder.\n");
      exit(1);
    }

    mContext = avcodec_alloc_context3(wCodec);
    if (!mContext)
    {
      printf("ERROR: Cannot start codec context.\n");
      exit(1);
    }

    av_opt_set(mContext->priv_data, "tune", "zerolatency", 0);
    av_opt_set(mContext->priv_data, "preset", "ultrafast", 0);

    mContext->bit_rate     = wBitRate;
    mContext->width        = wImageWidth; // resolution must be a multiple of two..
    mContext->height       = wImageHeight;
    mContext->time_base    = {1, (int)wTimeBase};
    mContext->gop_size     = wGOPSize;
    mContext->max_b_frames = wMaxBFrame;
    mContext->pix_fmt      = AV_PIX_FMT_RGB24;

    int wRet;
    wRet = avcodec_open2(mContext, wCodec, nullptr);
    if (wRet < 0)
    {
      printf("ERROR: Cannot opening codec.\n");
      exit(1);
    }

    mPacket = av_packet_alloc();
    mPacket->data = nullptr;
    mPacket->size = 0;

    mFrame = av_frame_alloc();
    if (!mFrame) 
    {
      printf("ERROR: Could not allocate video frame\n");
      exit(1);
    }

    mFrame->format = mContext->pix_fmt;
    mFrame->width = mContext->width;
    mFrame->height = mContext->height;
    mFrame->linesize[0] = 3 * mFrame->width;

    wRet = av_frame_get_buffer(mFrame, 0);
    if (wRet < 0)
    {
      printf("ERROR: Cannot get frame buffer.\n");
      exit(1);
    }

    printf("FFmpeg encoder is ready.\n");
  }

  H264Encoder::~H264Encoder()
  {
    av_frame_free(&mFrame);
    avcodec_free_context(&mContext);
    av_packet_free(&mPacket);

    printf("End of H264Encoder program.\n");
  }

  uint8_t* H264Encoder::encode(uint8_t* iRawData)
  {
    if (!iRawData)
    {
      return nullptr;
    }

    mFrame->data[0] = iRawData;
    // R: mFrame->data[0][(3 * i * mImageWidth) + (j * 3) + 0]
    // G: mFrame->data[0][(3 * i * mImageWidth) + (j * 3) + 1]
    // B: mFrame->data[0][(3 * i * mImageWidth) + (j * 3) + 2]

    // Send mFrame to FFmpeg codec
    send(mContext, mFrame);

    // Receive mPacket from FFmpeg codec
    receive(mContext, mPacket);

    // Return encoded video data
    return mPacket->data;
  }

  int H264Encoder::getPacketSize()
  {
    if (!mPacket->data)
    {
      return 0;
    }

    return mPacket->size;
  }

  void H264Encoder::send(AVCodecContext* iContext, AVFrame* iFrame)
  {
    int wRet = avcodec_send_frame(mContext, mFrame);
    if (wRet < 0)
    {
      printf("Cannot send packets. Error: %d\n", wRet);
      iFrame = nullptr;
    }
  }

  void H264Encoder::receive(AVCodecContext* iContext, AVPacket* iPacket)
  {
   int wRet = avcodec_receive_packet(iContext, iPacket);
    if (wRet < 0)
    {
      printf("Cannot encode frame. Error: %d\n", wRet);
      iPacket = nullptr;
    }
  }
}
