#ifndef NINJAPARTY_VERTEXSHADER_HPP
#define NINJAPARTY_VERTEXSHADER_HPP

#include <memory>

namespace NinjaParty
{
    class VertexShader
    {
    public:
        ~VertexShader();
        
		static VertexShader* FromFile(const std::string &fileName);
		static VertexShader* FromBuffer(const unsigned char *buffer, int length);
        
        unsigned int GetShaderId() const;
        
    private:
        VertexShader();
        
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAPARTY_VERTEXSHADER_HPP
