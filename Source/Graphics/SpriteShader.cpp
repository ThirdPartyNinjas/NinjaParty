#include <map>
#include <stdio.h>
#include <stdexcept>
#include <stdlib.h>

#include <NinjaParty/FragmentShader.hpp>
#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/SpriteShader.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/Vertex.hpp>
#include <NinjaParty/VertexShader.hpp>

namespace NinjaParty
{
    struct UniformData
    {
        GLint location;
        GLenum type;
    };
    
    struct SpriteShader::impl
    {
        unsigned int programId;
        
        std::map<std::string, UniformData> uniforms;
    };
	
    SpriteShader::SpriteShader(VertexShader *vertexShader, FragmentShader *fragmentShader)
    : pimpl(new impl())
    {
        pimpl->programId = glCreateProgram();
        if(pimpl->programId == 0)
            throw std::runtime_error("Failed to create OpenGL shader program id");

		glAttachShader(pimpl->programId, vertexShader->GetShaderId());
		glAttachShader(pimpl->programId, fragmentShader->GetShaderId());
		
		glBindAttribLocation(pimpl->programId, (GLuint)ShaderAttributes::Position, "position");
		glBindAttribLocation(pimpl->programId, (GLuint)ShaderAttributes::TexCoord, "texcoord");
		glBindAttribLocation(pimpl->programId, (GLuint)ShaderAttributes::Color, "color");
		
		glLinkProgram(pimpl->programId);
        
        GLint status, logLength;
		
		glGetProgramiv(pimpl->programId, GL_LINK_STATUS, &status);
		if(status != GL_TRUE)
		{
			glGetProgramiv(pimpl->programId, GL_INFO_LOG_LENGTH, &logLength);
			if(logLength > 1)
			{
                std::unique_ptr<GLchar[]> log(new GLchar[logLength]);
                glGetShaderInfoLog(pimpl->programId, logLength, &logLength, log.get());
                printf("%s\n", log.get());
			}
			glDeleteProgram(pimpl->programId);
			throw std::runtime_error("Shader program linking failed");
		}
        
        GLint uniformCount;
        glGetProgramiv(pimpl->programId, GL_ACTIVE_UNIFORMS, &uniformCount);
        
        for(GLuint i=0; i<uniformCount; i++)
        {
            GLenum type;
            GLint size;
            GLchar name[256];
            glGetActiveUniform(pimpl->programId, i, 256, nullptr, &size, &type, name);
            
            pimpl->uniforms[name] = { glGetUniformLocation(pimpl->programId, name), type };
        }
        
        if(pimpl->uniforms.find("TextureSampler") == pimpl->uniforms.end() ||
           pimpl->uniforms.find("ProjectionMatrix") == pimpl->uniforms.end())
        {
			glDeleteProgram(pimpl->programId);
			throw std::runtime_error("Shader doesn't contain required uniforms (TextureSampler, ProjectionMatrix)");
        }
        
        if(pimpl->uniforms["TextureSampler"].type != GL_SAMPLER_2D)
            throw std::runtime_error("TextureSampler uniform is not GL_SAMPLER_2D");
        if(pimpl->uniforms["ProjectionMatrix"].type != GL_FLOAT_MAT4)
            throw std::runtime_error("TransformMatrix uniform is not GL_FLOAT_MAT4");
    }
    
	SpriteShader::~SpriteShader()
	{
		glDeleteProgram(pimpl->programId);
	}

    void SpriteShader::SetTexture(const std::string &parameterName, const Texture *texture, const int32_t &textureSlot)
    {
        SetTexture(parameterName, texture->GetTextureId(), textureSlot);
    }
    
    void SpriteShader::SetTexture(const std::string &parameterName, const int32_t &textureId, const int32_t &textureSlot)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_SAMPLER_2D)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(uniformData.location, textureSlot);
    }
	
    void SpriteShader::SetParameter(const std::string &parameterName, const int32_t &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_SAMPLER_2D && uniformData.type != GL_INT)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform1i(uniformData.location, value);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const float &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform1f(uniformData.location, value);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Vector2 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_VEC2)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform2fv(uniformData.location, 1, &value[0]);
    }

    void SpriteShader::SetParameter(const std::string &parameterName, const IntVector2 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_INT_VEC2)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform2iv(uniformData.location, 1, &value[0]);
    }

    void SpriteShader::SetParameter(const std::string &parameterName, const Vector3 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_VEC3)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform3fv(uniformData.location, 1, &value[0]);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Vector4 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_VEC4)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform4fv(uniformData.location, 1, &value[0]);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Matrix2 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_MAT2)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniformMatrix2fv(uniformData.location, 1, GL_FALSE, value.Transpose());
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Matrix3 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_MAT3)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniformMatrix3fv(uniformData.location, 1, GL_FALSE, value.Transpose());
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Matrix4 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_MAT4)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniformMatrix4fv(uniformData.location, 1, GL_FALSE, value.Transpose());
    }
    
    void SpriteShader::Apply()
    {
        glUseProgram(pimpl->programId);
    }
}
