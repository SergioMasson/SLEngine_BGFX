#include "SLEngine/Internal/VertexArray.h"
#include "SLEngine/Debug.h"

namespace SLEngine
{
    VertexArray::~VertexArray()
    {
        Dispose();
    }

    void VertexArray::Dispose()
    {
        if (m_disposed)
        {
            return;
        }

        m_indexBufferRecord.Buffer = nullptr;

        for (auto &pair : m_vertexBufferRecords)
        {
            bgfx::destroy(pair.second.LayoutHandle);
        }

        m_vertexBufferRecords.clear();

        m_disposed = true;
    }

    bool VertexArray::RecordIndexBuffer(IndexBuffer *indexBuffer)
    {
        if (!indexBuffer->CreateHandle())
        {
            return false;
        }

        SLENGINE_ASSERT(m_indexBufferRecord.Buffer == nullptr);
        m_indexBufferRecord.Buffer = indexBuffer;

        return true;
    }

    bool VertexArray::RecordVertexBuffer(VertexBuffer *vertexBuffer, bgfx::Attrib::Enum location, uint32_t byteOffset, uint32_t byteStride, uint32_t numElements, bgfx::AttribType::Enum type, bool normalized)
    {
        bgfx::VertexLayout layout{};
        layout.begin();

        // clang-format off
        const bool promoteToFloats = !normalized
                && (bgfx::getCaps()->rendererType == bgfx::RendererType::Direct3D11 ||
                    bgfx::getCaps()->rendererType == bgfx::RendererType::Direct3D12 ||
                    bgfx::getCaps()->rendererType == bgfx::RendererType::Vulkan)
                && (type == bgfx::AttribType::Int8 ||
                    type == bgfx::AttribType::Uint8 ||
                    type == bgfx::AttribType::Uint10 ||
                    type == bgfx::AttribType::Int16 ||
                    type == bgfx::AttribType::Uint16);
        // clang-format on

        if (promoteToFloats)
        {
            layout.add(location, static_cast<uint8_t>(numElements), bgfx::AttribType::Float);
            layout.m_stride = static_cast<uint16_t>(sizeof(float) * numElements);
            vertexBuffer->PromoteToFloats(type, numElements, byteOffset, byteStride);
        }
        else
        {
            layout.add(location, static_cast<uint8_t>(numElements), type, normalized);
            layout.m_stride = static_cast<uint16_t>(byteStride);
            layout.m_offset[location] = static_cast<uint16_t>(byteOffset % byteStride);
        }

        layout.end();

        if (!vertexBuffer->CreateHandle(layout))
        {
            return false;
        }

        m_vertexBufferRecords[location] = {vertexBuffer, byteOffset / byteStride, bgfx::createVertexLayout(layout)};
        return true;
    }

    void VertexArray::SetIndexBuffer(uint32_t firstIndex, uint32_t numIndices)
    {
        if (m_indexBufferRecord.Buffer != nullptr)
        {
            m_indexBufferRecord.Buffer->Set(firstIndex, numIndices);
        }
    }

    void VertexArray::SetVertexBuffers(uint32_t startVertex, uint32_t numVertices)
    {
        for (const auto &pair : m_vertexBufferRecords)
        {
            auto stream{static_cast<uint8_t>(pair.first)};
            auto &record{pair.second};
            record.Buffer->Set(stream, record.Offset + startVertex, numVertices, record.LayoutHandle);
        }
    }
}