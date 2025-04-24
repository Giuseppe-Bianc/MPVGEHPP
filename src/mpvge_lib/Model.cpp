/*
 * Created by gbian on 09/04/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast, *-signed-bitwise)
// clang-format on
#include "../../include/MPVGE/Model.hpp"

namespace mpvge {

    static inline constexpr auto vertSize = sizeof(Model::Vertex);

    Model::Model(Device &devicein, const std::vector<Vertex> &vertices) : device{devicein} { createVertexBuffers(vertices); }

    Model::~Model() {
        DESTROY_VK_HANDLE(vertexBuffer, vkDestroyBuffer(device.getDevice(), vertexBuffer, nullptr));
        DESTROY_VK_HANDLE(vertexBufferMemory, vkFreeMemory(device.getDevice(), vertexBufferMemory, nullptr));
    }

    void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = C_UI32T(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

        void *data;
        auto deviceHandle = device.getDevice();
        vkMapMemory(deviceHandle, vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), C_ST(bufferSize));
        vkUnmapMemory(deviceHandle, vertexBufferMemory);
    }

    void Model::draw(VkCommandBuffer commandBuffer) { vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0); }

    void Model::bind(VkCommandBuffer commandBuffer) {
        const VkBuffer buffers[] = {vertexBuffer};
        const VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = vertSize;
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }

}  // namespace mpvge

// clang-format off
// NOLINTEND(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast, *-signed-bitwise)
// clang-format on
