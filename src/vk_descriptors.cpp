#include "vk_descriptors.h"

VkDescriptorPool createPool(VkDevice device, const DescriptorAllocator::PoolSizes& poolSizes, int count, VkDescriptorPoolCreateFlags flags) {
	std::vector<VkDescriptorPoolSize> sizes;
	sizes.reserve(poolSizes.sizes.size());

	for (auto sz : poolSizes.sizes) {
		sizes.push_back({ sz.first, uint32_t(sz.second * count) });
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = flags;
	poolInfo.maxSets = count;
	poolInfo.poolSizeCount = (uint32_t)sizes.size();
	poolInfo.pPoolSizes = sizes.data();

	VkDescriptorPool descriptorPool;
	vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);

	return descriptorPool;
}

void DescriptorAllocator::reset_pools()
{
	for (auto p : m_UsedPools) {
		vkResetDescriptorPool(m_Device, p, 0);
		m_FreePools.push_back(p);
	}

	m_UsedPools.clear();
	m_CurrentPool = VK_NULL_HANDLE;
}

bool DescriptorAllocator::allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout)
{
	CORE_ASSERT(m_Device, "DescriptorAllocator is not initialized");

	if (m_CurrentPool == VK_NULL_HANDLE)
	{
		m_CurrentPool = grab_pool();
		m_UsedPools.push_back(m_CurrentPool);
	}

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.pSetLayouts = &layout;
	allocInfo.descriptorPool = m_CurrentPool;
	allocInfo.descriptorSetCount = 1;

	VkResult allocResult = vkAllocateDescriptorSets(m_Device, &allocInfo, set);
	bool needReallocate = false;

	switch (allocResult) {
	case VK_SUCCESS:
		return true;
	case VK_ERROR_FRAGMENTED_POOL:
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		needReallocate = true;
		break;
	default:
		CORE_WARN("ERROR: {}", allocResult);
		CORE_WARN("Could not allocate descriptor set with layout: {}", layout);
		return false;
	}

	if (needReallocate) {
		m_CurrentPool = grab_pool();
		m_UsedPools.push_back(m_CurrentPool);

		allocResult = vkAllocateDescriptorSets(m_Device, &allocInfo, set);

		if (allocResult == VK_SUCCESS) return true;
	}

	CORE_WARN("ERROR: {}", allocResult);
	CORE_WARN("Could not allocate descriptor set with layout: {}", layout);
	return false;
}

void DescriptorAllocator::init(VkDevice device)
{
	m_Device = device;
}

void DescriptorAllocator::cleanup()
{
	CORE_ASSERT(m_Device, "DescriptorAllocator is not initialized");

	for (auto p : m_FreePools)
	{
		vkDestroyDescriptorPool(m_Device, p, nullptr);
	}
	for (auto p : m_UsedPools)
	{
		vkDestroyDescriptorPool(m_Device, p, nullptr);
	}
}

VkDescriptorPool DescriptorAllocator::grab_pool()
{
	CORE_ASSERT(m_Device, "DescriptorAllocator is not initialized");

	if (m_FreePools.size() > 0)
	{
		VkDescriptorPool pool = m_FreePools.back();
		m_FreePools.pop_back();
		return pool;
	}
	else
	{
		return createPool(m_Device, m_DescriptorSizes, 1000, 0);
	}
}

void DescriptorLayoutCache::init(VkDevice device)
{
	m_Device = device;
}

void DescriptorLayoutCache::cleanup()
{
	for (auto pair : m_LayoutCache) {
		vkDestroyDescriptorSetLayout(m_Device, pair.second, nullptr);
	}
}

VkDescriptorSetLayout DescriptorLayoutCache::create_descriptor_layout(VkDescriptorSetLayoutCreateInfo* info)
{
	CORE_ASSERT(m_Device, "DescriptorLayoutCache is not initialized");

	DescriptorLayoutInfo layoutInfo;
	bool isSorted = true;
	int lastBinding = -1;

	for (int i = 0; i < info->bindingCount; i++) {
		layoutInfo.bindings.push_back(info->pBindings[i]);

		if (info->pBindings[i].binding > lastBinding) {
			lastBinding = info->pBindings[i].binding;
		}
		else {
			isSorted = false;
		}
	}

	if (!isSorted) {
		std::sort(layoutInfo.bindings.begin(), layoutInfo.bindings.end(),
			[](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) {
				return a.binding < b.binding;
			});
	}

	auto it = m_LayoutCache.find(layoutInfo);
	if (it != m_LayoutCache.end()) return (*it).second;
	else {
		VkDescriptorSetLayout layout;
		vkCreateDescriptorSetLayout(m_Device, info, nullptr, &layout);

		m_LayoutCache[layoutInfo] = layout;
		return layout;
	}
}

bool DescriptorLayoutCache::DescriptorLayoutInfo::operator==(const DescriptorLayoutInfo& other) const
{
	if (other.bindings.size() != bindings.size()) {
		return false;
	}
	else {
		for (int i = 0; i < bindings.size(); i++) {
			if (other.bindings[i].binding != bindings[i].binding) return false;
			if (other.bindings[i].descriptorType != bindings[i].descriptorType)	return false;
			if (other.bindings[i].descriptorCount != bindings[i].descriptorCount) return false;
			if (other.bindings[i].stageFlags != bindings[i].stageFlags) return false;
		}
	}

	return true;
}

size_t DescriptorLayoutCache::DescriptorLayoutInfo::hash() const
{
	using std::size_t;
	using std::hash;

	size_t result = hash<size_t>()(bindings.size());

	for (const VkDescriptorSetLayoutBinding& b : bindings) {
		size_t bindingHash = b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;

		result ^= hash<size_t>()(bindingHash);
	}

	return result;
}

DescriptorBuilder& DescriptorBuilder::bind_buffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags flags)
{
	VkDescriptorSetLayoutBinding newBinding{};
	newBinding.descriptorCount = 1;
	newBinding.descriptorType = type;
	newBinding.pImmutableSamplers = nullptr;
	newBinding.stageFlags = flags;
	newBinding.binding = binding;

	bindings.push_back(newBinding);

	VkWriteDescriptorSet newWrite{};
	newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	newWrite.pNext = nullptr;

	newWrite.descriptorCount = 1;
	newWrite.descriptorType = type;
	newWrite.pBufferInfo = bufferInfo;
	newWrite.dstBinding = binding;

	writes.push_back(newWrite);
	return *this;
}

bool DescriptorBuilder::build(VkDescriptorSet& set, VkDescriptorSetLayout& layout)
{
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = nullptr;

	layoutInfo.pBindings = bindings.data();
	layoutInfo.bindingCount = bindings.size();

	layout = m_LayoutCache->create_descriptor_layout(&layoutInfo);

	bool success = m_Alloc->allocate(&set, layout);
	if (!success) { return false; };

	for (VkWriteDescriptorSet& w : writes) {
		w.dstSet = set;
	}

	vkUpdateDescriptorSets(m_Alloc->m_Device, writes.size(), writes.data(), 0, nullptr);

	return true;
}
