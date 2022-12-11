#pragma once

#include "bgfx/bgfx.h"
#include <string>

namespace SLEngine
{
    class UniformBuffer final
    {
    public:
        UniformBuffer(std::string& name, bgfx::UniformType::Enum type, uint16_t num);
        ~UniformBuffer();

        UniformBuffer(const UniformBuffer&) = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;

        void Update(void* value, uint16_t num);

        void Dispose();
    private:
        uint8_t Stage{};
        bgfx::UniformHandle m_handle;
        size_t MaxElementLength{};
    };
}