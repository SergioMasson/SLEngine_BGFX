#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include <vector>

namespace SLEngine
{
    class MeshRenderer
    {
    public:
        static std::unique_ptr<MeshRenderer> CreateFromFile(std::filesystem::path path);
    private:
        std::vector<uint32_t> m_vertexBuffersHandlers;
        uint32_t m_indexBufferHandler;
    };
}