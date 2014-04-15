#include <cstdio>
#include <string>

#include <NinjaParty/FragmentShader.hpp>
#include <NinjaParty/IncludeGL.h>

namespace NinjaParty
{
    struct FragmentShader::impl
    {
        unsigned int shaderId = 0;
    };
    
    FragmentShader::FragmentShader()
    : pimpl(new impl())
    {
    }
    
    FragmentShader::~FragmentShader()
    {
        if(pimpl->shaderId != 0)
            glDeleteShader(pimpl->shaderId);
    }

    FragmentShader* FragmentShader::FromFile(const std::string &fileName)
    {
        std::FILE *file = std::fopen(fileName.c_str(), "rb");
        if(!file)
            throw std::runtime_error(std::string("Failed to load fragment shader: ") + fileName);
        
        std::fseek(file, 0, SEEK_END);
        int fileLength = std::ftell(file);
        std::rewind(file);

        std::unique_ptr<unsigned char[]> buffer(new unsigned char[fileLength]);
        std::fread(buffer.get(), 1, fileLength, file);
        
        return FromBuffer(buffer.get(), fileLength);
    }
    
    FragmentShader* FragmentShader::FromBuffer(const unsigned char *buffer, int length)
    {
        unsigned int shaderId = glCreateShader(GL_FRAGMENT_SHADER);
        if(shaderId == 0)
            throw std::runtime_error("Failed to create OpenGL fragment shader id");
        
        FragmentShader *fragmentShader = new FragmentShader();
        fragmentShader->pimpl->shaderId = shaderId;
        
        GLint bufferLengths[] = { length };
        GLchar *buffers[] = { (GLchar*)buffer };
        
        glShaderSource(fragmentShader->pimpl->shaderId, 1, buffers, bufferLengths);
        glCompileShader(fragmentShader->pimpl->shaderId);

        GLint status, logLength;
        
        glGetShaderiv(fragmentShader->pimpl->shaderId, GL_COMPILE_STATUS, &status);
        if(status != GL_TRUE)
        {
            glGetShaderiv(fragmentShader->pimpl->shaderId, GL_INFO_LOG_LENGTH, &logLength);
            if(logLength > 1)
            {
                std::unique_ptr<GLchar[]> log(new GLchar[logLength]);
                glGetShaderInfoLog(fragmentShader->pimpl->shaderId, logLength, &logLength, log.get());
                printf("%s\n", log.get());
            }
            glDeleteShader(fragmentShader->pimpl->shaderId);
            throw std::runtime_error("Fragment shader compile failed");
        }
        
        return fragmentShader;
    }

    unsigned int FragmentShader::GetShaderId() const
    {
        return pimpl->shaderId;
    }
}