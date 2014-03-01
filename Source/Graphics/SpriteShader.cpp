#include <map>
#include <stdio.h>
#include <stdexcept>
#include <stdlib.h>

#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/SpriteShader.hpp>
#include <NinjaParty/Vertex.hpp>

namespace NinjaParty
{
#if defined TARGET_OS_IPHONE || defined TARGET_IPHONE_SIMULATOR
	static const char vertexShaderSource[] =
    // input from CPU
	"attribute vec4 position;\n"
	"attribute vec4 color;\n"
	"attribute vec2 texcoord;\n"
    // output to fragment shader
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
    // custom input from program
	"uniform mat4 ProjectionMatrix;\n"
    //
	"void main()\n"
	"{\n"
	"	gl_Position = ProjectionMatrix * position;\n"
	"	v_color = color;\n"
	"	v_texcoord = texcoord;\n"
	"}\n";
	
	static const char fragmentShaderSource[] =
    // input from vertex shader
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
    // custom input from program
	"uniform sampler2D TextureSampler;\n"
    //
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(TextureSampler, v_texcoord) * v_color;\n"
	"}\n";
#endif

    struct UniformData
    {
        GLuint id;
        GLenum type;
    };
    
    struct SpriteShader::impl
    {
        unsigned int programId;
        
        std::map<std::string, UniformData> uniforms;
    };
	
	SpriteShader::SpriteShader()
    : pimpl(new impl)
	{
		unsigned int vertexShader, fragmentShader;

		const GLchar *vss = vertexShaderSource;		
		const GLchar *fss = fragmentShaderSource;
		
		GLint logLength, status;

		pimpl->programId = glCreateProgram();
	
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		glShaderSource(vertexShader, 1, &vss, nullptr);
		glCompileShader(vertexShader);
		
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
		{
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
			if(logLength > 1)
			{
				GLchar *log = (GLchar*)malloc(logLength);
				glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
				printf("%s\n", log);
				free(log);
			}
			glDeleteShader(vertexShader);
			glDeleteProgram(pimpl->programId);
			throw std::runtime_error("Vertex shader compile failed");
		}
		
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		glShaderSource(fragmentShader, 1, const_cast<const GLchar**>(&fss), nullptr);
		glCompileShader(fragmentShader);
		
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
		{
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
			if(logLength > 1)
			{
				GLchar *log = (GLchar*)malloc(logLength);
				glGetShaderInfoLog(fragmentShader, logLength, &logLength, log);
				printf("%s\n", log);
				free(log);
			}
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(pimpl->programId);
			throw std::runtime_error("Fragment shader compile failed");
		}
		
		glAttachShader(pimpl->programId, vertexShader);
		glAttachShader(pimpl->programId, fragmentShader);
		
		glBindAttribLocation(pimpl->programId, (GLuint)ShaderAttributes::Position, "position");
		glBindAttribLocation(pimpl->programId, (GLuint)ShaderAttributes::TexCoord, "texcoord");
		glBindAttribLocation(pimpl->programId, (GLuint)ShaderAttributes::Color, "color");
		
		glLinkProgram(pimpl->programId);
		
		glGetShaderiv(pimpl->programId, GL_LINK_STATUS, &status);
		if(status != GL_TRUE)
		{
			glGetProgramiv(pimpl->programId, GL_INFO_LOG_LENGTH, &logLength);
			if(logLength > 1)
			{
				GLchar *log = (GLchar*)malloc(logLength);
				glGetProgramInfoLog(pimpl->programId, logLength, &logLength, log);
				printf("%s\n", log);
				free(log);
			}
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(pimpl->programId);
			throw std::runtime_error("Shader linking failed");
		}
        
        GLint uniformCount;
        glGetProgramiv(pimpl->programId, GL_ACTIVE_UNIFORMS, &uniformCount);
        
        for(GLuint i=0; i<uniformCount; i++)
        {
            GLenum type;
            GLint size;
            GLchar name[256];
            glGetActiveUniform(pimpl->programId, i, 256, nullptr, &size, &type, name);
            
            pimpl->uniforms[name] = { i, type };
        }
        
        if(pimpl->uniforms.find("TextureSampler") == pimpl->uniforms.end() ||
           pimpl->uniforms.find("ProjectionMatrix") == pimpl->uniforms.end())
        {
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(pimpl->programId);
			throw std::runtime_error("Shader doesn't contain required uniforms (TextureSampler, ProjectionMatrix)");
        }
        
        if(pimpl->uniforms["TextureSampler"].type != GL_SAMPLER_2D)
            throw std::runtime_error("TextureSampler uniform is not GL_SAMPLER_2D");
        if(pimpl->uniforms["ProjectionMatrix"].type != GL_FLOAT_MAT4)
            throw std::runtime_error("TransformMatrix uniform is not GL_FLOAT_MAT4");
		
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}
    
	SpriteShader::~SpriteShader()
	{
		glDeleteProgram(pimpl->programId);
	}
	
    void SpriteShader::SetParameter(const std::string &parameterName, const int32_t &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_SAMPLER_2D && uniformData.type != GL_INT)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform1i(uniformData.id, value);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const float &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform1f(uniformData.id, value);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Vector2 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_VEC2)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform2fv(uniformData.id, 1, &value[0]);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Vector3 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_VEC3)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform3fv(uniformData.id, 1, &value[0]);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Vector4 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_VEC4)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniform4fv(uniformData.id, 1, &value[0]);
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Matrix2 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_MAT2)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniformMatrix2fv(uniformData.id, 1, GL_FALSE, value.Transpose());
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Matrix3 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_MAT3)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniformMatrix3fv(uniformData.id, 1, GL_FALSE, value.Transpose());
    }
    
    void SpriteShader::SetParameter(const std::string &parameterName, const Matrix4 &value)
    {
        if(pimpl->uniforms.find(parameterName) == pimpl->uniforms.end())
            throw std::runtime_error("Couldn't find the named SpriteEffect parameter: " + parameterName);
        
        auto &uniformData = pimpl->uniforms[parameterName];
        
        if(uniformData.type != GL_FLOAT_MAT4)
            throw std::runtime_error("Type mismatch for named SpriteEffect paramater: " + parameterName);
        
        glUniformMatrix4fv(uniformData.id, 1, GL_FALSE, value.Transpose());
    }
    
    void SpriteShader::Apply()
    {
        glUseProgram(pimpl->programId);
    }
}
