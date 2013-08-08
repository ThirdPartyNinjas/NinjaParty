#include <stdio.h>
#include <stdexcept>
#include <stdlib.h>

#include <NinjaParty/SpriteShader.hpp>
//#include <NinjaParty/Texture.hpp>
#include <NinjaParty/Vertex.hpp>

#undef far
#undef near

namespace NinjaParty
{
#if defined _WIN32 || defined _WIN64
	static const char vertexShaderSource[] =
	"#version 120\n"
	"attribute vec4 position;\n"
	"attribute vec4 color;\n"
	"attribute vec2 texcoord;\n"
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * position;\n"
	"	v_color = color;\n"
	"	v_texcoord = texcoord;\n"
	"}\n";
	
	static const char fragmentShaderSource[] =
	"#version 120\n"
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
	"uniform sampler2D sampler;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(sampler, v_texcoord) * v_color;\n"
	"}\n";

#elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	static const char vertexShaderSource[] =
	"attribute vec4 position;\n"
	"attribute vec4 color;\n"
	"attribute vec2 texcoord;\n"
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * position;\n"
	"	v_color = color;\n"
	"	v_texcoord = texcoord;\n"
	"}\n";
	
	static const char fragmentShaderSource[] =
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
	"uniform sampler2D sampler;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(sampler, v_texcoord) * v_color;\n"
	"}\n";
	
#elif TARGET_OS_MAC
	static const char vertexShaderSource[] =
	"#version 120\n"
	"attribute vec4 position;\n"
	"attribute vec4 color;\n"
	"attribute vec2 texcoord;\n"
	"varying vec4 v_color;\n"
	"varying vec2 v_texcoord;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * position;\n"
	"	v_color = color;\n"
	"	v_texcoord = texcoord;\n"
	"}\n";

	static const char fragmentShaderSource[] =
	"#version 120\n"
	"varying vec4 v_color;\n"
	"varying vec2 v_texcoord;\n"
	"uniform sampler2D sampler;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(sampler, v_texcoord) * v_color;\n"
	"}\n";

#elif __ANDROID__
	static const char vertexShaderSource[] =
	"attribute vec4 position;\n"
	"attribute vec4 color;\n"
	"attribute vec2 texcoord;\n"
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * position;\n"
	"	v_color = color;\n"
	"	v_texcoord = texcoord;\n"
	"}\n";

	static const char fragmentShaderSource[] =
	"varying highp vec4 v_color;\n"
	"varying highp vec2 v_texcoord;\n"
	"uniform sampler2D sampler;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(sampler, v_texcoord) * v_color;\n"
	"}\n";

#endif
	
	SpriteShader::SpriteShader()
	{
		unsigned int vertexShader, fragmentShader;

		const GLchar *vss = vertexShaderSource;		
		const GLchar *fss = fragmentShaderSource;
		
		GLint logLength, status;

		shaderId = glCreateProgram();
	
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
			glDeleteProgram(shaderId);
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
			glDeleteProgram(shaderId);
			throw std::runtime_error("Fragment shader compile failed");
		}
		
		glAttachShader(shaderId, vertexShader);
		glAttachShader(shaderId, fragmentShader);
		
		glBindAttribLocation(shaderId, (GLuint)ShaderAttributes::Position, "position");
		glBindAttribLocation(shaderId, (GLuint)ShaderAttributes::TexCoord, "texcoord");
		glBindAttribLocation(shaderId, (GLuint)ShaderAttributes::Color, "color");
		
		glLinkProgram(shaderId);
		
		glGetShaderiv(shaderId, GL_LINK_STATUS, &status);
		if(status != GL_TRUE)
		{
			glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
			if(logLength > 1)
			{
				GLchar *log = (GLchar*)malloc(logLength);
				glGetProgramInfoLog(shaderId, logLength, &logLength, log);
				printf("%s\n", log);
				free(log);
			}
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(shaderId);
			throw std::runtime_error("Shader linking failed");
		}
		
		uniforms[(int)ShaderUniforms::Projection] = glGetUniformLocation(shaderId, "projection");
		uniforms[(int)ShaderUniforms::Sampler] = glGetUniformLocation(shaderId, "sampler");
		
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}
	
	SpriteShader::SpriteShader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
	{
		unsigned int vertexShader, fragmentShader;
		FILE *file;
		int fileLength;
		
		GLchar *buffer;
		GLint logLength;
		
		shaderId = glCreateProgram();	

		file = fopen(vertexShaderFile.c_str(), "r");
		if(!file)
			throw std::runtime_error("Failed to load vertex shader");
		
		fseek(file, 0, SEEK_END);
		fileLength = static_cast<int>(ftell(file));
		fseek(file, 0, SEEK_SET);
		buffer = new char[fileLength + 1];
		fread(buffer, 1, fileLength, file);
		buffer[fileLength] = '\0';
		fclose(file);

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		glShaderSource(vertexShader, 1, const_cast<const GLchar**>(&buffer), nullptr);
		glCompileShader(vertexShader);
		delete[] buffer;
		
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0)
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
			printf("%s\n", log);
			free(log);
			glDeleteShader(vertexShader);
			glDeleteProgram(shaderId);
			throw std::runtime_error("Vertex shader compile failed");
		}

		file = fopen(fragmentShaderFile.c_str(), "r");
		if(!file)
			throw std::runtime_error("Failed to load fragment shader");
		
		fseek(file, 0, SEEK_END);
		fileLength = static_cast<int>(ftell(file));
		fseek(file, 0, SEEK_SET);
		buffer = new char[fileLength + 1];
		fread(buffer, 1, fileLength, file);
		buffer[fileLength] = '\0';
		fclose(file);

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		glShaderSource(fragmentShader, 1, const_cast<const GLchar**>(&buffer), nullptr);
		glCompileShader(fragmentShader);
		delete[] buffer;
		
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0)
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetShaderInfoLog(fragmentShader, logLength, &logLength, log);
			printf("%s\n", log);
			free(log);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(shaderId);
			throw std::runtime_error("Vertex shader compile failed");
		}
		
		glAttachShader(shaderId, vertexShader);
		glAttachShader(shaderId, fragmentShader);
		
		glBindAttribLocation(shaderId, (GLuint)ShaderAttributes::Position, "position");
		glBindAttribLocation(shaderId, (GLuint)ShaderAttributes::TexCoord, "texcoord");
		glBindAttribLocation(shaderId, (GLuint)ShaderAttributes::Color, "color");
		
		glLinkProgram(shaderId);
		
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0)
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetProgramInfoLog(shaderId, logLength, &logLength, log);
			printf("%s\n", log);
			free(log);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(shaderId);
			throw std::runtime_error("Shader linking failed");
		}
		
		uniforms[(int)ShaderUniforms::Projection] = glGetUniformLocation(shaderId, "projection");
		uniforms[(int)ShaderUniforms::Sampler] = glGetUniformLocation(shaderId, "sampler");
		
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}
	
	void SpriteShader::SetOrthoMatrix(int width, int height)
	{
		float left = 0;
		float right = (float)width;
		float top = 0;
		float bottom = (float)height;
		float far = -1;
		float near = 1;
		
		projectionMatrix[ 0] = 2 / (right - left);
		projectionMatrix[ 4] = 0;
		projectionMatrix[ 8] = 0;
		projectionMatrix[ 12] = -(right + left) / (right - left);
		
		projectionMatrix[ 1] = 0;
		projectionMatrix[ 5] = 2 / (top - bottom);
		projectionMatrix[ 9] = 0;
		projectionMatrix[ 13] = -(top + bottom) / (top - bottom);
		
		projectionMatrix[ 2] = 0;
		projectionMatrix[ 6] = 0;
		projectionMatrix[10] = -2 / (far - near);
		projectionMatrix[14] = -(far + near) / (far - near);
		
		projectionMatrix[ 3] = 0;
		projectionMatrix[ 7] = 0;
		projectionMatrix[11] = 0;
		projectionMatrix[15] = 1;	
	}
	
	SpriteShader::~SpriteShader()
	{
		glDeleteProgram(shaderId);
	}
	
	void SpriteShader::Draw(Vertex *vertices, int vertexCount, GLuint textureId)
	{
		glUseProgram(shaderId);
		
		glUniformMatrix4fv(uniforms[(int)ShaderUniforms::Projection], 1, 0, projectionMatrix);
		glUniform1i(uniforms[(int)ShaderUniforms::Sampler], 0);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		
		glVertexAttribPointer((GLuint)ShaderAttributes::Position, 2, GL_FLOAT, 0, sizeof(Vertex), &vertices[0].x);
		glEnableVertexAttribArray((GLuint)ShaderAttributes::Position);
		glVertexAttribPointer((GLuint)ShaderAttributes::TexCoord, 2, GL_FLOAT, 0, sizeof(Vertex), &vertices[0].u);
		glEnableVertexAttribArray((GLuint)ShaderAttributes::TexCoord);
		glVertexAttribPointer((GLuint)ShaderAttributes::Color, 4, GL_FLOAT, 0, sizeof(Vertex), &vertices[0].r);
		glEnableVertexAttribArray((GLuint)ShaderAttributes::Color);
		
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
}
