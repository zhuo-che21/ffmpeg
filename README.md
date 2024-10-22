# Project 2: ffmpeg and multimedia processing

This repository contains base code for project 2. Please refer to handout for detailed information.

# How to get dependencies

## ffmpeg

On Windows, please download
`ffmpeg-n4.4-latest-win64-gpl-shared-4.4.zip` from https://github.com/BtbN/FFmpeg-Builds/releases/tag/latest
or https://cloud.tsinghua.edu.cn/d/27439285b1614e3cb745/.
Exact the zip file, rename the folder as `ffmpeg-windows`, and put it under
the `external` directory of the project.

On macOS, please install ffmpeg with HomeBrew:
```
homebrew install ffmpeg
```

On Linux or WSL, please use the package manager of respective system. For Debian
or Ubuntu, the following command should suffice:
```
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev
```

The Tsinghua Cloud link provided above
(https://cloud.tsinghua.edu.cn/d/27439285b1614e3cb745/) also contains additional
video samples and reading material.

## git submodules

After cloning, use the following command to initialize all git submodules
```
git submodule update --init --depth=1
```

The repository uses CMake, after these two steps you should be able to open it with your IDE.
