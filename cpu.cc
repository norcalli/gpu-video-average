#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <cstdio>
#include <string>
#include <iostream>

std::string getImgType(int imgTypeInt) {
    int numImgTypes = 35; // 7 base types, with five channel options each (none or C1, ..., C4)

    int enum_ints[] =       {CV_8U,  CV_8UC1,  CV_8UC2,  CV_8UC3,  CV_8UC4,
                             CV_8S,  CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
                             CV_16U, CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
                             CV_16S, CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
                             CV_32S, CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
                             CV_32F, CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
                             CV_64F, CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4};

    std::string enum_strings[] = {"CV_8U",  "CV_8UC1",  "CV_8UC2",  "CV_8UC3",  "CV_8UC4",
                             "CV_8S",  "CV_8SC1",  "CV_8SC2",  "CV_8SC3",  "CV_8SC4",
                             "CV_16U", "CV_16UC1", "CV_16UC2", "CV_16UC3", "CV_16UC4",
                             "CV_16S", "CV_16SC1", "CV_16SC2", "CV_16SC3", "CV_16SC4",
                             "CV_32S", "CV_32SC1", "CV_32SC2", "CV_32SC3", "CV_32SC4",
                             "CV_32F", "CV_32FC1", "CV_32FC2", "CV_32FC3", "CV_32FC4",
                             "CV_64F", "CV_64FC1", "CV_64FC2", "CV_64FC3", "CV_64FC4"};

    for (int i=0; i<numImgTypes; i++) {
        if(imgTypeInt == enum_ints[i]) return enum_strings[i];
    }
    return "unknown image type";
}


int main(int argc, char const *argv[]) {
  cv::VideoCapture cap("Alizee - J'en Ai Marre (Tubes) Remastered [720p].mp4"); // open video stream for(;;)

  cv::Mat buffer(720, 960, CV_32FC3), average(720, 960, CV_32FC3);
  // for (;;) {
  int frame_count = 0;
  for (; frame_count < 200; frame_count++) {
    //read video frame to the header which actually points to page-locked buffer
    cap >> buffer;
    // std::cout << "type: " << getImgType(buffer.type()) << ", channels: " << buffer.channels() << '\n';
    // break;
    if (buffer.empty())
      break;
    // now can copy the page_locked to GPU in asynchronous fashion
    // stream.enqueueUpload(page_locked, buffer);

    // void gpu::addWeighted(const GpuMat& src1, double alpha, const GpuMat& src2, double beta, double gamma, GpuMat& dst, int dtype=-1, Stream& stream=Stream::Null())
    // gpu::addWeighted(average, 0.95, buffer, 0.05, 0.0, average, -1, stream);
    // cv::addWeighted(average, 0.95, buffer, 0.05, 0.0, average, CV_32FC3);
    cv::add(average, buffer, average, cv::noArray(), CV_32FC3);
  }
  average /= frame_count;
  cv::imwrite("output.png", average);

  // // asynchronous case Stream stream;
  // gpu::Stream stream;
  // stream.enqueueUpload(host_image, device_image);
  // a_opencv_gpu_function(device_image, output, stream);

  // stream.enqueueDownload(output, ouput_host);
  // // CPU resources are available.
  // // Let’s compute on CPU here while GPU does own work.
  // stream.waitForCompletion(); // output_host is ready!
  return 0;
}
