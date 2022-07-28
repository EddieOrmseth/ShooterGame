#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include "Array.hpp"
#include "TransmissionConnection.hpp"

class NetworkTransmission;
class BasicConnection;
class TransmissionConnection;

class TimeStamp {

public:

	TimeStamp();
	TimeStamp(long long millisecondsSinceEpoch);
	static TimeStamp* FromArray(std::vector<Array<char>*>* data);

	long long getMillisecondsSinceEpoch();

private:

	long long millisecondsSinceEpoch;

};

class DataOrSupplier {

public:

	DataOrSupplier(std::function<NetworkTransmission* (NetworkTransmission*)> supplier);
	DataOrSupplier(NetworkTransmission* networkTransmission);

	NetworkTransmission* getNetworkTransmission(NetworkTransmission* networkTransmission);

private:

	std::function<NetworkTransmission* (NetworkTransmission*)> supplier;
	NetworkTransmission* networkTransmission;
	bool hasData;

};

class NetworkFuture {

	friend class RequestConnection;

public:

	NetworkFuture();

	std::vector<Array<char>*>* getData();

	template <class T>
	T* getObject() {
		volatile int cat = 0;

		while (true) {
			cat++;
			if (isReady()) {
				cat++;
				return T::FromArray(data);;
			}
		}
	}

	//template <class T>
	//T getPrimitive() {

	//	volatile int cat = 0;

	//	while (true) {
	//		cat++;
	//		if (isReady()) {
	//			cat++;
	//			std::string typeID = std::string(typeid(T).name());
	//			if (typeID == "char") {
	//				char primitive = ((char*)data->getData())[0];
	//				return primitive;
	//			}
	//			else if (typeID == "int") {
	//				int primitive = ((int*)data->getData())[0];
	//				return primitive;
	//			}
	//			else if (typeID == "double") {
	//				double primitive = ((double*)data->getData())[0];
	//				return primitive;
	//			}
	//			else if (typeID == "long") {
	//				long primitive = ((long*)data->getData())[0];
	//				return primitive;
	//			}
	//			else if (typeID == "long long") {
	//				long long primitive = ((long long*)data->getData())[0];
	//				return primitive;
	//			}
	//		}
	//	}
	//}

	bool isReady();

private:

	void set(std::vector<Array<char>*>* data);

	bool ready;
	std::vector<Array<char>*>* data;

};

class RequestConnection : public TransmissionConnection {

public:

	RequestConnection(BasicConnection* basicConnection);
	~RequestConnection();

	NetworkFuture* request(std::string* id);
	void addAnswer(std::string* id, DataOrSupplier* answer);

protected:

	DataOrSupplier* getAnswer(std::string* id);
	NetworkFuture* getFuture(std::string* id);

	void addRequest(std::string* id, NetworkFuture* future);
	void removeRequest(std::string* id);

	std::unordered_map<std::string, DataOrSupplier*>* answerMap;
	std::mutex* answerMapMutex;

	std::unordered_map<std::string, NetworkFuture*>* requestMap;
	std::mutex* requestMapMutex;

	virtual void processTransmission(NetworkTransmission* networkTransmission);

};