#pragma once

#include <vector>
#include "bgfx/bgfx.h"

namespace SLEngine 
{
    class IndexBuffer final
    {
    public:
        IndexBuffer(std::vector<uint8_t>& bytes, uint16_t flags, bool dynamic);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        void Dispose();

        void Update(std::vector<uint8_t>& bytes, uint32_t startIndex);
        bool CreateHandle();
        void Set(uint32_t firstIndex, uint32_t numIndices);

    private:
        std::vector<uint8_t> m_bytes{};
        uint16_t m_flags{};
        bool m_dynamic{};

        union
        {
            bgfx::IndexBufferHandle m_handle{bgfx::kInvalidHandle};
            bgfx::DynamicIndexBufferHandle m_dynamicHandle;
        };

        bool m_disposed{};
    };
}