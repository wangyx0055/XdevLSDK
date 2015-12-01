#include <XdevLEventQueueBuffer.h>

namespace xdl {

    XdevLEventQueueBuffer::XdevLEventQueueBuffer() {
    }

    XdevLEventQueueBuffer::~XdevLEventQueueBuffer() {
      m_thread.interrupt();
    }

    void XdevLEventQueueBuffer::interrupt() {
      m_thread.interrupt();
    }

    void XdevLEventQueueBuffer::join() {
      m_thread.join();
    }

	void XdevLEventQueueBuffer::consume(XdevLEvent& event) {

		eventListenerMap::const_iterator eventListener = m_eventListener.begin();
		while(eventListener != m_eventListener.end() ) {
			(*eventListener)->notify(event);
			eventListener++;
		}
	}

	xdl_int  XdevLEventQueueBuffer::registerListener(XdevLListener* listener) {

		assert(listener && "XdevLCoreImpl::registerEventListener(listener = NULL)");

//		eventListenerMap::const_iterator i = m_eventListener.find(listener->getId()->identifier.hashCode);
//		if(i!=m_eventListener.end()) {
//			std::cerr << "## Listener: '" << listener << "' is registered already." << std::endl;
//			return ERR_WARNING;
//		}
//		m_eventListener.insert(eventListenerMap::value_type(listener->getId()->identifier.hashCode, listener));

		m_eventListener.push_back(listener);
		return ERR_OK;
	}

}
