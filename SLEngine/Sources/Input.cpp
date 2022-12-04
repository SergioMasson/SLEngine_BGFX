#include "SLEngine/Internal/Input.h"
#include "GLFW/glfw3.h"
#include <map>

using namespace SLEngine::Input;

// Buffer for all buttons. We need two buffers in order to know if the key has been release last frame or not.
static bool s_Buttons[2][(int)KeyCode::NumDigitalInputs];

// Cumulative buffer with time past since key press.
static float s_HoldDuration[(int)KeyCode::NumDigitalInputs] = {0.0f};

// Buffer to store analog input.
static float s_Analogs[static_cast<uint32_t>(AnalogInput::kNumAnalogInputs)];

// Buffer to store normalized analog input.
static float s_AnalogsTC[static_cast<uint32_t>(AnalogInput::kNumAnalogInputs)];

// Dictionary to map GLFW key codes to our API key enum.
static std::map<int, KeyCode> g_windowToKeyMap;

// Target glfw window. (Assumes application will only support a single window)
static GLFWwindow *g_window;

void KbmBuildKeyMapping()
{
    // Maps the GLFW key codes to our API internal keycodes
    g_windowToKeyMap = std::map<int, KeyCode>{
        {GLFW_KEY_ESCAPE, KeyCode::Key_escape},
        {GLFW_KEY_0, KeyCode::Key_0},
        {GLFW_KEY_1, KeyCode::Key_1},
        {GLFW_KEY_2, KeyCode::Key_2},
        {GLFW_KEY_3, KeyCode::Key_3},
        {GLFW_KEY_4, KeyCode::Key_4},
        {GLFW_KEY_5, KeyCode::Key_5},
        {GLFW_KEY_6, KeyCode::Key_6},
        {GLFW_KEY_7, KeyCode::Key_7},
        {GLFW_KEY_8, KeyCode::Key_8},
        {GLFW_KEY_9, KeyCode::Key_9},
        {GLFW_KEY_A, KeyCode::Key_a},
        {GLFW_KEY_B, KeyCode::Key_b},
        {GLFW_KEY_C, KeyCode::Key_c},
        {GLFW_KEY_D, KeyCode::Key_d},
        {GLFW_KEY_E, KeyCode::Key_e},
        {GLFW_KEY_F, KeyCode::Key_f},
        {GLFW_KEY_G, KeyCode::Key_g},
        {GLFW_KEY_H, KeyCode::Key_h},
        {GLFW_KEY_I, KeyCode::Key_i},
        {GLFW_KEY_J, KeyCode::Key_j},
        {GLFW_KEY_K, KeyCode::Key_k},
        {GLFW_KEY_L, KeyCode::Key_l},
        {GLFW_KEY_M, KeyCode::Key_m},
        {GLFW_KEY_N, KeyCode::Key_n},
        {GLFW_KEY_O, KeyCode::Key_o},
        {GLFW_KEY_P, KeyCode::Key_p},
        {GLFW_KEY_Q, KeyCode::Key_q},
        {GLFW_KEY_R, KeyCode::Key_r},
        {GLFW_KEY_S, KeyCode::Key_s},
        {GLFW_KEY_T, KeyCode::Key_t},
        {GLFW_KEY_U, KeyCode::Key_u},
        {GLFW_KEY_V, KeyCode::Key_v},
        {GLFW_KEY_W, KeyCode::Key_w},
        {GLFW_KEY_X, KeyCode::Key_x},
        {GLFW_KEY_Y, KeyCode::Key_y},
        {GLFW_KEY_Z, KeyCode::Key_z},

        // Non alpha numeric keys
        {GLFW_KEY_SPACE, KeyCode::Key_space},
        {GLFW_KEY_BACKSPACE, KeyCode::Key_back},
        {GLFW_KEY_UP, KeyCode::Key_up},
        {GLFW_KEY_DOWN, KeyCode::Key_down},
        {GLFW_KEY_LEFT, KeyCode::Key_left},
        {GLFW_KEY_RIGHT, KeyCode::Key_right},
        {GLFW_KEY_ENTER, KeyCode::Key_return},
        {GLFW_KEY_KP_ADD, KeyCode::Key_add},
        {GLFW_KEY_KP_SUBTRACT, KeyCode::Key_minus},

        // Mouse buttons
        {GLFW_MOUSE_BUTTON_RIGHT, KeyCode::MouseRigth},
        {GLFW_MOUSE_BUTTON_LEFT, KeyCode::MouseLeft},
        {GLFW_MOUSE_BUTTON_MIDDLE, KeyCode::MouseMiddle},

        // Numpad
        {GLFW_KEY_KP_0, KeyCode::Key_numpad0},
        {GLFW_KEY_KP_1, KeyCode::Key_numpad1},
        {GLFW_KEY_KP_2, KeyCode::Key_numpad2},
        {GLFW_KEY_KP_3, KeyCode::Key_numpad3},
        {GLFW_KEY_KP_4, KeyCode::Key_numpad4},
        {GLFW_KEY_KP_5, KeyCode::Key_numpad5},
        {GLFW_KEY_KP_6, KeyCode::Key_numpad6},
        {GLFW_KEY_KP_7, KeyCode::Key_numpad7},
        {GLFW_KEY_KP_8, KeyCode::Key_numpad8},
        {GLFW_KEY_KP_9, KeyCode::Key_numpad9},

        // F keys
        {GLFW_KEY_F1, KeyCode::Key_f1},
        {GLFW_KEY_F2, KeyCode::Key_f2},
        {GLFW_KEY_F3, KeyCode::Key_f3},
        {GLFW_KEY_F4, KeyCode::Key_f4},
        {GLFW_KEY_F5, KeyCode::Key_f5},
        {GLFW_KEY_F6, KeyCode::Key_f6},
        {GLFW_KEY_F7, KeyCode::Key_f7},
        {GLFW_KEY_F8, KeyCode::Key_f8},
        {GLFW_KEY_F9, KeyCode::Key_f9},
        {GLFW_KEY_F10, KeyCode::Key_f10},
        {GLFW_KEY_F11, KeyCode::Key_f11},
        {GLFW_KEY_F12, KeyCode::Key_f12}};
}

float FilterAnalogInput(int val, int deadZone)
{
    if (val < 0)
    {
        if (val > -deadZone)
            return 0.0f;
        else
            return (val + deadZone) / (32768.0f - deadZone);
    }
    else
    {
        if (val < deadZone)
            return 0.0f;
        else
            return (val - deadZone) / (32767.0f - deadZone);
    }
}

void NormalizeMousePosition(float x, float y, GLFWwindow *window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float normalizedX = (x / static_cast<float>(width)) - 0.5f;
    float normalizedY = (y / static_cast<float>(height)) - 0.5f;

    s_Analogs[static_cast<uint32_t>(AnalogInput::MouseX)] = -normalizedX;
    s_Analogs[static_cast<uint32_t>(AnalogInput::MouseY)] = normalizedY;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    SLEngine::Input::SetKey(key, action);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    SLEngine::Input::SetScrollValue(static_cast<float>(yoffset));
}

namespace SLEngine
{
    namespace Input
    {
        void Initialize(GLFWwindow *window)
        {
            g_window = window;
            memset(s_Buttons, 0, sizeof(s_Buttons));
            memset(s_Analogs, 0, sizeof(s_Analogs));
            KbmBuildKeyMapping();
            glfwSetKeyCallback(window, key_callback);
            glfwSetScrollCallback(window, scroll_callback);
        }

        void Shutdown()
        {
            g_window = nullptr;
        }

        void Update(float frameDelta)
        {
            if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
            {
                GLFWgamepadstate state;

                if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
                {
                    // Read all digital keys states.
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_DPadUp)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_DPadDown)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_DPadLeft)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_DPadRight)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_StartButton)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_BackButton)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_LThumbClick)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_RThumbClick)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_LShoulder)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_RShoulder)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_AButton)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_BButton)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_XButton)] = true;
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_Y])
                        s_Buttons[0][static_cast<uint32_t>(KeyCode::Gamepad_YButton)] = true;

                    state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                    // Read all analog states.
                    s_Analogs[static_cast<uint32_t>(AnalogInput::LeftTrigger)] = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
                    s_Analogs[static_cast<uint32_t>(AnalogInput::RightTrigger)] = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
                    s_Analogs[static_cast<uint32_t>(AnalogInput::LeftStickX)] = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                    s_Analogs[static_cast<uint32_t>(AnalogInput::LeftStickY)] = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
                    s_Analogs[static_cast<uint32_t>(AnalogInput::RightStickX)] = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                    s_Analogs[static_cast<uint32_t>(AnalogInput::RightStickY)] = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                }

                // Update time duration for buttons pressed
                for (uint32_t i = 0; i < (uint32_t)KeyCode::NumDigitalInputs; ++i)
                {
                    if (s_Buttons[0][i])
                    {
                        if (!s_Buttons[1][i])
                            s_HoldDuration[i] = 0.0f;
                        else
                            s_HoldDuration[i] += frameDelta;
                    }
                }

                // Normalizes analog input by delta time.
                for (uint32_t i = 0; i < static_cast<uint32_t>(AnalogInput::kNumAnalogInputs); ++i)
                {
                    s_AnalogsTC[i] = s_Analogs[i] * frameDelta;
                }

                // Check if mouse is over screen.
                if (glfwGetWindowAttrib(g_window, GLFW_HOVERED))
                {
                    // Update mouse position.
                    double xpos, ypos;
                    glfwGetCursorPos(g_window, &xpos, &ypos);
                    NormalizeMousePosition(static_cast<float>(xpos), static_cast<float>(ypos), g_window);
                }
            }
        }

        void PostUpdate()
        {
            memcpy(s_Buttons[1], s_Buttons[0], sizeof(s_Buttons[0]));

            bool *dataPtr = &s_Buttons[0][static_cast<uint32_t>(KeyCode::NumKeys)];
            auto size = static_cast<uint32_t>(KeyCode::NumDigitalInputs) - static_cast<uint32_t>(KeyCode::NumKeys);

            // Cleans up buffers. Do not clean the keyboard keys.
            memset(dataPtr, 0, size * sizeof(bool));
            memset(s_Analogs, 0, sizeof(s_Analogs));
        }

        bool IsAnyPressed(void)
        {
            return s_Buttons[0] != 0;
        }

        bool IsPressed(KeyCode di)
        {
            return s_Buttons[0][static_cast<uint32_t>(di)];
        }

        bool IsFirstPressed(KeyCode di)
        {
            return s_Buttons[0][static_cast<uint32_t>(di)] && !s_Buttons[1][static_cast<uint32_t>(di)];
        }

        bool IsReleased(KeyCode di)
        {
            return !s_Buttons[0][static_cast<uint32_t>(di)];
        }

        bool IsFirstReleased(KeyCode di)
        {
            return !s_Buttons[0][static_cast<uint32_t>(di)] && s_Buttons[1][static_cast<uint32_t>(di)];
        }

        float GetDurationPressed(KeyCode di)
        {
            return s_HoldDuration[static_cast<uint32_t>(di)];
        }

        float GetAnalogInput(AnalogInput ai)
        {
            return s_Analogs[static_cast<uint32_t>(ai)];
        }

        float GetTimeCorrectedAnalogInput(AnalogInput ai)
        {
            return s_AnalogsTC[static_cast<uint32_t>(ai)];
        }

        void SetKey(int key, int scancode)
        {
            auto keyFind = g_windowToKeyMap.find(key);

            if (keyFind != g_windowToKeyMap.end())
            {
                auto targetKey = g_windowToKeyMap[key];

                switch (scancode)
                {
                case GLFW_PRESS:
                    s_Buttons[0][static_cast<uint32_t>(targetKey)] = true;
                    break;

                case GLFW_RELEASE:
                    s_Buttons[0][static_cast<uint32_t>(targetKey)] = false;
                    break;

                case GLFW_REPEAT:
                    break;

                default:
                    break;
                }
            }
        }

        void SetScrollValue(float scroll)
        {
            s_Analogs[static_cast<uint32_t>(AnalogInput::MouseScroll)] = scroll;
        }
    }
}