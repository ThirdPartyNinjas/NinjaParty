#include <cstdio>
#include <string>

#include <NinjaParty/VertexShader.hpp>
#include <NinjaParty/IncludeGL.h>

namespace NinjaParty
{
    struct VertexShader::impl
    {
        unsigned int shaderId = 0;
    };
    
    VertexShader::VertexShader()
    : pimpl(new impl())
    {
    }
    
    VertexShader::~VertexShader()
    {
        if(pimpl->shaderId != 0)
            glDeleteShader(pimpl->shaderId);
    }
    
    VertexShader* VertexShader::FromFile(const std::string &fileName)
    {
        std::FILE *file = std::fopen(fileName.c_str(), "rb");
        if(!file)
            throw std::runtime_error(std::string("Failed to load vertex shader: ") + fileName);
        
        std::fseek(file, 0, SEEK_END);
        int fileLength = std::ftell(file);
        std::rewind(file);
        
        std::unique_ptr<unsigned char[]> buffer(new unsigned char[fileLength]);
        std::fread(buffer.get(), 1, fileLength, file);
        
        return FromBuffer(buffer.get(), fileLength);
    }
    
    VertexShader* VertexShader::FromBuffer(const unsigned char *buffer, int length)
    {
        unsigned int shaderId = glCreateShader(GL_VERTEX_SHADER);
        if(shaderId == 0)
            throw std::runtime_error("Failed to create OpenGL vertex shader id");
        
        VertexShader *vertexShader = new VertexShader();
        vertexShader->pimpl->shaderId = shaderId;
        
        GLint bufferLengths[] = { length };
        GLchar *buffers[] = { (GLchar*)buffer };
        
        glShaderSource(vertexShader->pimpl->shaderId, 1, buffers, bufferLengths);
        glCompileShader(vertexShader->pimpl->shaderId);
        
        GLint status, logLength;
        
        glGetShaderiv(vertexShader->pimpl->shaderId, GL_COMPILE_STATUS, &status);
        if(status != GL_TRUE)
        {
            glGetShaderiv(vertexShader->pimpl->shaderId, GL_INFO_LOG_LENGTH, &logLength);
            if(logLength > 1)
            {
                std::unique_ptr<GLchar[]> log(new GLchar[logLength]);
                glGetShaderInfoLog(vertexShader->pimpl->shaderId, logLength, &logLength, log.get());
                printf("%s\n", log.get());
            }
            glDeleteShader(vertexShader->pimpl->shaderId);
            throw std::runtime_error("Vertex shader compile failed");
        }
        
        return vertexShader;
    }
    
    unsigned int VertexShader::GetShaderId() const
    {
        return pimpl->shaderId;
    }
}