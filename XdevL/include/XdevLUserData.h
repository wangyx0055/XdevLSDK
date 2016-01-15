#ifndef XDEVL_USER_DATA_H
#define XDEVL_USER_DATA_H

#include <XdevLID.h>

namespace xdl {

	/**
		@class XdevLCoreUserDataList
		@brief The iterface for user data.
		@author Cengiz Terzibas
	*/
	class XdevLUserData {
		public:
			XdevLID			id;
			void*				data;
	};

}

#endif // XDEVL_USER_DATA_H
