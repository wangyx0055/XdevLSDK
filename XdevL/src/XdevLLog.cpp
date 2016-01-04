/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#include "XdevLLog.h"
#include <algorithm>

namespace xdl {

	static void defaultConsumerCallback(const xdl::XdevLLogItem& item) {
		// Ignore user specified data defined in item.userData
		std::cout << "Time [us]: " << item.timestamp << ": " << item.additional << " Message: " << item.message << std::endl ;
	}
	
	XdevLLog* XdevLLog::instance = nullptr;
	std::queue<XdevLLogItem>	XdevLLog::m_messageQueue;
	thread::Mutex 							XdevLLog::m_mutex;
	XdevLLog::ConsumerCallbackListType	XdevLLog::m_callbackFunctions;
	XdevLTimer 								XdevLLog::m_timer;
	xdl_bool										XdevLLog::m_threadIsRunning;
	thread::Mutex							XdevLLog::m_threasIsRunningMutex;


	XdevLLog::XdevLLog()  : CustomCallBack(defaultConsumerCallback) {
	
	}

	XdevLLog::~XdevLLog() {

	}
	
	void XdevLLog::log(const XdevLLogLevel& level, void* logger, const xdl_char* str, ...) {
		va_list args;
		va_start(args, str);
		parse(level, logger, str, args);
		va_end(args);
	}

//	void XdevLLog::log(const XdevLLogLevel& level, void* userData, const xdl_char* str, ...) {
//		va_list args;
//		va_start(args, str);
//		parse(level, userData, str, args);
//		va_end(args);
//	}
	
	void XdevLLog::parse(const XdevLLogLevel& level, void* logger, const xdl_char* str, va_list args) {
		thread::XdevLScopeLock lock(m_mutex);
		
		char buffer[1024] = {0};
		vsnprintf(buffer, 1024, str, args);
		
		XdevLLogItem item(level, XdevLString(buffer));
		item.timestamp = m_timer.getTime64();
		m_messageQueue.push(item);
		
		XdevLLog* log = static_cast<XdevLLog*>(logger);
		log->CustomCallBack(item);
	}

//	void XdevLLog::parse(const XdevLLogLevel& level, void* userData, const xdl_char* str, va_list args) {
//		thread::XdevLScopeLock lock(m_mutex);
//		
//		char buffer[1024] = {0};
//		vsnprintf(buffer, 1024, str, args);
//		
//		XdevLLogItem item(level, XdevLString(buffer));
//		item.timestamp = m_timer.getTime64();
//		item.userData = userData;
//		m_messageQueue.push(item);
//	}

	void XdevLLog::log(const XdevLLogItem& item) {
		thread::XdevLScopeLock lock(m_mutex);

		XdevLLogItem it = item;
		it.timestamp = m_timer.getTime64();
		m_messageQueue.push(it);
	}

	void XdevLLog::registerConsumer(void* logger, XdevLLogConsumerCallbackFunction callback) {
		thread::XdevLScopeLock lock(m_mutex);

		if(callback == nullptr) {
			return;
		}

		// Only add the new callback if it not already in the list.
		auto it = std::find(m_callbackFunctions.begin(), m_callbackFunctions.end(), callback);
		if(it == m_callbackFunctions.end()) {
			m_callbackFunctions.push_back(callback);
		}
		
		XdevLLog* log = static_cast<XdevLLog*>(logger);
		log->CustomCallBack = callback;
	}

	void XdevLLog::unregisterConsumer(void* logger, XdevLLogConsumerCallbackFunction callback) {
		thread::XdevLScopeLock lock(m_mutex);

		if(callback == nullptr) {
			return;
		}

		// Remove if the callback exists in the list.
		auto it = std::find(m_callbackFunctions.begin(), m_callbackFunctions.end(), callback);
		if(it != m_callbackFunctions.end()) {
			m_callbackFunctions.erase(it);
		}

		XdevLLog* log = static_cast<XdevLLog*>(logger);
		log->CustomCallBack = callback;
	}

	int XdevLLog::RunThread(thread::ThreadArgument* arg) {

		while(isThreadRunning()) {
			consumeEvents();
		}
		
		return 0;

	}

	void  XdevLLog::consumeEvents() {
		thread::XdevLScopeLock lock(m_mutex);
		
		// Iterate through all callbacks. 
		for(const auto& ib : m_callbackFunctions) {
			while(!m_messageQueue.empty()) {
				XdevLLogItem& item = m_messageQueue.back();
				(*ib)(item);
				m_messageQueue.pop();
			}
		}
	}

	void XdevLLog::startThread() {
		thread::XdevLScopeLock lock(m_threasIsRunningMutex);
		m_threadIsRunning = xdl_true;
		Start();
	}

	void XdevLLog::stopThread() {
		thread::XdevLScopeLock lock(m_threasIsRunningMutex);
		m_threadIsRunning = xdl_false;
		Join();
	}

	xdl_bool XdevLLog::isThreadRunning() {
		thread::XdevLScopeLock lock(m_threasIsRunningMutex);
		return m_threadIsRunning;
	}

	XdevLLog* XdevLLog::getInstance() {
		if(XdevLLog::instance == NULL) {
			XdevLLog::instance = new XdevLLog();
		}
		return XdevLLog::instance;
	}

	void XdevLLog::destroyInstance() {
		if(XdevLLog::instance != NULL) {
			delete XdevLLog::instance;
			XdevLLog::instance = NULL;
		}
	}


}
