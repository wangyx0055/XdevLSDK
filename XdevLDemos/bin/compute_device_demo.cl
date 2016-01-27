__kernel void calculate_sqrt(__global float* input, __global float* output, float value) {
   size_t tid = get_global_id(0);
	 output[tid] = value * (input[tid] + 1.0f);
}