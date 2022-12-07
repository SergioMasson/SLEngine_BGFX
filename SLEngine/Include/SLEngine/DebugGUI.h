#pragma once

#include <string>

struct GLFWwindow;

namespace SLEngine
{
    struct DebugGUIFloat
    {
        public:
            DebugGUIFloat() : m_value(nullptr){}
            DebugGUIFloat(float* value) : m_value(value){}
            
            inline float Get() const 
            {
                if(m_value != nullptr)
                    return *m_value;
                
                return 0;
            }

            inline void Set(float value)
            {
                if(m_value != nullptr)
                    *m_value = value;
            }
            inline operator float() const { return *m_value; }
        private:
            float* m_value;
    };

    enum class FloatWidget
    {
        InputField,
        Slider,
        Drag
    };

    namespace DebugGUI
    {
        DebugGUIFloat CreateGUIFloat(float value, std::string text, FloatWidget widget = FloatWidget::InputField, float sliderMin = 0, float sliderMax = 1);
        void DeleteGUIFloat(DebugGUIFloat debugFloat);
    }
}