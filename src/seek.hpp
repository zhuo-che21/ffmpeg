//
// Created by jacky on 2022/6/26.
//

#ifndef PROJECT2_SEEK_HPP
#define PROJECT2_SEEK_HPP
#include<stdio.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
}
struct decoder_info
{
	int timebase_rational_num;
	int timebase_rational_den;
	double timebase;
	char* codec_name;
	int64_t duration;
};

struct frame_info
{
	int width;
	int height;
	double timebase;
	int format;
	uint8_t *data[8];
	int linesize[8];
};

//typedef struct {
//	char * data;
//	size_t size;
//	size_t offset;
//}ImageSource;

#endif //PROJECT2_SEEK_HPP
