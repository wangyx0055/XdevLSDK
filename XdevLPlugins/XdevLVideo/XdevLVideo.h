#ifndef DEVICE_VIDEO_H
#define DEVICE_VIDEO_H

#include <XdevLModule.h>

namespace xdl {

class XdevLVideo : public XdevLModule {
public:
	virtual ~XdevLVideo() {}
	virtual int open(const char* file) = 0;
	virtual int close() = 0;
	virtual int getFrameWidth() = 0;
	virtual int getFrameHeight() = 0;
	virtual void* getFrameData() = 0;
	virtual void setFPS(float fps) = 0;
	virtual int update() = 0;
};

}

#endif
