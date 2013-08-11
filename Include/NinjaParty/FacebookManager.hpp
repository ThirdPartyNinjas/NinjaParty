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

		// todo:
		// QueryPermissions (using graph api me/permissions)
		// RequestPublishPermission (require calling up iOS/Android API)
		
		void RequestInfo();

		void InjectLogin(bool success, const std::string &accessToken);
		void InjectLogout();
		
	private:
		void FB_InfoResponse(bool success, const std::string &response);
		
		std::string accessToken;
		
		bool isLoggedIn;
		
		std::mutex mutex;
		HttpRequestHandle requestHandle;
		
		FacebookUserInfo userInfo;
		std::vector<FacebookUserInfo> friendsUserInfo;
	};
}

#endif//NINJAPARTY_FACEBOOKMANAGER_HPP
