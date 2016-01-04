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

#ifndef XDEVL_MANAGED_BUFFER_H
#define XDEVL_MANAGED_BUFFER_H

#include <XdevLThread.h>
#include <XdevLMutex.h>
#include <XdevLConditionalVariable.h>

#include <queue>
#include <list>

namespace xdl {

	/**
	  @class XdevLManagedBuffer
	  @brief A buffer that manages messages.
	  @author Cengiz Terzibas

	  This buffer is a self managed buffer. The main purpose of this class
	  is to manage incomming items.
		Let's say we have incomming packages from the UDP socket. The first thing
		we have to do is get it out of the buffer as fast as possible to prevent
		dropouts. We can use this buffer to do that.

	*/
	template<typename T>
	class XdevLManagedBuffer : public thread::Thread {
		public:


			XdevLManagedBuffer() {
				Start();
			};

			~XdevLManagedBuffer() {

			};



			/// Return if the message buffer is empty or not.
			/**
			 * @return 'true' if the buffer is empty else 'false'.
			 */
			bool empty() {
				thread::XdevLScopeLock lock(this->m_mutex_items);

				bool ret = this->m_items.empty();

				return ret;
			}

			/// Add a new message into the queue.
			/**
			 *
			 * Event though we pass the reference of an object this method
			 * is save because the std::queue will make a copy of that object.
			 *
			 * @param msg The message to add into the buffer.
			 */
			void add(T& msg) {

				thread::XdevLScopeLock lock(this->m_temp_items_mutex);

				this->m_queue.push(msg);

				lock.Unlock();

				this->m_temp_condition.signal();



			}

			/// Get a message from the tail.
			/**
			 *
			 * The poped message will be removed from the buffer.
			 *
			 * @return The T message.
			 */
			T& pop_front() {
				thread::XdevLScopeLock lock(this->m_mutex_items);

				// TODO I have to investigate if it is save to return the reference
				// of the message.
				T& tmp = this->m_items.front();
				this->m_items.pop_front();

				return tmp;
			}

			/// Get a message from the head.
			/**
			 *
			 * The poped message will be removed from the buffer.
			 *
			 * @return The T message.
			 */
			T& pop_back() {
				thread::XdevLScopeLock lock(this->m_mutex_items);

				T& tmp = this->m_items.back();
				this->m_items.pop_back();

				return tmp;
			}

			/// Consumes the queued items.
			/**
				Override this method to consume the items that are arrvied in the buffer.
			*/
			virtual void consume(T& itm) = 0;


		private:
			virtual int RunThread(thread::ThreadArgument* arg) {

				while(true) {


					thread::XdevLScopeLock lock(this->m_temp_items_mutex);

					// Wait until new messages arrive. Until then the m_temp_messages_mutex will
					// be not locked. After receiving signal it will lock the mutex.
					while(this->m_queue.empty()) {
						this->m_temp_condition.wait(lock);
					}

					// Put everything into the item list. Why do we have this? We want to empty the queue as
					// fast as possible.
					while(!this->m_queue.empty()) {
						this->m_items.push_back(this->m_queue.front());
						this->m_queue.pop();
					}

					// Now we are finished with reading from that buffer. Let's unlock it.
					lock.Unlock();

					// Let's go trough all arrived items and consume it.
					typename std::list<T>::iterator itm(this->m_items.begin());
					while(itm != this->m_items.end()) {

						// Consume arrived item.
						// TODO This consume method may block this operation.
						this->consume(*itm);

						// Next message ...
						itm++;
					}

					// We did all the items. Let's clear it for the next round.
					this->m_items.clear();


				}
			}

		protected:

			// Holds the temporary messages.
			std::queue<T> 								m_queue;

			// Holds the messages.
			std::list<T> 									m_items;

			// The conditionals variable for received messages.
			thread::ConditionalVariable		m_temp_condition;

			// Mutex for the temporary item list.
			thread::Mutex									m_temp_items_mutex;

			// Mutex for the item list.
			thread::Mutex									m_mutex_items;

	};

}

#endif
