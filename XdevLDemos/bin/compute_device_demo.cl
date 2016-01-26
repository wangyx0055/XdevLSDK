
__kernel void calculate_sqrt(__global float* input, __global float* output) {
   size_t tid = get_global_id(0);
	 output[tid] = input[tid] * input[tid];
}