#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <cstdio>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  std::string filename = argv[1];
  cv::VideoCapture cap(filename);
  if (!cap.isOpened()) {
    printf("Failed to open video file %s\n", filename.c_str());
    return -1;
  }

  cv::gpu::setDevice(0);
  printf("Getting GPU device info...\n");
  auto device_info = cv::gpu::DeviceInfo(0);
  auto free = device_info.freeMemory(), total = device_info.totalMemory();
  printf("%zu / %zu bytes free\n", free, total);
  printf("%f / %f MB free\n", free/1024.0/1024.0, total/1024.0/1024.0);

  cv::Mat header;
  cap >> header;

  cv::gpu::GpuMat buffer(header.size(), CV_32FC3),
                  average(header.size(), CV_32FC3),
                  empty;

  int frame_count = 0;
  average.upload(cv::Mat(header.size(), CV_32FC3));

  for (;; frame_count++) {
  // for (;frame_count < 200; frame_count++) {
    // now can copy the page_locked to GPU in asynchronous fashion
    // stream.enqueueUpload(page_locked, buffer);
    if (header.empty())
      break;
    buffer.upload(header);
    // buffer.convertTo(buffer, CV_32FC3, 1.0/255.0);
    buffer.convertTo(buffer, CV_32FC3);
    cv::gpu::add(average, buffer, average, empty, CV_32FC3);
    cap >> header;
  }
  printf("%d frames\n", frame_count);
  average.download(header);
  // result *= 255;
  header /= frame_count;
  // result = average;
  // buffer.convertTo(buffer, CV_32FC3, 255.0);

  // std::string outfile = filename.substr(0, filename.find_last_of('.')) + ".png";
  std::string outfile = filename.substr(filename.find_last_of('/')+1, filename.find_last_of('.')) + ".png";
  cv::imwrite(outfile, header);

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
