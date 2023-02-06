#version 430 core

layout(local_size_x = 1024) in;

shared float sharedData[gl_WorkGroupSize.x * 2];

layout(binding = 0, r32f) readonly uniform image2D inputImage;
layout(binding = 1, r32f) writeonly uniform image2D outputImage;

void main(void)
{
	uint id = gl_LocalInvocationID.x;
	uint rdId;
	uint wrId;
	uint mask;
	ivec2 P = ivec2(id * 2, gl_WorkGroupID.x);

	const uint steps = uint(log2(gl_WorkGroupSize.x)) + 1;
	uint step = 0;

	sharedData[id * 2] = imageLoad(inputImage, P).r;
	sharedData[id * 2 + 1] = imageLoad(inputImage, P + ivec2(1, 0)).r;

	barrier();
	memoryBarrierShared();

	for(step = 0 ; step < steps; step++)
	{
		mask = (1 << step) - 1;
		rdId = ((id >> step) << (step + 1)) + mask;
		wrId = rdId + 1 + (id & mask);

		sharedData[wrId] += sharedData[rdId];

		barrier();
		memoryBarrierShared();
	}

	imageStore(outputImage, P.yx, vec4(sharedData[id * 2]));
	imageStore(outputImage, P.yx + ivec2(0, 1), vec4(sharedData[id * 2 + 1]));
}