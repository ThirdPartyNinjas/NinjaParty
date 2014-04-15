#ifndef NINJAPARTY_FRAGMENTSHADER_HPP
#define NINJAPARTY_FRAGMENTSHADER_HPP

#include <memory>

namespace NinjaParty
{
    class FragmentShader
    {
    public:
        ~FragmentShader();
        
		static FragmentShader* FromFile(const std::string &fileName);
		static FragmentShader* FromBuffer(const unsigned char *buffer, int length);

        unsigned int GetShaderId() const;
        
    private:
        FragmentShader();
        
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAPARTY_FRAGMENTSHADER_HPP
