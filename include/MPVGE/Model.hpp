/*
 * Created by gbian on 09/04/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast)
#pragma once

#include "Device.hpp"
#include "headers.hpp"

namespace mpvge {

    class Model {
    public:
        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        Model(Device &devicein, const std::vector<Vertex> &vertices);
        ~Model();
        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        Device &device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };

}  // namespace mpvge

// NOLINTEND(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast)