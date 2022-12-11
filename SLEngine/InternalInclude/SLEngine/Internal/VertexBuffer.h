#pragma once

#include <bgfx/bgfx.h>
#include <map>
#include <optional>
#include <vector>

namespace SLEngine
{
    class VertexBuffer final
    {
    public:
        VertexBuffer(std::vector<uint8_t> bytes, bool dynamic);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        void Dispose();

        void Update(std::vector<uint8_t> bytes, size_t byteOffset);
        bool CreateHandle(const bgfx::VertexLayout& layout);
        void PromoteToFloats(bgfx::AttribType::Enum attribType, uint32_t numElements, uint32_t byteOffset, uint32_t byteStride);
        void Set(uint8_t stream, uint32_t startVertex, uint32_t numVertices, bgfx::VertexLayoutHandle layoutHandle);
     
    private:
        std::vector<uint8_t> m_bytes{};
        bool m_dynamic{};

        union
        {
            bgfx::VertexBufferHandle m_handle{bgfx::kInvalidHandle};
            bgfx::DynamicVertexBufferHandle m_dynamicHandle;
        };

        bool m_disposed{};
    };
}