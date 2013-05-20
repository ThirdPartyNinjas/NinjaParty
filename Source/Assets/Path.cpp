#include <NinjaParty/Path.hpp>

#include <string>
#include <vector>

namespace NinjaParty
{
	namespace Path
	{
		// This code comes from a stackoverflow.com answer by Mr.Ree
		// http://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c/325000#325000
		void split(std::vector<std::string>& output, const std::string &input, const std::string &delimiter)
		{
			size_t start = 0, end = 0;
			
			while (end != std::string::npos)
			{
				end = input.find(delimiter, start);
				
				// if at end, use length = maxLength, else use length = end - start.
				std::string s = input.substr(start, (end == std::string::npos) ? std::string::npos : end - start);
				if(s != "")
					output.push_back(s);
				
				// if at end, use start = maxSize, else use start = end + delimiter.
				start = ((end > (std::string::npos - delimiter.size())) ? std::string::npos : end + delimiter.size());
			}
		}
	
		static std::vector<std::string> split(const std::string &input, const std::string &delimiter)
		{
			std::vector<std::string> output;
			split(output, input, delimiter);
			return output;
		}

		std::string CombinePaths(const std::string &firstPath, const std::string &secondPath)
		{
			std::vector<std::string> splitPaths = split(firstPath, "/");
			split(splitPaths, secondPath, "/");
			
			for(int i=static_cast<int>(splitPaths.size()) - 1; i >= 0; i--)
			{
				if(splitPaths[i] == "..")
				{
					splitPaths.erase(splitPaths.begin() + i);
					
					if(i > 0)
					{
						splitPaths.erase(splitPaths.begin() + i - 1);
						i--;
					}
				}
			}
			
			std::string combinedPaths;
			
			int splitCount = static_cast<int>(splitPaths.size());
			for(int i=0; i<splitCount - 1; i++)
			{
				combinedPaths += splitPaths[i];
				combinedPaths += "/";
			}
			combinedPaths += splitPaths[splitPaths.size() - 1];
			
			return combinedPaths;
		}
		
		std::string GetPathName(const std::string &filePath)
		{
			std::string directoryName = "";
			std::vector<std::string> splitPath = split(filePath, "/");
			
			int splitCount = static_cast<int>(splitPath.size());
			for(int i=0; i<splitCount - 1; i++)
			{
				directoryName += splitPath[i];
				directoryName += "/";
			}

			return directoryName;
		}
	}
}