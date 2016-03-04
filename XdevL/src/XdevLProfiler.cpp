#include <XdevLProfiler.h>

namespace xdl {

	XdevLProfiler* XdevLProfiler::instance = nullptr;

	XdevLProfiler::XdevLProfiler() {
		
	}
	
	XdevLProfiler* XdevLProfiler::getInstance() {
		if(XdevLProfiler::instance == nullptr) {
			XdevLProfiler::instance = new XdevLProfiler();
			return XdevLProfiler::instance;
		}
		return XdevLProfiler::instance;
	}

	void XdevLProfiler::add(const XdevLString& scopeName, xdl_uint64 timestamp) {
		scopes[scopeName] = timestamp;
	}

	void XdevLProfiler::dump() {
		for(auto scope : scopes) {
			std::cout << scope.first << " = " << scope.second << std::endl;
		}
	}


	XdevLProfilerScope::XdevLProfilerScope(const xdl::XdevLString& name) :
		scopeName(name) {
		timer.reset();
	}

	XdevLProfilerScope::~XdevLProfilerScope() {
		XdevLProfiler::getInstance()->add(scopeName, timer.getTime64());
	}

}
