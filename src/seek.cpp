//
// Created by jacky on 2022/6/26.
//
#include "seek.hpp"
#include<stdio.h>
#include<string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../external/stb/stb_image_resize.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}


static AVCodecContext *pCoctx = NULL;
static AVFormatContext *pFctx = NULL;
static AVFrame *pframe = NULL;

static AVPacket *pkt = NULL;
static int video_stream_idx;
static struct decoder_info decoder_info;

int64_t duration;
uint8_t *rgba;
uint8_t *combine;
uint8_t *resize;
int n;
double timep;
double timeb;
int timet[6][3];

int width;
int height;
int channel;
uint8_t *num1;
uint8_t *num2;
uint8_t *num3;
uint8_t *num4;
uint8_t *num5;
uint8_t *num6;
uint8_t *num7;
uint8_t *num8;
uint8_t *num9;
uint8_t *num0;
uint8_t *colon;
uint8_t *dot;

static void logging(const char *fmt, ...)
{
	va_list args;
	fprintf( stderr, "LOG: " );
	va_start( args, fmt );
	vfprintf( stderr, fmt, args );
	va_end( args );
	fprintf( stderr, "\n" );
}

int open_codec_context(int *stream_idx,enum AVMediaType type)
{
	int ret, stream_index;
	AVStream *st;
	AVCodec *dec = NULL;
	AVDictionary *opts = NULL;

	if ((ret = av_find_best_stream(pFctx, type, -1, -1, NULL, 0)) < 0) {
		fprintf(stderr, "Cannot find %s stream!\n", av_get_media_type_string(type));
	}

	stream_index = ret;
	st = pFctx->streams[stream_index];

	dec = avcodec_find_decoder(st->codecpar->codec_id);
	if (!dec) {
		fprintf(stderr, "Failed to find a codec\n");
		return AVERROR(EINVAL);
	}

	pCoctx = avcodec_alloc_context3(dec);
	if (!pCoctx) {
		fprintf(stderr, "Failed to allocate AVCodecContext\n");
		return AVERROR(ENOMEM);
	}

	if ((ret = avcodec_parameters_to_context(pCoctx, st->codecpar)) < 0) {
		//fprintf(stderr, "Failed to set decoder parameter: %s\n", av_err2str(ret));
		return ret;
	}

	if ((ret = avcodec_open2(pCoctx, dec, &opts)) < 0) {
		//fprintf(stderr, "Failed to open codec: %s\n", av_err2str(ret));
		return ret;
	}

	*stream_idx = stream_index;
	return 0;
}

static void convert_420(){
	SwsContext* swsContext  = sws_getContext(pframe->width, pframe->height, AV_PIX_FMT_YUV420P,
	                                        pframe->width, pframe->height, AV_PIX_FMT_RGBA,
	                                                     SWS_BILINEAR, NULL, NULL, NULL);
	int w =pframe->width;
	int h = pframe->height;
	int linesize[3] = {w, w/2, w/2};
	int rgbline=4*pframe->width;
	sws_scale(swsContext, pframe->data, linesize, 0, pframe->height,
	          &rgba, &rgbline);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < 4*w; ++j) {
			combine[(i+(n/2)*h)*8*w+j+(n%2)*4*w]=rgba[j+i*4*w];
		}
	}
	sws_freeContext(swsContext);
}

static void convert_444(){
	SwsContext* swsContext  = sws_getContext(pframe->width, pframe->height, AV_PIX_FMT_YUV444P,
	                                        pframe->width, pframe->height, AV_PIX_FMT_RGBA,
	                                        SWS_BILINEAR, NULL, NULL, NULL);
	int w =pframe->width;
	int h = pframe->height;
	int linesize[3] = {w, w/2, w/2};
	int rgbline=4*pframe->width;
	sws_scale(swsContext, pframe->data, linesize, 0, pframe->height,
	          &rgba, &rgbline);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < 4*w; ++j) {
			combine[(i+(n/2)*h)*8*w+j+(n%2)*4*w]=rgba[j+i*4*w];
		}
	}
	sws_freeContext(swsContext);

}

static void write(int num, int x, int y, int w, int h){
	switch (num) {
	case 0:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num0[i*width*4+j];
			}
		}
		break;
	case 1:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num1[i*width*4+j];
			}
		}
		break;
	case 2:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num2[i*width*4+j];
			}
		}
		break;
	case 3:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num3[i*width*4+j];
			}
		}
		break;
	case 4:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num4[i*width*4+j];
			}
		}
		break;
	case 5:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num5[i*width*4+j];
			}
		}
		break;
	case 6:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num6[i*width*4+j];
			}
		}
		break;
	case 7:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num7[i*width*4+j];
			}
		}
		break;
	case 8:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num8[i*width*4+j];
			}
		}
		break;
	case 9:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=num9[i*width*4+j];
			}
		}
		break;
	case -1:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=colon[i*width*4+j];
			}
		}
		break;
	case -2:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				combine[x+y*w+j+i*w]=dot[i*width*4+j];
			}
		}
		break;
	}

}

static void writere(int num, int x, int y, int w, int h){
	switch (num) {
	case 0:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num0[i*width*4+j];
			}
		}
		break;
	case 1:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num1[i*width*4+j];
			}
		}
		break;
	case 2:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num2[i*width*4+j];
			}
		}
		break;
	case 3:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num3[i*width*4+j];
			}
		}
		break;
	case 4:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num4[i*width*4+j];
			}
		}
		break;
	case 5:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num5[i*width*4+j];
			}
		}
		break;
	case 6:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num6[i*width*4+j];
			}
		}
		break;
	case 7:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num7[i*width*4+j];
			}
		}
		break;
	case 8:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num8[i*width*4+j];
			}
		}
		break;
	case 9:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=num9[i*width*4+j];
			}
		}
		break;
	case -1:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=colon[i*width*4+j];
			}
		}
		break;
	case -2:
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width*4; ++j) {
				resize[x+y*w+j+i*w]=dot[i*width*4+j];
			}
		}
		break;
	}

}

static int decode_video(int64_t timestamp){
	if (avcodec_send_packet(pCoctx,pkt) < 0){
		//logging("ERROR send packet");
		return -1;
	}
	while (1){
		av_frame_unref(pframe);
		if (avcodec_receive_frame(pCoctx,pframe) < 0){
			//logging("ERROR receive frame");
			return -1;
		}
		//logging("get frame :%d", pframe->best_effort_timestamp);
		if (pframe->best_effort_timestamp < timestamp){

			return 2;
		}
		timep = double(pframe->best_effort_timestamp)*timeb;
		timet[n][0] = (int)(timep) /60;
		timet[n][1] = (int)(timep) %60;
		timet[n][2] = (int)(100*(timep-(int)(timep)));
		if (timet[n][2]>=100){
			logging("WARNING: the video is longer than 100m");
		}


		if (pframe->format == AV_PIX_FMT_YUV420P){
			convert_420();}
		else if (pframe->format == AV_PIX_FMT_YUV444P){
			convert_444();
		}
		else if(pframe->format == AV_PIX_FMT_RGBA){;}
		else{
			logging("ERROR format!!");
			return 3;
		}

		return 1;
	}
}

int main(int argc, char const *argv[])
{
//	std::string inputfile = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\Test samples\\8k.mp4";
//	std::string inputfile = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\Test samples\\yuv420.mkv";
	std::string inputfile = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\Test samples\\yuv444.mkv";

	if (argc==2)
	{
		std::string inputfile = argv[1];
	}
	n=0;

	av_register_all();
	avcodec_register_all();

	pFctx = avformat_alloc_context();
	pframe = av_frame_alloc();
	pkt = av_packet_alloc();

	width = 38;
	height = 50;
	channel = 4;
	std::string num_0 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_0.png";
	std::string num_1 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_1.png";
	std::string num_2 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_2.png";
	std::string num_3 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_3.png";
	std::string num_4 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_4.png";
	std::string num_5 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_5.png";
	std::string num_6 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_6.png";
	std::string num_7 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_7.png";
	std::string num_8 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_8.png";
	std::string num_9 = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\num_9.png";
	std::string colon_ = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\colon.png";
	std::string dot_ = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\number\\dot.png";
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	num0 = new uint8_t[4*width*height];
	num1 = new uint8_t[4*width*height];
	num2 = new uint8_t[4*width*height];
	num3 = new uint8_t[4*width*height];
	num4 = new uint8_t[4*width*height];
	num5 = new uint8_t[4*width*height];
	num6 = new uint8_t[4*width*height];
	num7 = new uint8_t[4*width*height];
	num8 = new uint8_t[4*width*height];
	num9 = new uint8_t[4*width*height];
	colon = new uint8_t[4*width*height];
	dot = new uint8_t[4*width*height];

	num0 = stbi_load(num_0.c_str(), &width, &height, &channel, 0);
	num1 = stbi_load(num_1.c_str(), &width, &height, &channel, 0);
	num2 = stbi_load(num_2.c_str(), &width, &height, &channel, 0);
	num3 = stbi_load(num_3.c_str(), &width, &height, &channel, 0);
	num4 = stbi_load(num_4.c_str(), &width, &height, &channel, 0);
	num5 = stbi_load(num_5.c_str(), &width, &height, &channel, 0);
	num6 = stbi_load(num_6.c_str(), &width, &height, &channel, 0);
	num7 = stbi_load(num_7.c_str(), &width, &height, &channel, 0);
	num8 = stbi_load(num_8.c_str(), &width, &height, &channel, 0);
	num9 = stbi_load(num_9.c_str(), &width, &height, &channel, 0);
	colon = stbi_load(colon_.c_str(), &width, &height, &channel, 0);
	dot = stbi_load(dot_.c_str(), &width, &height, &channel, 0);

	if (avformat_open_input(&pFctx, inputfile.c_str(), NULL, NULL) != 0) {
		logging("ERROR could not open the file");
		return -1;
	}

	if (avformat_find_stream_info(pFctx,  NULL) < 0) {
		logging("ERROR could not get the stream info");
		return -1;
	}

	if (open_codec_context(&video_stream_idx, AVMEDIA_TYPE_VIDEO) < 0){
		logging("ERROR could not open codec");
		return -1;
	}

	AVRational timebase = pFctx->streams[video_stream_idx]->time_base;
	int64_t tistall;
	timeb = av_q2d(timebase);
	tistall = double (pFctx->duration/AV_TIME_BASE)/av_q2d(timebase);
	int64_t tist;
	av_read_frame(pFctx,pkt);
	avcodec_send_packet(pCoctx,pkt);
	rgba = new uint8_t[4*pCoctx->width*pCoctx->height];
	combine = new uint8_t[24*pCoctx->width*pCoctx->height];

	for (int i = 0; i < 6; ++i) {
		tist = i*tistall/5;
	if (av_seek_frame(pFctx, video_stream_idx, tist, AVSEEK_FLAG_BACKWARD) < 0){
		logging("ERROR in seeking");
		return -1;
	}
	avcodec_flush_buffers(pCoctx);
	while (av_read_frame(pFctx, pkt ) >= 0){
		if (pkt->stream_index == video_stream_idx){
			//logging("send packet: %ld", pkt->pos);
			int r ;
			r = decode_video(tist);
			av_packet_unref(pkt);
			if (r == 2 || r < 0){;}
			else if (r == 3){
				break;
			}
			else{
				std::string filepath = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\src\\";
				std::string filepng = std::to_string(pframe->best_effort_timestamp);
				std::string filename = inputfile + " frame_" + filepng + ".png";
				stbi_write_png(filename.c_str(),pframe->width,pframe->height,4,rgba,pframe->width*4);
				n++;
				break;
			}
		}
	}

	if (n!=6){
		timep = double(pframe->best_effort_timestamp)*timeb;
		//		logging("time: %.2f", timep);
		timet[n][0] = (int)(timep) /60;
		timet[n][1] = (int)(timep) %60;
		timet[n][2] = (int)(100*(timep-(int)(timep)));
		if (timet[n][2]>=100){
			logging("WARNING: the video is longer than 100m");
		}

		if (pframe->format == AV_PIX_FMT_YUV420P){
			convert_420();}
		else if (pframe->format == AV_PIX_FMT_YUV444P){
			convert_444();
		}
		else if(pframe->format == AV_PIX_FMT_RGBA){;}
		else{
			logging("ERROR format!!");
		}
		std::string filepath = "C:\\Users\\jacky\\Desktop\\dayixia hw\\bian\\project-2-zhuo-che21\\src\\";
		std::string filepng = std::to_string(pframe->best_effort_timestamp);
		std::string filename = inputfile + " frame_" + filepng + ".png";

		stbi_write_png(filename.c_str(),pframe->width,pframe->height,4,rgba,pframe->width*4);


	}
	}
//	logging("ERROR read frame %d", av_read_frame(pFctx, pkt));
	std::string filecombine = inputfile + " combined.png";
	std::string contactsheet = inputfile + " contactshee.png";
	if (pframe->width > 1080 || pframe->height > 720){
		int a = pframe->width / 1080 + 1;
		int b = pframe->height / 720 + 1;
		if (a > b){
			b=a;
		}
		resize = new uint8_t[4*2160*2160];
		stbir_resize_uint8(combine, 2*pframe->width, 3*pframe->height, 0, resize, 2*pframe->width/b, 3*pframe->height/b, 0, 4 );
		stbi_write_png(filecombine.c_str(),pframe->width*2/b,pframe->height*3/b,4,resize,pframe->width*2/b*4);
		int w = pframe->width*2/b*4;
		int h = pframe->height*3/b;
		for (int i = 0; i < 6; ++i) {
			writere(timet[i][0]/10, (i%2)*w/2, (i/2)*h/3, w, h);
			writere(timet[i][0]%10, (i%2)*w/2+width*4, (i/2)*h/3, w, h);
			writere(-1, (i%2)*w/2+2*width*4, (i/2)*h/3, w, h);
			writere(timet[i][1]/10, (i%2)*w/2+3*width*4, (i/2)*h/3, w, h);
			writere(timet[i][1]%10, (i%2)*w/2+4*width*4, (i/2)*h/3, w, h);
			writere(-2, (i%2)*w/2+5*width*4, (i/2)*h/3, w, h);
			writere(timet[i][2]/10, (i%2)*w/2+6*width*4, (i/2)*h/3, w, h);
			writere(timet[i][2]%10, (i%2)*w/2+7*width*4, (i/2)*h/3, w, h);
		}
		stbi_write_png(contactsheet.c_str(),pframe->width*2/b,pframe->height*3/b,4,resize,pframe->width*2/b*4);
		delete []resize;
	}
	else {
		stbi_write_png(filecombine.c_str(), pframe->width * 2, pframe->height * 3, 4, combine, pframe->width * 8);
		int w = pframe->width*2*4;
		int h = pframe->height*3;
		for (int i = 0; i < 6; ++i) {
			//logging(": %d",timet[i][0]/10);
			write(timet[i][0]/10, (i%2)*w/2, (i/2)*h/3, w, h);
			write(timet[i][0]%10, (i%2)*w/2+width*4, (i/2)*h/3, w, h);
			write(-1, (i%2)*w/2+2*width*4, (i/2)*h/3, w, h);
			write(timet[i][1]/10, (i%2)*w/2+3*width*4, (i/2)*h/3, w, h);
			write(timet[i][1]%10, (i%2)*w/2+4*width*4, (i/2)*h/3, w, h);
			write(-2, (i%2)*w/2+5*width*4, (i/2)*h/3, w, h);
			write(timet[i][2]/10, (i%2)*w/2+6*width*4, (i/2)*h/3, w, h);
			write(timet[i][2]%10, (i%2)*w/2+7*width*4, (i/2)*h/3, w, h);
		}
		stbi_write_png(contactsheet.c_str(), pframe->width * 2, pframe->height * 3, 4, combine, pframe->width * 8);
	}

	delete []rgba;
	delete []combine;
	av_frame_free(&pframe);
	//av_frame_free(&newframe);
	av_packet_free(&pkt);
	avcodec_free_context(&pCoctx);
	avformat_close_input(&pFctx);
	avformat_free_context(pFctx);
	return 1;


}

