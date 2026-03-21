#include "FSR2.h"
#include "GlobalDX.h"
#include "Resource.h"

#include <cmath>

using namespace RT;

struct Data
{
#ifdef interface
#undef interface
#endif
	ID3D12Resource* scratch_buffer;
	void* scratch_buffer_ptr;
	
} static data;

void FSR2::Init()
{
}

void FSR2::Exit()
{
}

void FSR2::Dispatch(ID3D12CommandList* command_list, ID3D12Resource* rt_color, ID3D12Resource* rt_depth,
	ID3D12Resource* rt_motion_vector, ID3D12Resource* rt_reactive, ID3D12Resource* rt_output, uint32_t render_width, uint32_t render_height,
	float camera_jitter_x, float camera_jitter_y, float camera_near, float camera_far, float camera_vfov_angle, float delta_time, bool reset)
{
}

void FSR2::AdjustRenderResolutionForFSRMode(uint32_t output_width, uint32_t output_height, uint32_t& render_width, uint32_t& render_height)
{
}

void FSR2::RescaleResolutionForFSRMode(uint32_t& width, uint32_t& height)
{
}

RT_Vec2 FSR2::GetMipBiasForFSRMode(uint32_t output_width, uint32_t output_height, uint32_t render_width, uint32_t render_height)
{
	return RT_Vec2Make(std::log2f((float)render_width / (float)output_width) - 1.0f, std::log2f((float)render_height / (float)output_height) - 1.0f);
}

void FSR2::OnWindowResize(uint32_t width, uint32_t height)
{
}
