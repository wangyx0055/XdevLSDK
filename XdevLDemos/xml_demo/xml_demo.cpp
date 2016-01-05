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

#include <XdevL.h>
#include <XdevLXstring.h>
#include <tinyxml.h>
#include <vector>

XDEVL_DECLARE_LOG(MyLog);
XDEVL_DEFINE_LOG(MyLog);

void defaultConsumerCallback(const xdl::XdevLLogItem& item) {
		// Ignore user specified data defined in item.userData
		std::cout << "Message: " << item.message << std::endl ;
	}
	

int main(int argc, char** argv) {
	
	XDEVL_LOG_REGISTER_LISTENER(MyLog, defaultConsumerCallback);
	
	// Now we need the tinyxml xml document holder which is going to parse our XML file.
	TiXmlDocument doc;
	
	// Now we parse the xml file.
	if(!doc.LoadFile("xml_demo.xml")) {
		return -1;
	}

	// This TiXmlHandle class is pretty useful to get really fast information if an element exists or not.
	TiXmlHandle docHandle(&doc);
	
	// We are looking for the MyElement element in the xml file.
	TiXmlElement* element = docHandle.FirstChild("XdevLCoreProperties").FirstChild("MyElement").ToElement();
	if(element == nullptr) {
		return -1;
	}

	// Ok we have it, now let's check if we can find the "myattribute" attribute in this element.
	if(element->Attribute("myattribute")) {
		// Yes, good than convert the string into a integer value.
		xdl::xdl_int value = xstd::from_string<xdl::xdl_int>(element->Attribute("myattribute"));
		XDEVL_LOG_INFO(MyLog, STRING("The value of 'myattribute' is: %d"), value);
	}

	//
	// Reading text field of an xml element with tinyxml methods.
	//
	TiXmlElement* element2 = docHandle.FirstChild("XdevLCoreProperties").FirstChild("MyNextElement").ToElement();
	if(element2 == nullptr) {
		return -1;
	}

	if(element2->GetText()) {
		XDEVL_LOG_INFO(MyLog, STRING("%s"), element2->GetText() );
	}

	//
	// You can use the TiXmlFindElement method to get easier access to xml elements.
	//
	TiXmlElement* node = xdl::TiXmlFindElement(&doc, "XdevLCoreProperties/MyElement");
	if(node) {
		XDEVL_LOG_INFO(MyLog, STRING("Using  xdl::TiXmlFindElement to find myattribute = %s"), node->Attribute("myattribute") );
	}
	else {
		XDEVL_LOG_INFO(MyLog, STRING("Could not found specified element.\n ") );
	}

	return 0;
}
