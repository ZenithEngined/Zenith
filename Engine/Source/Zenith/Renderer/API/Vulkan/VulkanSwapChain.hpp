#pragma once

#include "Zenith/Core/Base.hpp"

#include "Vulkan.hpp"
#include "VulkanDevice.hpp"
#include "VulkanAllocator.hpp"

#include <vector>

struct SDL_Window;

namespace Zenith {

	class VulkanSwapChain
	{
	public:
		VulkanSwapChain() = default;

		void Init(VkInstance instance, const Ref<VulkanDevice>& device);
		void InitSurface(SDL_Window* windowHandle);
		void Create(uint32_t* width, uint32_t* height, bool vsync = false);

		void OnResize(uint32_t width, uint32_t height);

		void BeginFrame();
		void Present();

		uint32_t GetImageCount() const { return m_ImageCount; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		VkRenderPass GetRenderPass() { return m_RenderPass; }

		VkFramebuffer GetCurrentFramebuffer() { return GetFramebuffer(m_CurrentBufferIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return GetDrawCommandBuffer(m_CurrentBufferIndex); }

		VkFormat GetColorFormat() { return m_ColorFormat; }

		uint32_t GetCurrentBufferIndex() const { return m_CurrentBufferIndex; }
		VkFramebuffer GetFramebuffer(uint32_t index)
		{
			ZN_CORE_ASSERT(index < m_ImageCount);
			return m_Framebuffers[index];
		}
		VkCommandBuffer GetDrawCommandBuffer(uint32_t index)
		{
			ZN_CORE_ASSERT(index < m_ImageCount);
			return m_DrawCommandBuffers[index];
		}

		void Cleanup();
	private:
		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

		void CreateFramebuffer();
		void CreateDepthStencil();
		void CreateDrawBuffers();
		void FindImageFormatAndColorSpace();
	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		Ref<VulkanDevice> m_Device;
		VulkanAllocator m_Allocator;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
		uint32_t m_ImageCount = 0;
		std::vector<VkImage> m_Images;

		struct SwapChainBuffer
		{
			VkImage image;
			VkImageView view;
		};
		std::vector<SwapChainBuffer> m_Buffers;

		struct
		{
			VkImage Image;
			VkDeviceMemory DeviceMemory;
			VkImageView ImageView;
		} m_DepthStencil;

		std::vector<VkFramebuffer> m_Framebuffers;
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> m_DrawCommandBuffers;

		struct
		{
			// Swap chain
			VkSemaphore PresentComplete;
			// Command buffer
			VkSemaphore RenderComplete;
		} m_Semaphores;
		VkSubmitInfo m_SubmitInfo;

		std::vector<VkFence> m_WaitFences;

		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		uint32_t m_CurrentBufferIndex = 0;

		uint32_t m_QueueNodeIndex = UINT32_MAX;
		uint32_t m_Width = 0, m_Height = 0;

		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

		bool m_JustResized = false;
		bool m_SkipNextFrame = false;

		friend class VulkanContext;
	};
}