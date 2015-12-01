#include "XdevLVideoFFmpeg.h"
#include <XdevLPlugin.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <XdevLClassTypes.h>
#include <iostream>
#include <string>

xdl::XdevLModuleFFmpegDescriptorImp xdl::XdevLVideoFFmpeg::m_moduleDesc;
xdl::XdevLFFmpegPluginDescriptor m_audioALPluginDescriptor;

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(std::string(xdl::XdevLVideoFFmpeg::m_moduleDesc.getName()) == parameter->getModuleName()){
		xdl::XdevLModule* obj  = new xdl::XdevLVideoFFmpeg(parameter);
		if (!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);		
		return xdl::ERR_OK;
	}
	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLDevice* obj) {
	if (obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor(){
	return &m_audioALPluginDescriptor;
}


namespace xdl {

XdevLVideoFFmpeg::XdevLVideoFFmpeg(xdl::XdevLModuleCreateParameter* parameter) : 	
		m_id(parameter->getModuleId()),
		m_core(parameter->getMediator()),
		m_formatCtx(NULL),
		m_codecCtx(NULL),
		m_codec(NULL),
		m_frame(NULL),
		m_frameRGB(NULL),
		m_buffer(NULL) ,
		m_updateFrequency(30.0),
		m_connected(false),
		m_initialized(false),
		m_fileOpen(false){
}

XdevLVideoFFmpeg::~XdevLVideoFFmpeg() {}


void XdevLVideoFFmpeg::setFPS(float fps){
	m_updateFrequency = fps;
}

moduleId XdevLVideoFFmpeg::getId() const{
	return m_id;
}

moduleClassId XdevLVideoFFmpeg::getClassId() const{
	return XDEVL_MODULE;
}	

void* XdevLVideoFFmpeg::getInternal(const xdl_char* id) {
	return NULL;
}
XdevLModuleDescriptor* XdevLVideoFFmpeg::getDescriptor() const {
	return &m_moduleDesc;
}	
xdl_int XdevLVideoFFmpeg::init() {
	av_register_all();
	videoStream = -1;
	m_initialized = true;
	return ERR_OK;
}
xdl_int XdevLVideoFFmpeg::shutdown() {
	if (m_buffer)
		av_free(m_buffer);
	if (m_frame)
		av_free(m_frame);
	if (m_frameRGB)
		av_free(m_frameRGB);
	if (m_codecCtx)
		avcodec_close(m_codecCtx);
	if (m_formatCtx)
		av_close_input_file(m_formatCtx);
		
	m_initialized = false;
	m_connected		= false;
	m_fileOpen		= false;
	return ERR_OK;
}

int XdevLVideoFFmpeg::open(const char* filename) {
	// Open video file
	if (av_open_input_file(&m_formatCtx, filename, NULL, 0, NULL)!=0) {
		std::cerr << "Could not open video file: " << filename << std::endl;
		return xdl::ERR_ERROR;
	}

	// Retrieve stream information
	if (av_find_stream_info(m_formatCtx)<0)
		return xdl::ERR_ERROR;

	// Dump information about file onto standard error
	dump_format(m_formatCtx, 0, filename, 0);

	for (unsigned int i=0; i<m_formatCtx->nb_streams; i++)
		if (m_formatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO) {
			videoStream=i;
			break;
		}

	if (videoStream==-1) {
		std::cerr << "Videostream not found.\n";
		return xdl::ERR_ERROR;
	}

	// Get a pointer to the codec context for the video stream
	m_codecCtx=m_formatCtx->streams[videoStream]->codec;
	// Find the decoder for the video stream
	m_codec=avcodec_find_decoder(m_codecCtx->codec_id);
	if (m_codec==NULL) {
		std::cerr << "Unsupported codec!\n";
		return xdl::ERR_ERROR; // Codec not found
	}

	// Open codec
	if (avcodec_open(m_codecCtx, m_codec)<0) {
		std::cerr << "Could not open coded.\n";
		return xdl::ERR_ERROR;
	}


	// Allocate video frame
	m_frame=avcodec_alloc_frame();
	m_frameRGB=avcodec_alloc_frame();
	if (m_frameRGB==NULL) {
		std::cerr << "Could not allocate codec frame memory.\n";
		return xdl::ERR_ERROR;
	}

	int numBytes;
	// Determine required buffer size and allocate buffer
	numBytes=avpicture_get_size(PIX_FMT_RGB24, m_codecCtx->width, m_codecCtx->height);
	m_buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in m_frame
	// Note that m_frame is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)m_frameRGB, m_buffer, PIX_FMT_RGB24, m_codecCtx->width, m_codecCtx->height);

	// Create image scale context to convert image into different formats.
	m_img_convert_ctx = sws_getContext(	m_codecCtx->width, m_codecCtx->height, 
																			m_codecCtx->pix_fmt, 
																			m_codecCtx->width, m_codecCtx->height,
																			PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

	m_fileOpen = true;
	m_timer.reset();
	return xdl::ERR_OK;
}

int XdevLVideoFFmpeg::update() {

	if(!m_fileOpen)
		return ERR_ERROR;

	m_timer.update();
		if(m_timer.getTime() >= (1.0/m_updateFrequency)){
//	if (true) {
		int frameFinished;
		AVPacket packet;

		av_read_frame(m_formatCtx, &packet);
		// Is this a packet from the video stream?
		if (packet.stream_index==videoStream) {
			// Decode video frame
			avcodec_decode_video(m_codecCtx, m_frame, &frameFinished, packet.data, packet.size);

			// Did we get a video frame?
			if (frameFinished) {
				// Convert the image from its native format to RGB
//				img_convert((AVPicture *)m_frameRGB, PIX_FMT_RGB24, (AVPicture*)m_frame, m_codecCtx->pix_fmt, m_codecCtx->width, m_codecCtx->height);
				sws_scale(m_img_convert_ctx, m_frame->data, m_frame->linesize, 0, m_codecCtx->height, m_frameRGB->data, m_frameRGB->linesize);
				// Save the frame to disk
				//				saveFrame(m_frameRGB, m_codecCtx->width, m_codecCtx->height, 0);
			}
		}
		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
		m_timer.reset();
	}
	return xdl::ERR_OK;
}

int XdevLVideoFFmpeg::close() {
	// Close the video file
	av_close_input_file(m_formatCtx);
	m_fileOpen = false;
	return ERR_OK;
}
int XdevLVideoFFmpeg::getFrameWidth() {
	if (m_codecCtx)
		return m_codecCtx->width;
	return 0;
}
int XdevLVideoFFmpeg::getFrameHeight() {
	if (m_codecCtx)
		return m_codecCtx->height;
	return 0;
}
void* XdevLVideoFFmpeg::getFrameData() {
	return m_frameRGB->data[0];
}

void XdevLVideoFFmpeg::saveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
	FILE *pFile;
	char szFilename[32];
	int  y;

	// Open file
	sprintf(szFilename, "frame%d.ppm", iFrame);
	pFile=fopen(szFilename, "wb");
	if (pFile==NULL)
		return;

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	// Write pixel data
	for (y=0; y<height; y++)
		fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width*3, pFile);

	// Close file
	fclose(pFile);
}


}
