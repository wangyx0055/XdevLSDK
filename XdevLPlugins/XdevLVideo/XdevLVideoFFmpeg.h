#ifndef DEVICE_VIDEO_FFmpeg_H
#define DEVICE_VIDEO_FFmpeg_H

#include <XdevLPlatform.h>
#include <XdevLListener.h>
#include <XdevLModule.h>
#include <XdevLPlugin.h>
#include <XdevLTimer.h>
#include "XdevLVideo.h"

#ifdef _WIN32
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#else
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
#include <ffmpeg/swscale.h>
#endif

namespace xdl {


class XdevLFFmpegPluginDescriptor : public XdevLPluginDescriptor {
public:
	virtual ~XdevLFFmpegPluginDescriptor() {}
	virtual const xdl_char* getName() const 	{ static xdl_char name[] 		= "XdevLVideo";return name;}
	virtual const xdl_char* getModules() const {static xdl_char moduleName[] = "FFmpeg"; return moduleName;}
	virtual void getVersion(xdl_int* major, xdl_int* minor, xdl_int* patch) { *major = 0; *minor = 1; *patch = 0;}
};
	
class XdevLModuleFFmpegDescriptorImp : public XdevLModuleDescriptor {
public:
	virtual ~XdevLModuleFFmpegDescriptorImp() {}
	virtual const xdl_char* getVendor() const { static xdl_char vendor[] 	= "www.codeposer.net"; return vendor;}
	virtual const xdl_char* getAuthor() const { static xdl_char author[] 	= "Cengiz Terzibas"; 																return author;}
	virtual const xdl_char* getName() const 	{ static xdl_char name[] 		= "FFmpeg"; 																				return name;}
	virtual const xdl_char* getCopyright() const { static xdl_char copyright[] = "(c) 2007 Cengiz Terzibas"; return copyright;}
	virtual const xdl_char* getDescription() const {static xdl_char description[] = "FFmpeg video playback XdevLModule"; return description;}			
	virtual xdl_int getMajor() 								{ static xdl_int major 			= 0; 																								return major;}
	virtual xdl_int getMinor() 								{ static xdl_int minor 			= 1; 																								return minor;}
	virtual xdl_int getPatch() 								{ static xdl_int patch 			= 0; 																								return patch;}
};
	
class XdevLVideoFFmpeg : public XdevLVideo {
public:
	XdevLVideoFFmpeg(xdl::XdevLModuleCreateParameter* parameter);
	virtual ~XdevLVideoFFmpeg();
	static XdevLModuleFFmpegDescriptorImp m_moduleDesc;	
	
	virtual moduleId getId() const;
	virtual moduleClassId getClassId() const;	
	virtual xdl_int init();
	virtual xdl_int shutdown();
	virtual XdevLModuleDescriptor* getDescriptor() const;
	virtual void* getInternal(const xdl_char* id);	
	virtual xdl_int update();
	
	
	int open(const char* filename);
	int close();
	int getFrameWidth();
	int getFrameHeight();
	void* getFrameData();
	virtual void setFPS(float fps);	
protected:
	void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);
protected:
	XdevLCoreMediator* m_core;
	moduleId m_id;

	AVFormatContext* 	m_formatCtx;
	AVCodecContext* 	m_codecCtx;
	AVCodec*					m_codec;
	AVFrame*					m_frame;
	AVFrame*					m_frameRGB;
	SwsContext*				m_img_convert_ctx;	
	uint8_t*					m_buffer;

	int videoStream;
	xdl::XdevLTimer				m_timer;
	double						m_updateFrequency;
	
	bool m_connected;
	bool m_initialized;
	bool m_fileOpen;

};

}

#endif
