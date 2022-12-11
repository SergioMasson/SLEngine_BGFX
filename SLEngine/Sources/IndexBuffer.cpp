#include "SLEngine/Internal/IndexBuffer.h"
#include "SLEngine/Debug.h"

namespace SLEngine
{
    IndexBuffer::IndexBuffer(std::vector<uint8_t>& bytes, uint16_t flags, bool dynamic)
        : m_bytes{bytes.data(), bytes.data() + bytes.size()}
        , m_flags{flags}
        , m_dynamic{dynamic}
    {
    }

    IndexBuffer::~IndexBuffer()
    {
        Dispose();
    }

    void IndexBuffer::Dispose()
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

    void IndexBuffer::Update(std::vector<uint8_t>& bytes, uint32_t startIndex)
    {
        SLENGINE_ASSERT(m_dynamic, "Error: Trying to update a non-dynamic buffer");

        if (bgfx::isValid(m_dynamicHandle))
        {
            // Buffer was already created, do a real update operation.
            bgfx::update(m_dynamicHandle, startIndex, bgfx::copy(bytes.data(), static_cast<uint32_t>(bytes.size())));
        }
        else
        {
            // Buffer hasn't been finalized yet, all that's necessary is to swap out the bytes.
            m_bytes = {bytes.data(), bytes.data() + bytes.size()};
        }
    }

    bool IndexBuffer::CreateHandle()
    {
        if (bgfx::isValid(m_handle))
        {
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
            m_dynamicHandle = bgfx::createDynamicIndexBuffer(memory, m_flags);
        }
        else
        {
            m_handle = bgfx::createIndexBuffer(memory, m_flags);
        }

        return bgfx::isValid(m_handle);
    }

    void IndexBuffer::Set(uint32_t firstIndex, uint32_t numIndices)
    {
        if (bgfx::isValid(m_handle))
        {
            if (m_dynamic)
            {
                bgfx::setIndexBuffer(m_dynamicHandle, firstIndex, numIndices);
            }
            else
            {
                bgfx::setIndexBuffer(m_handle, firstIndex, numIndices);
            }
        }
    }
}