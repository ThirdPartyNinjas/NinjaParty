#ifndef NINJAPARTY_HTTPREQUESTMANAGER_HPP
#define NINJAPARTY_HTTPREQUESTMANAGER_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <list>
#include <thread>
#include <vector>

namespace NinjaParty
{
	typedef int HttpRequestHandle;
	typedef std::function<void (bool success, const std::string &response)> HttpCallback;
	
	class HttpRequestManager
	{
	public:
		static HttpRequestManager& Instance()
		{
			static HttpRequestManager instance;
			return instance;
		}
		
		~HttpRequestManager();

		HttpRequestHandle PerformGet(const std::string &requestUrl, HttpCallback callback);
		// todo: PerformPost ? (string url, map<string,string> arguments, HttpCallback callback)

		void Cancel(HttpRequestHandle requestHandle);
		
	private:
		HttpRequestManager();

		void ThreadFunction();
		
		typedef std::tuple<std::string, HttpRequestHandle, HttpCallback> RequestInfo;
		std::list<RequestInfo> waitingRequests;
		std::list<RequestInfo> activeRequests;
		
		std::condition_variable conditionVariable;
		std::mutex mutex;
		std::mutex cancelMutex;
		std::vector<std::thread> threads;
		bool exit;
		
		HttpRequestHandle currentRequestHandle;
		
		static HttpRequestManager *instance;
		
		static const int MaxiumumConnections = 5;
	};
}

#endif//NINJAPARTY_HTTPREQUESTMANAGER_HPP
