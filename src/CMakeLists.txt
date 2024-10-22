add_executable(src
        seek.cpp
        seek.hpp

        )
target_link_libraries(src PUBLIC
        FFmpeg::avformat
        FFmpeg::avcodec
        FFmpeg::avutil
        FFmpeg::swscale
        util
        benchmark::benchmark
)
find_package(FFmpeg REQUIRED COMPONENTS avformat avcodec avutil swscale)

