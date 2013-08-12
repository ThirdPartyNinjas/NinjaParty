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
		
		// Unspecified error
		virtual void HandleGenericError() { }
		// Retry after app-defined delay
		virtual void HandleDelayRetryError() { }
		// Reauthorize user, access token invalid or expired
		virtual void HandleReauthorizeError() { }
		// Reauthorize user, unless on iOS and version >= 6.0, then send to iOS settings to change password
		virtual void HandleReauthorizeSpecialError() { }
		// Send user to login to the facebook website http://m.facebook.com
		virtual void HandleFacebookWebsiteLoginError() { }
		// We lack the appropriate permissions
		virtual void HandleNeedPermissionsError() { }
		
		std::string accessToken;
		
		bool isLoggedIn;
		
		std::mutex mutex;
		HttpRequestHandle requestHandle;
		
		FacebookUserInfo userInfo;
		std::vector<FacebookUserInfo> friendsUserInfo;
	};
}

#endif//NINJAPARTY_FACEBOOKMANAGER_HPP
