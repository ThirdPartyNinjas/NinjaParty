#ifndef NINJAPARTY_KEYBOARD_HPP
#define NINJAPARTY_KEYBOARD_HPP

namespace NinjaParty
{
    enum class KeyState
    {
        Released = 0,
        Pressed,
    };
    
    enum class Key
    {
        Up = 0,
        Down,
        Left,
        Right,
        Space,
        
        KeyCount // Must always come last
    };
    
    struct KeyboardState
    {
        KeyboardState()
        {
            for(auto &i : keys)
                i = KeyState::Released;
        }
        
        bool IsKeyDown(Key key) const { return keys[(int)key] == KeyState::Pressed; }
        bool IsKeyUp(Key key) const { return keys[(int)key] == KeyState::Released; }
        
        KeyState keys[(int)Key::KeyCount];
    };
}

#endif//NINJAPARTY_KEYBOARD_HPP
