#include <XdevLProfiler.h>

namespace xdl {


	std::map<xdl::XdevLString, xdl::xdl_uint64> XdevLProfiler::scopes;

	void XdevLProfiler::dump() {
		for(auto scope : scopes) {
			std::cout << scope.first << " = " << scope.second << std::endl;
		}
	}

}
