#gpu-video-average

##Usage

Call `gpu_average` on the filename and it'll output a png (with default level 3 compression) in the same folder as `gpu_average`:

```
./gpu_average <filename>
```

##Compilation

I wrote it on my Macbook, so I used Macports to install `opencv` and `cmake`. I also linked to `/opt/local/lib/` to find the libraries. I'm not sure if it works on Windows or Linux, but I'd love to hear if it does.

###Requirements:
- [CUDA Toolkit](https://developer.nvidia.com/cuda-downloads)
- OpenCV
- Nvidia GPU
- Cmake

###Macports OpenCV Installation:
Eventually I'll make a patch, but I didn't have a vanilla version of the Portfile to diff against, but for now, OpenCV doesn't support CUDA by default. Copy my Portfile using this command:

```
sudo cp Portfile $(port edit --editor echo opencv)
```

You can install OpenCV with CUDA support then with the following command (feel free to add whatever other variants you want, I added `+tbb +eigen +python27`):

```
sudo port install opencv +cuda
```

##TODO:
- Write a patchfile for OpenCV CUDA.
- Write a version for AMD as well using OpenCL.
