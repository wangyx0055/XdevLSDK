#include <XdevLConditionalVariable.h>
#include <iostream>
#include <errno.h>
#include <string.h>

	namespace thread {

		ConditionalVariable::ConditionalVariable() {
#ifdef XDEVL_PLATFORM_UNIX
			if ( pthread_cond_init(&m_condition, NULL) != 0 ) {
				throw("Mutex::Could not create mutex");
			}
#elif defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
			InitializeConditionVariable(&m_condition);
#else
			#error "Not implemented for this platform"
#endif
		}

		ConditionalVariable::~ConditionalVariable() {

		}

		int ConditionalVariable::signal() {
#ifdef XDEVL_PLATFORM_UNIX
			if (pthread_cond_signal(&m_condition) != 0) {
				std::cerr << "ConditionalVariable::Could not send signal: " << strerror(errno) << std::endl;
				return 1;
			}
#elif defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
			WakeConditionVariable(&m_condition);
#else
			#error "Not implemented for this platform"
#endif
			return 0;
		}

		int ConditionalVariable::brodcast() {
#if XDEVL_PLATFORM_UNIX
			if ( pthread_cond_broadcast(&m_condition) != 0) {
				std::cerr << "ConditionalVariable::Could send signal to all: " << strerror(errno) << std::endl;
				return 1;
			}
#elif defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
			WakeAllConditionVariable(&m_condition);
#else
			#error "Not implemented for this platform"
#endif
			return 0;
		}

		int ConditionalVariable::wait(Mutex& mutex) {
#if XDEVL_PLATFORM_UNIX
			if (pthread_cond_wait(&m_condition, &mutex.getNativeMutex()) != 0) {
				std::cerr << "ConditionalVariable::Could wait: " << strerror(errno) << std::endl;
				return 1;
			}
#elif defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
			WakeAllConditionVariable(&m_condition);
#else
			#error "Not implemented for this platform"
#endif
			return 0;
		}

	
}
