#include <XdevL.h>
#include <XdevLComputeDevice/XdevLComputeDevice.h>

float data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(int argc, char** argv) {
	xdl::IPXdevLCore core = xdl::createCore();
	core->plug(xdl::XdevLPluginName("XdevLComputeDeviceCL"), xdl::XdevLVersion(0,1,0));

	auto computeDevice = xdl::createModule<xdl::IPXdevLComputeDevice>(core, xdl::XdevLModuleName("XdevLComputeDevice"), xdl::XdevLID("MyComputeDevice"));

	// We need a context.
	auto context = computeDevice->createContext();

	// We need a command queue to run commands.
	auto commandQueue = context->createCommandQueue();

	
	auto program = context->createProgram();
	auto inBuffer = context->createBuffer(xdl::XDEVL_COMPUTE_BUFFER_READ_ONLY, sizeof(float) * 10);
	auto outBuffer = context->createBuffer(xdl::XDEVL_COMPUTE_BUFFER_WRITE_ONLY, sizeof(float) * 10);

	//
	// Load and build the kernel.
	//
	auto kernel = program->compileFromFile(xdl::XdevLFileName("compute_device_demo.cl"), xdl::XdevLString("calculate_sqrt"));

	kernel->setArgument(0, inBuffer);
	kernel->setArgument(1, outBuffer);

	inBuffer->upload(commandQueue.get(), sizeof(float) * 10, (xdl::xdl_uint8*)data);


	program->execute(commandQueue.get(), kernel.get());


	std::cout << "Before: " << std::endl;
	for(auto item : data) {
		std::cout << item << " : ";
	}
	std::cout << std::endl;

	outBuffer->download(commandQueue.get(), sizeof(float) * 10, (xdl::xdl_uint8*)data);

	std::cout << "After: " << std::endl;
	for(auto item : data) {
		std::cout << item << " : ";
	}
	std::cout << std::endl;

	xdl::destroyCore(core);
}
