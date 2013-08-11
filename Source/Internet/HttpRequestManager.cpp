#include <algorithm>
#include <sstream>

#include <curl/curl.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>

#include <NinjaParty/HttpRequestManager.hpp>

#include "Cacerts.hpp"

namespace NinjaParty
{
	// Global mutexes for openssl
	static std::mutex *g_sslMutexes = nullptr;

	static CURLcode ssl_ctx_function(CURL *curl, void *sslctx, void *parm)
	{
		X509_STORE *store;
		X509 *cert = nullptr;
		BIO *bio;
		
		bio = BIO_new_mem_buf(const_cast<char*>(NinjaParty::Cacerts), -1);
		while((cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr)) != nullptr)
		{
			store = SSL_CTX_get_cert_store(static_cast<SSL_CTX*>(sslctx));

			if(X509_STORE_add_cert(store, cert) == 0)
			{
				// todo: log error adding certificate
				BIO_free(bio);
				return CURLE_SSL_CACERT;
			}
		}
		
		BIO_free(bio);
		return CURLE_OK;
	}
	
	static void crypto_threadid_callback(CRYPTO_THREADID *threadId)
	{
		std::ostringstream oss;
		oss << std::this_thread::get_id();
		std::hash<std::string> hash;
		CRYPTO_THREADID_set_numeric(threadId, static_cast<unsigned long>(hash(oss.str())));
	}
	
	static void crypto_locking_callback(int mode, int n, const char *file, int line)
	{
		if(mode & CRYPTO_LOCK)
			g_sslMutexes[n].lock();
		else
			g_sslMutexes[n].unlock();
	}
	
	HttpRequestManager::HttpRequestManager()
	{
		if(g_sslMutexes != nullptr)
			throw std::runtime_error("Multiple HttpRequestManagers not currently supported");
		
		currentRequestHandle = 1;
		exit = false;

		curl_global_init(CURL_GLOBAL_ALL);
		CRYPTO_THREADID_set_callback(&crypto_threadid_callback);
		CRYPTO_set_locking_callback(&crypto_locking_callback);

		int locks = CRYPTO_num_locks();
		g_sslMutexes = new std::mutex[locks];
		
		for(int i=0; i<MaxiumumConnections; i++)
		{
			threads.push_back(std::thread(&HttpRequestManager::ThreadFunction, this));
		}
	}

	HttpRequestManager::~HttpRequestManager()
	{
		exit = true;
		conditionVariable.notify_all();
		
		for(auto &thread : threads)
		{
			thread.join();
		}
		
		delete[] g_sslMutexes;
		g_sslMutexes = nullptr;
		curl_global_cleanup();
	}
	
	void HttpRequestManager::Cancel(HttpRequestHandle requestHandle)
	{
		std::lock_guard<std::mutex> lock(mutex);
		std::lock_guard<std::mutex> cancelLock(cancelMutex);
		waitingRequests.remove_if([=](RequestInfo &i){return std::get<1>(i) == requestHandle;});
		activeRequests.remove_if([=](RequestInfo &i){return std::get<1>(i) == requestHandle;});
	}
	
	HttpRequestHandle HttpRequestManager::PerformGet(const std::string &requestUrl, HttpCallback callback)
	{
		int id;
		
		{
			std::lock_guard<std::mutex> lock(mutex);
			id = currentRequestHandle++;
			waitingRequests.push_back(RequestInfo(requestUrl, id, callback));
		}
		conditionVariable.notify_one();
		return id;
	}

	struct RequestUserData
	{
		std::string response;
	};
	
	static size_t write_function(void *ptr, size_t size, size_t nmemb, void *userdata)
	{
		RequestUserData *requestData = static_cast<RequestUserData*>(userdata);
		requestData->response.append(static_cast<char*>(ptr), size * nmemb);
		return(nmemb * size);
	}
	
	void HttpRequestManager::ThreadFunction()
	{
		CURL *curlEasyHandle;
		RequestUserData requestData;
		
		curlEasyHandle = curl_easy_init();
		curl_easy_setopt(curlEasyHandle, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curlEasyHandle, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, *write_function);
		curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, &requestData);
		curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERTTYPE, "PEM");
		curl_easy_setopt(curlEasyHandle, CURLOPT_SSL_CTX_FUNCTION, &ssl_ctx_function);

		do
		{
			std::unique_lock<std::mutex> lock(mutex);

			if(!waitingRequests.empty())
			{
				auto request = waitingRequests.front();
				waitingRequests.pop_front();
				activeRequests.push_back(request);
				requestData.response.clear();
				lock.unlock();

				curl_easy_setopt(curlEasyHandle, CURLOPT_URL, std::get<0>(request).c_str());
				CURLcode curlReturnValue = curl_easy_perform(curlEasyHandle);

				lock.lock();
				auto iterator = std::find_if(activeRequests.begin(), activeRequests.end(),
											 [=](RequestInfo &i){return std::get<1>(i) == std::get<1>(request);});
				if(iterator != activeRequests.end())
				{
					activeRequests.erase(iterator);
					
					lock.unlock();
					
					cancelMutex.lock();
					
					if(std::get<2>(request) != nullptr)
					{
						if(curlReturnValue == CURLE_OK)
							std::get<2>(request)(true, requestData.response);
						else
							std::get<2>(request)(false, "");
					}
					
					cancelMutex.unlock();
				}
			}
			else
			{
				conditionVariable.wait(lock);
			}
		} while(!exit);
	}
}
