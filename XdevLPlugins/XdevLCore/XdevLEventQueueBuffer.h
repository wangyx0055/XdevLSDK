#ifndef XDEVL_EVENT_QUEUE_BUFFER_H
#define XDEVL_EVENT_QUEUE_BUFFER_H

#include <XdevLEvent.h>
#include <XdevLListener.h>
#include <XdevLManagedBuffer.h>



namespace xdl {

//	typedef std::map<xdl_uint64, XdevLEventListener*>	eventListenerMap;
	typedef std::vector<XdevLListener*>	eventListenerMap;

	/**
		@class XdevLEventQueueBuffer
		@brief Handle all sorts of events in the Core system.
	*/
	class XdevLEventQueueBuffer : public XdevLManagedBuffer<XdevLEvent> {
	public:
		XdevLEventQueueBuffer();
		virtual ~XdevLEventQueueBuffer();
		virtual xdl_int registerListener(XdevLListener* listener);
		void interrupt();
		void join();
	private:
		void consume(XdevLEvent& event);
	private:
		eventListenerMap m_eventListener;
	};

}

#endif
