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


	for(int a = 0; a < 100; a++) {
		kernel->setArgumentBuffer(0, inBuffer);
		kernel->setArgumentBuffer(1, outBuffer);
		kernel->setArgumentFloat(2, 2);

		inBuffer->upload(commandQueue.get(), sizeof(float) * 10, (xdl::xdl_uint8*)data);

		xdl::XdevLComputeExecuteParameter para(commandQueue.get(), kernel.get(), {32});
		program->execute(para);


//		std::cout << "Before: " << std::endl;
//		for(auto item : data) {
//			std::cout << item << " : ";
//		}
//		std::cout << std::endl;

		outBuffer->download(commandQueue.get(), sizeof(float) * 10, (xdl::xdl_uint8*)data);

		std::cout << "After: " << std::endl;
		for(auto item : data) {
			std::cout << item << " : ";
		}
		std::cout << std::endl;
	}
	xdl::destroyCore(core);
}
