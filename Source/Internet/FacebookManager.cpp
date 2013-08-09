#include <functional>

#include <json/json.h>

#include <NinjaParty/FacebookManager.hpp>
#include <NinjaParty/HttpRequestManager.hpp>

extern "C"
{
	void FacebookLogin();
	void FacebookLogout();
}

//Expired token
//{
//	"error": {
//		"message": "Error validating access token: Session has expired at unix time 1375992000. The current unix time is 1375992383.",
//		"type": "OAuthException",
//		"code": 190,
//		"error_subcode": 463
//	}
//}

//Invalid token
//{
//	"error": {
//		"message": "Malformed access token CEose0cBANxVcP2bPKZAwP6rlXmv7YWHGq6hab8kTLTTvogKWGuZCZAWYqQaQjlWoExyBUZCk5wW5ZClkrQC5FSoOb78BpurRaK92idv35DRFycXoyLLlAyBQ288OOv2Co96qZA0bQihvnQZCbBZBtuMvnhbyZCv7ejpPwShpUwZDZD",
//		"type": "OAuthException",
//		"code": 190
//	}
//}

namespace NinjaParty
{
	FacebookManager::FacebookManager()
	{
		requestHandle = HttpRequestHandle();
		isLoggedIn = false;
	}
	
	FacebookManager::~FacebookManager()
	{
		HttpRequestManager::Instance()->Cancel(requestHandle);
	}

	void FacebookManager::Login()
	{
		FacebookLogin();
	}
	
	void FacebookManager::Logout()
	{
		FacebookLogout();
	}
	
	void FacebookManager::RequestInfo()
	{
		if(IsLoggedIn())
		{
			HttpRequestManager::Instance()->Cancel(requestHandle);
			
			std::string requestPath = "https://graph.facebook.com/me?fields=friends.fields(picture),picture&access_token=";
			requestHandle = HttpRequestManager::Instance()->PerformGet(requestPath + accessToken, std::bind(&FacebookManager::FB_InfoResponse, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
	
	void FacebookManager::FB_Login(bool success, const std::string &accessToken)
	{
		isLoggedIn = success;
		this->accessToken = accessToken;
	}
	
	void FacebookManager::FB_Logout()
	{
		isLoggedIn = false;
		this->accessToken = "";
	}
	
	void FacebookManager::FB_InfoResponse(bool success, const std::string &response)
	{
		std::lock_guard<std::mutex> lock(mutex);

		Json::Value root;
		Json::Reader reader;
		
		bool parsingSuccessful = reader.parse(reinterpret_cast<const char*>(response.c_str()), root);
		
		if(!parsingSuccessful)
		{
			// handle error
			return;
		}
		
		Json::Value error = root["error"];
		if(!error.isNull())
		{
			// handle error
			return;
		}
		
		userInfo.userId = root["id"].asString();
		userInfo.profilePictureUrl = root["picture"]["data"]["url"].asString();
		
		Json::Value friendData = root["friends"]["data"];
		friendsUserInfo.reserve(friendData.size());
		for(int index = 0; index < friendData.size(); ++index)
		{
			FacebookUserInfo info;
			info.userId = friendData[index]["id"].asString();
			info.profilePictureUrl = friendData[index]["picture"]["data"]["url"].asString();
			friendsUserInfo.push_back(info);
		}
	}
}
