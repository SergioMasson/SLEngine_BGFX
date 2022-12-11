#include "SLEngine/Internal/VertexBuffer.h"
#include "SLEngine/Debug.h"

namespace SLEngine
{
    template<typename sourceType> void PromoteToFloats(std::vector<uint8_t>& bytes, uint32_t numElements, uint32_t byteOffset, uint32_t byteStride)
    {
        const size_t count = bytes.size() / byteStride;
        const size_t destinationSize = count * numElements * sizeof(float);
        if (destinationSize != bytes.size()) // ensure both vectors have different size
        {
            std::vector<uint8_t> newBytes(destinationSize);
            float* destination = reinterpret_cast<float*>(newBytes.data());
            for (size_t i = 0; i < count; ++i)
            {
                sourceType* source = reinterpret_cast<sourceType*>(bytes.data() + byteOffset + byteStride * i);
                for (size_t element = 0; element < numElements; element++)
                {
                    *destination++ = static_cast<float>(*source++);
                }
            }
            bytes.swap(newBytes);
        }
    }

    VertexBuffer::VertexBuffer(std::vector<uint8_t> bytes, bool dynamic)
        : m_bytes{bytes.data(), bytes.data() + bytes.size()}
        , m_dynamic{dynamic}
    {
    }

    VertexBuffer::~VertexBuffer()
    {
        Dispose();
    }

    void VertexBuffer::Dispose()
    {
        if (m_disposed)
        {
            return;
        }

        if (bgfx::isValid(m_handle))
        {
            if (m_dynamic)
            {
                bgfx::destroy(m_dynamicHandle);
            }
            else
            {
                bgfx::destroy(m_handle);
            }
        }

        m_bytes = {};
        m_disposed = true;
    }

    void VertexBuffer::Update(std::vector<uint8_t> bytes, size_t byteOffset)
    {
        SLENGINE_ASSERT(m_dynamic, "Error: Trying to update a non-dynamic VertexBuffer");

        if (bgfx::isValid(m_dynamicHandle))
        {
            SLENGINE_ASSERT(byteOffset, "Dynamic VertexBuffer Update: byte offset unimplemented.");

            // Buffer was already created, do a real update operation.
            bgfx::update(m_dynamicHandle, 0, bgfx::copy(bytes.data(), static_cast<uint32_t>(bytes.size())));
        }       
    }

    bool VertexBuffer::CreateHandle(const bgfx::VertexLayout& layout)
    {
        if (bgfx::isValid(m_handle))
        {
            // NOTE: This code is assuming that layout stride hasn't changed.
            return true;
        }

        auto releaseFn = [](void*, void* userData)
        {
            auto* bytes = reinterpret_cast<decltype(m_bytes)*>(userData);
            bytes->clear();
        };

        const bgfx::Memory* memory = bgfx::makeRef(m_bytes.data(), static_cast<uint32_t>(m_bytes.size()), releaseFn, &m_bytes);

        if (m_dynamic)
        {
            m_dynamicHandle = bgfx::createDynamicVertexBuffer(memory, layout);
        }
        else
        {
            m_handle = bgfx::createVertexBuffer(memory, layout);
        };

        return bgfx::isValid(m_handle);
    }

    void VertexBuffer::PromoteToFloats(bgfx::AttribType::Enum attribType, uint32_t numElements, uint32_t byteOffset, uint32_t byteStride)
    {
        switch (attribType)
        {
            case bgfx::AttribType::Int8:
            {
                SLEngine::PromoteToFloats<int8_t>(m_bytes, numElements, byteOffset, byteStride);
                break;
            }
            case bgfx::AttribType::Uint8:
            {
                SLEngine::PromoteToFloats<uint8_t>(m_bytes, numElements, byteOffset, byteStride);
                break;
            }
            case bgfx::AttribType::Int16:
            {
                SLEngine::PromoteToFloats<int16_t>(m_bytes, numElements, byteOffset, byteStride);
                break;
            }
            case bgfx::AttribType::Uint16:
            {
                SLEngine::PromoteToFloats<uint16_t>(m_bytes, numElements, byteOffset, byteStride);
                break;
            }
            case bgfx::AttribType::Uint10: // is supported by any format ?
            default:
            {
                SLENGINE_ASSERT(false, "Unable to promote vertex stream to a float array.");
            }
        }
    }

    void VertexBuffer::Set(uint8_t stream, uint32_t startVertex, uint32_t numVertices, bgfx::VertexLayoutHandle layoutHandle)
    {
        if (bgfx::isValid(m_handle))
        {
            if (m_dynamic)
            {
                bgfx::setVertexBuffer(stream, m_dynamicHandle, startVertex, numVertices, layoutHandle);
            }
            else
            {
                bgfx::setVertexBuffer(stream, m_handle, startVertex, numVertices, layoutHandle);
            }
        }
    }
}