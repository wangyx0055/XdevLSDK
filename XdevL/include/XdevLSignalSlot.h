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

#ifndef XDEVL_SIGNAL_SLOT_H
#define XDEVL_SIGNAL_SLOT_H

#include <iostream>
#include <vector>
#include <exception>
#include <Signal.h>

template<typename T>
class Observable;

template<typename T>
class ObservableValue;

template<typename T>
class InvalidationListener;

template<typename T>
class ChangeListener;

template<typename T>
class Property;


template<typename T>
class InvalidationListener {
	public:
		virtual void invalidated(Observable<T>& observable) {};
};

template<typename T>
class ChangeListener {
	public:
		virtual void changed(const ObservableValue<T>& old_value, const ObservableValue<T>& new_value) {};
};


template<typename T>
class Observable {
	public:
		// Add new listener.
		void addListener(InvalidationListener<T>* listener) {
			this->invalidationListeners.push_back(listener);
		}

		// Remove the listener.
		void removeListener(InvalidationListener<T>* listener) {
			this->invalidationListeners.erease(listener);
		}

	protected:
		std::vector<InvalidationListener<T>*> invalidationListeners;
};


template<typename T>
class ObservableValue : public Observable<T> {
	public:
		friend class Property<T>;

		ObservableValue() : value(NULL), bindObservable(NULL) {
			value = new T();
		}

		ObservableValue(const T& v) : bindObservable(NULL) {
			value = new T();
			*value = v;
		}

		~ObservableValue() {
			delete value;
		}

		T& getValue() {
			if(bindObservable) {
				return bindObservable->getValue();
			}
			return *value;
		}

		const T& getValue() const {
			if(bindObservable) {
				return bindObservable->getValue();
			}
			return *value;
		}

		void addListener(ChangeListener<T>* listener) {
			changeListeners.push_back(listener);
		}

		void removeListener(ChangeListener<T>* listener) {
			changeListeners.erease(listener);
		}

	protected:
		T* value;
		std::vector<ChangeListener<T>*> changeListeners;
		ObservableValue<T>* bindObservable;
};



template<typename T>
class Property : public ObservableValue<T> {
	public:
		Property() {}
		Property(const T& value) : ObservableValue<T>(value), tmp_value(NULL) {}
		~Property() {
			if(isBound() && (tmp_value != NULL)) {
				unbindBidirectional();
			} else if(isBound() && (tmp_value == NULL)) {
				unbind();
			}
		}

		void bind(ObservableValue<T>* observableValue) {
			this->bindObservable = observableValue;

		}

		void unbind() {
			this->bindObservable = NULL;
		}

		bool isBound() {
			if(this->bindObservable) {
				return true;
			}
			return false;
		}

		void bindBidirectional(ObservableValue<T>* observableValue) {
			this->tmp_value = this->value;
			this->value = observableValue->value;
			this->bind(observableValue);
		}

		void unbindBidirectional() {
			this->value = this->tmp_value;
			this->tmp_value = NULL;
		}

		void setValue(const T& value) {

			// If the Property is bound we are not allowed to change the value.
			if(isBound() && (tmp_value == NULL)) {
				throw new std::exception();
			}

			// Do all listener stuff.
			typename std::vector< InvalidationListener<T>* >::iterator itil(this->invalidationListeners.begin());
			while(itil != this->invalidationListeners.end()) {
				(*itil)->invalidated(*this);
				itil++;
			}

			typename std::vector< ChangeListener<T>* >::iterator it(this->changeListeners.begin());
			while(it != this->changeListeners.end()) {
				(*it)->changed(*this->value, value);
				it++;
			}

			*this->value = value;
		}

	private:
		T* tmp_value;
};

#endif
