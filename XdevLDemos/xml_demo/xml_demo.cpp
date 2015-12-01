/*
	Application: xml_demo.cpp
	Author		: Cengiz Terzibas
	Brief			: Demonstrates how to use tinyxml to parse xml files and how to use the logger.
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
