/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2016 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef XDEVL_DELEGATE_H
#define XDEVL_DELEGATE_H

namespace xdl {

	/**
		@brief A template that supports delegates.

		Delegates helps you to develop more functional rather then object oriented API's. No virtual functions and object passing to
		call method of an object are needed. This is reducing dependency between classes. You just pass the delegate
		object to trigger either the globally definied function or a class member function.

		@section usage Usage

		@code

		xdl_float func(xdl_int value, xdl_float anotherValue) {
			// Do something here.
			xdl_float value = ...
			...
			return value.
		}

		class MyClass {
			public:
			void func(xdl_int value) {
				// Do something here.
			}
		};
		MyClass MyClassInstance;

		// Create a delegate for a globally defined function.
		auto d1 = xdl::XdevLDelegate<xdl_float, xdl_int, xdl_float>::Create<&func>();

		// Create a delegate for a class member.
		auto d2 = xdl::XdvLDelegate<void, xdl_int>::Create<MyClass, &MyClass::func>(&MyClassInstance);


		// Using the delegates.
		d1(10);

		d2(20);

		@endcode
	*/
	template <typename ReturnType, typename... Parameters>
	class XdevLDelegate {
			typedef ReturnType(*CallbackFunctionType)(void* callee, Parameters...);

		public:
			XdevLDelegate() :  m_callee(nullptr), m_callbackFunction(nullptr) {}
			XdevLDelegate(CallbackFunctionType function) : m_callee(nullptr), m_callbackFunction(function) {}
			XdevLDelegate(void* callee, CallbackFunctionType function) : m_callee(callee), m_callbackFunction(function) {}

			/// Creates a delegate for a class member function.
			/**
				Use this method to create a delegate for a class member function. See @ref usage.

				@tparam ReturnType The return type of the member function.
				@tparam Parameters The argument parameter types of the member function.
				@param callee The instance of the class for this memember function. This is usually used when using class members. Here you specify the instance
				of that class.
				@return The delegate for the class member function.
			*/
			template <typename T, ReturnType(T::*TMethod)(Parameters...)>
			static XdevLDelegate Create(T* callee) {
				return XdevLDelegate(callee, &methodCaller<T, TMethod>);
			}

			/// Creates a delegate for a globally defined function.
			/**
				Use this method to create a delegate for a globally defined  function .See @ref usage.

				@tparam ReturnType The return type of the function.
				@tparam Parameters The argument parameter types of the function.
				@return The delegate for the globally defined function.
			*/
			template <ReturnType(*TMethod)(Parameters...)>
			static XdevLDelegate Create() {
				return XdevLDelegate(&methodCaller2<TMethod>);
			}

			/// Executes the delegate.
			/**
				Use this method to execute a delegate.See @ref usage.

				@tparam ReturnType The return type of the member/function specified in the create method.
				@tparam Parameters The argument parameter types of the member/function.
				@return The return value of the member/function.
			*/
			ReturnType operator()(Parameters... pm) const {
				assert(this->m_callbackFunction && "No callback function assigned to Delegate.");
				return this->m_callbackFunction(m_callee, pm...);
			}

			/// Compares two delegates.
			bool operator==(const XdevLDelegate& other) const {
				return (m_callee == other.m_callee) && (m_callbackFunction == other.m_callbackFunction);
			}

			// Returns if the callback function is valid.
			xdl_bool isValid() const {
				return (m_callbackFunction != nullptr);
			}

		private:

			// Holds the callee object.
			void* m_callee;

			// Holds teh call
			CallbackFunctionType m_callbackFunction;

			template <typename T, ReturnType(T::*TMethod)(Parameters...)>
			static ReturnType methodCaller(void* callee, Parameters... pm) {
				return (static_cast<T*>(callee)->*TMethod)(pm...);
			}

			template <ReturnType(*TMethod)(Parameters...)>
			static ReturnType methodCaller2(void* callee, Parameters... pm) {
				return TMethod(pm...);
			}
	};

}

#endif // XDEVL_DELEGATE_H
