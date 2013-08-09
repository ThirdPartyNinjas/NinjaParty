#ifndef NINJAPARTY_FACEBOOKMANAGER_HPP
#define NINJAPARTY_FACEBOOKMANAGER_HPP

#include <mutex>
#include <string>
#include <vector>

#include <NinjaParty/HttpRequestManager.hpp>

namespace NinjaParty
{
	struct FacebookUserInfo
	{
		std::string userId;
		std::string profilePictureUrl;
	};

	class FacebookManager
	{
	public:
		FacebookManager();
		~FacebookManager();
		
		bool IsLoggedIn() const { return isLoggedIn; }
		
		void Login();
		void Logout();
		
		void RequestInfo();

	private:
		void FB_Login(bool success, const std::string &accessToken);
		void FB_Logout();
		
		void FB_InfoResponse(bool success, const std::string &response);
		
		std::string accessToken;
		
		bool isLoggedIn;
		
		std::mutex mutex;
		HttpRequestHandle requestHandle;
		
		FacebookUserInfo userInfo;
		std::vector<FacebookUserInfo> friendsUserInfo;
		
		friend class Game;
	};
}

#endif//NINJAPARTY_FACEBOOKMANAGER_HPP
