#include "SLEngine/Internal/UniformBuffer.h"

namespace SLEngine
{
    UniformBuffer::UniformBuffer(std::string& name, bgfx::UniformType::Enum type, uint16_t num)
    {
        m_handle = bgfx::createUniform(name.c_str(), type);
    }

    void UniformBuffer::Update(void* value, uint16_t num)
    {
        bgfx::setUniform(m_handle, value);
    }

    void UniformBuffer::Dispose()
    {
        if (bgfx::isValid(m_handle))
        {
            bgfx::destroy(m_handle);
            m_handle = {bgfx::kInvalidHandle};
        }
        
    }

    UniformBuffer::~UniformBuffer()
    {
        Dispose();
    }
}