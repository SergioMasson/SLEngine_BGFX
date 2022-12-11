#pragma once

#include "SLEngine/Internal/IndexBuffer.h"
#include "SLEngine/Internal/VertexBuffer.h"
#include <map>

namespace SLEngine
{
    class VertexArray final
    {
    public:
        VertexArray() = default;
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        void Dispose();

        bool RecordIndexBuffer(IndexBuffer* indexBuffer);
        bool RecordVertexBuffer(VertexBuffer* vertexBuffer, bgfx::Attrib::Enum location, uint32_t byteOffset, uint32_t byteStride, uint32_t numElements, bgfx::AttribType::Enum type, bool normalized);
        void SetIndexBuffer(uint32_t firstIndex, uint32_t numIndices);
        void SetVertexBuffers(uint32_t startVertex, uint32_t numVertices);

    private:
        struct IndexBufferRecord
        {
            IndexBuffer* Buffer{};
        };

        IndexBufferRecord m_indexBufferRecord{};

        struct VertexBufferRecord
        {
            VertexBuffer* Buffer{};
            uint32_t Offset{};
            bgfx::VertexLayoutHandle LayoutHandle{};
        };

        std::map<bgfx::Attrib::Enum, VertexBufferRecord> m_vertexBufferRecords{};

        bool m_disposed{};
    };
}