#include <functional>

#include <NinjaParty/FacebookManager.hpp>
#include <NinjaParty/HttpRequestManager.hpp>

extern "C"
{
	void FacebookLogin();
	void FacebookLogout();
	bool FacebookIsLoggedIn();
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
	}
	
	FacebookManager::~FacebookManager()
	{
		
	}

	bool FacebookManager::IsLoggedIn() const
	{
		return FacebookIsLoggedIn();
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
		this->accessToken = accessToken;
	}
	
	void FacebookManager::FB_Logout()
	{
		this->accessToken = "";
	}
	
	void FacebookManager::FB_InfoResponse(bool success, const std::string &response)
	{
		std::lock_guard<std::mutex> lock(mutex);
		
		// parse the response
		printf("%s\n", response.c_str());
	}
}
