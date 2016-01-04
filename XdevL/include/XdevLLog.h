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
#ifndef XDEVL_LOG_H
#define XDEVL_LOG_H

#include <XdevLCoreMediator.h>
#include <XdevLTypes.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>
#include <XdevLTimer.h>
#include <queue>
#include <list>
#include <cstdarg>
#include <stdio.h>

namespace xdl {


	class XdevLLogItem;
	class XdevLLog;

	typedef void (*XdevLLogConsumerCallbackFunction)(const XdevLLogItem& item);

	enum XdevLLogLevel {
	    XDEVL_LOG_INFO,
	    XDEVL_LOG_WARNING,
	    XDEVL_LOG_ERROR,
	    XDEVL_LOG_FATAL
	};

	/**
		@class XdevLLogItem
		@brief A logging item
	*/
	class XdevLLogItem {
		public:
			XdevLLogItem(XdevLLogLevel _level, const XdevLString& _message,  const XdevLString& _detail = XdevLString(), const XdevLString& _additional = XdevLString()) :
				level(_level),
				message(_message),
				detail(_detail),
				additional(_additional),
				userData(nullptr) {
			}

			/// Time stamp of the log.
			xdl_uint64		timestamp;

			/// The level of the log.
			XdevLLogLevel	level;

			/// The logging message.
			XdevLString 	message;

			/// Detail of the log.
			XdevLString 	detail;

			/// Some more additional information.
			XdevLString 	additional;

			void*			userData;
	};

	/**
		@class XdevLLog
		@brief A cool and boring logger :D.
	*/
	class XdevLLog : public thread::Thread {
		public:
			typedef std::list<XdevLLogConsumerCallbackFunction> ConsumerCallbackListType;
			XdevLLog();

			~XdevLLog();

			/// Returns a valid instance of the logger.
			static XdevLLog* getInstance();

			/// Destroys the logger object.
			static void destroyInstance();

			/// Logs a message.
			static void log(const XdevLLogLevel& level, void* logger, const xdl_char* fmt, ...);

			/// Logs a message.
	//		static void log(const XdevLLogLevel& level, void* user, const xdl_char* fmt, ...);

			/// Logs a message.
			static void log(const XdevLLogItem& item);

			/// Registers a consumer callback function.
			static void registerConsumer(void* logger, XdevLLogConsumerCallbackFunction callback);

			/// Unregister a consumer callback function.
			static void unregisterConsumer(void* logger, XdevLLogConsumerCallbackFunction callback);

			XdevLLog* operator()() {
				return instance;
			}

			XdevLLogConsumerCallbackFunction CustomCallBack;
			
		private:

			static void parse(const XdevLLogLevel& level, void* logger, const xdl_char* str, va_list args);

	//		static void parse(const XdevLLogLevel& level, void* userData, const xdl_char* str, va_list args);

			virtual int RunThread(thread::ThreadArgument* p_arg);

			void consumeEvents();

			void startThread();

			void stopThread();

			xdl_bool isThreadRunning();

		private:

			// Queue for all messages.
			static std::queue<XdevLLogItem>	m_messageQueue;

			// Mutex to sync incoming messages.
			static thread::Mutex 							m_mutex;

			// Holds the consumer callbacks
			static ConsumerCallbackListType	m_callbackFunctions;

			// Global timer.
			static XdevLTimer 								m_timer;

			static xdl_bool										m_threadIsRunning;

			static thread::Mutex							m_threasIsRunningMutex;
		

		public:

			static XdevLLog* instance;

	};

#define XDEVL_DECLARE_LOG(DeclarationName) class XdevLLog##DeclarationName : public xdl::XdevLLog { }
#define XDEVL_DEFINE_LOG(DeclarationName) XdevLLog##DeclarationName DeclarationName

#define XDEVL_LOG(DeclarationName, LEVEL_MESSAGE, ...) XdevLLog##DeclarationName::log(LEVEL_MESSAGE,  ##__VA_ARGS__)

#define XDEVL_LOG_REGISTER_LISTENER(DeclarationName, LISTENER_CALLBACK) { \
		XdevLLog##DeclarationName::registerConsumer(&DeclarationName, LISTENER_CALLBACK) ; \
	}

#define XDEVL_LOGU(LEVEL, MESSAGE, USER, ...)	{ \
		XdevLLog##DeclarationName::log(LEVEL, MESSAGE,  USER, ##__VA_ARGS__); \
	}

#define XDEVL_LOG_INFO(DeclarationName, MESSAGE, ...)	{ \
		XdevLLog##DeclarationName::log(xdl::XDEVL_LOG_INFO, &DeclarationName, MESSAGE, ##__VA_ARGS__); \
	}

#define XDEVL_LOG_WARNING(MESSAGE, ...)	{ \
		XdevLLog##DeclarationName::log(xdl::XDEVL_LOG_WARNING, MESSAGE, ##__VA_ARGS__); \
	}

#define XDEVL_LOG_ERROR(MESSAGE, ...)	{ \
		XdevLLog##DeclarationName::log(xdl::XDEVL_LOG_ERROR, MESSAGE, ##__VA_ARGS__); \
	}

#define XDEVL_LOG_FATAL(MESSAGE, ...)	{ \
		XdevLLog##DeclarationName::log(xdl::XDEVL_LOG_FATAL, MESSAGE, ##__VA_ARGS__); \
	}
}

#endif // XDEVL_LOG_H
