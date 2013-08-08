#include <string>
#include <vector>

#include <stb_image.h>
#include <jpgd.h>
#include <resampler.h>

#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/LoadTexture.hpp>

namespace NinjaParty
{
	static unsigned char* Resample(unsigned char *data, int inputWidth, int inputHeight, int outputWidth, int outputHeight)
	{
		// one resampler per channel
		Resampler *resamplers[4];
		// one sample buffer per channel
		std::vector<float> samples[4];
		
		// Now create a Resampler instance for each component to process. The first instance will create new contributor tables, which are shared by the resamplers
		// used for the other components (a memory and slight cache efficiency optimization).
		resamplers[0] = new Resampler(inputWidth, inputHeight, outputWidth, outputHeight, Resampler::BOUNDARY_CLAMP, 0.0f, 0.0f, RESAMPLER_DEFAULT_FILTER, NULL, NULL);
		samples[0].resize(inputWidth);
		for (int i = 1; i < 4; i++)
		{
			resamplers[i] = new Resampler(inputWidth, inputHeight, outputWidth, outputHeight, Resampler::BOUNDARY_CLAMP, 0.0f, 0.0f, RESAMPLER_DEFAULT_FILTER, resamplers[0]->get_clist_x(), resamplers[0]->get_clist_y());
			samples[i].resize(inputWidth);
		}
		
		unsigned char *outputImage = new unsigned char[outputWidth * outputHeight * 4];
		
		const int inputPitch = inputWidth * 4;
		const int outputPitch = outputWidth * 4;
		int outputY = 0;
		
		for(int inputY = 0; inputY < inputHeight; inputY++)
		{
			const unsigned char* pSource = &data[inputY * inputPitch];
			
			for(int x = 0; x < inputWidth; x++)
			{
				for(int c = 0; c < 4; c++)
				{
					samples[c][x] = *pSource++ / 255.0f;
				}
			}
			
			for(int c = 0; c < 4; c++)
			{
				if(!resamplers[c]->put_line(&samples[c][0]))
				{
					// todo: log error
					delete[] outputImage;
					return nullptr;
				}
			}
			
			while(true)
			{
				int componentIndex;
				for(componentIndex = 0; componentIndex < 4; componentIndex++)
				{
					const float* pOutputSamples = resamplers[componentIndex]->get_line();
					if(!pOutputSamples)
						break;
					
					unsigned char* pDestination = &outputImage[outputY * outputPitch + componentIndex];
					
					for(int x = 0; x < outputWidth; x++)
					{
						int c = (int)(255.0f * pOutputSamples[x] + .5f);
						if (c < 0) c = 0; else if (c > 255) c = 255;
						*pDestination = (unsigned char)c;
						
						pDestination += 4;
					}
				}
				if (componentIndex < 4)
					break;
				
				outputY++;
			}
		}
		return outputImage;
	}
	
	int LoadTexture(const std::string &fileName, int &width, int &height, bool resample)
	{
		GLuint textureId;
		
		int w, h, bpp;
		
		unsigned char *data = stbi_load(fileName.c_str(), &w, &h, &bpp, 4);
		
		if(data == nullptr)
			data = jpgd::decompress_jpeg_image_from_file(fileName.c_str(), &w, &h, &bpp, 4);
		
		if(data == nullptr)
			return 0;

		unsigned char *resampledData = nullptr;
		if(resample && (w != width || h != height))
		{
			resampledData = Resample(data, w, h, width, height);
		}
		else
		{
			width = w;
			height = h;
		}
		
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (resampledData != nullptr) ? resampledData : data);
		
		stbi_image_free(data);
		delete[] resampledData;
		
		return textureId;
	}
	
	int LoadTextureFromBuffer(const unsigned char *buffer, int length, int &width, int &height, bool resample)
	{
		GLuint textureId;
		
		int w, h, bpp;
		
		unsigned char *data = stbi_load_from_memory(buffer, length, &w, &h, &bpp, 4);
		
		if(data == nullptr)
			data = jpgd::decompress_jpeg_image_from_memory(buffer, length, &w, &h, &bpp, 4);

		if(data == nullptr)
			return 0;
		
		unsigned char *resampledData = nullptr;
		if(resample && (w != width || h != height))
		{
			resampledData = Resample(data, w, h, width, height);
		}
		else
		{
			width = w;
			height = h;
		}
		
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (resampledData != nullptr) ? resampledData : data);
		
		stbi_image_free(data);
		delete[] resampledData;
		
		return textureId;
	}
}
