# Video Processor

Dev Environment : Debian 11, cmake

External Libraries : OpenCV (4.5.1-dev)

This present code reads a corrupted video file. After processing each frame, it reorders the frame, removing intruders.
The output is a new corrected video, in mp4 format.

## Build 

In the main repository:

```bash
cmake .
make
```

# Configuration

Configuration template is provided in the repository.

```bash
debug=false # to print detailed logs in a file, and save the frames of the corrected video in order, in the output_directory
video_filename=../Data/corrupted_video.mp4 # the video file to analyse
output_directory=../Data/output/ # the output directory where the new video will be and the debug logs
matches_threshold=190 # Descriptor Matches Threshold between Frames, to consider there is enough similarity.
reverse_video=true # To reverse to order of the new frames in the video
```

# Run 

```bash
./CorruptedToCorrected <config_file_path>
```

# Some borrowed code

https://gist.github.com/itsrifat/66b253db2736b23f247c
