#include "../include/rscv.h"
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  /****************************************************
   * ==>> 初始化
   ****************************************************/
  // 声明Realsense管道
  rs2::pipeline pipe;
  // 创建一个rs2::frameset对象, 包含一组帧和访问他们的接口
  rs2::frameset frames;
  // 声明彩色图
  rs2::colorizer color_map;
  // 配置管道的数据流信息
  rs2::config cfg;
  cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
  // 启动设备的管道配置文件, 开始传送数据流
  rs2::pipeline_profile selection = pipe.start(cfg);

  // 打开视频路劲，设置基本信息 参数跟上面给出的VideoWriter函数是一样的
  cv::VideoWriter outputVideo;
  outputVideo.open("./video.avi", cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), 
      30.0, cv::Size(640,480), true);
  if(!outputVideo.isOpened()) {
    std::cout << "Fail to open video!" << std::endl;
    return -1;
  }

  cv::Mat img;
  // 储存直线信息
  std::vector<cv::Vec4i> tline;
  std::string dir = "./";
  double delta_x, delta_y, k, h;
  double h_min;
  int id, flag;

  while(true) {
    frames = pipe.wait_for_frames();    // 等待下一帧

    // Get each frame
    rs2::frame color_frame = frames.get_color_frame();  // 获取彩色图

    // 创建Opencv中的Mat类,并传入数据
    cv::Mat src(cv::Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), 
        cv::Mat::AUTO_STEP);

    // 显示图片
    cv::imshow("LineImage", src);
    // 保存视频
    outputVideo << src;

    // 等待n ms;如果有键盘输入则退出循环; 否则n ms后跳过并执行下一个循环
    // 在退出的时候,所有数据的内存空间将会被自动释放
    if(cv::waitKey(10) >= 0){
      cv::imwrite(dir+"saved"+".jpg",src);
      break;
    }
  }  // while

  // 释放窗口
  cv::destroyWindow("LineImage");
  
  return 0;
}
