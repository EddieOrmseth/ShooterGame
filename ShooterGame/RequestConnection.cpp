#include "RequestConnection.hpp"

TimeStamp::TimeStamp() : TimeStamp((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch())).count()) {

}

TimeStamp::TimeStamp(long long millisecondsSinceEpoch) {
	this->millisecondsSinceEpoch = millisecondsSinceEpoch;
}

TimeStamp* TimeStamp::FromArray(std::vector<Array<char>*>* data) {
	long long time = ((long long*)data->at(0)->getData())[0];
	return new TimeStamp(time);
}

long long TimeStamp::getMillisecondsSinceEpoch() {
	return millisecondsSinceEpoch;
}

DataOrSupplier::DataOrSupplier(std::function<NetworkTransmission* (NetworkTransmission*)> supplier) {
	this->supplier = supplier;
	hasData = false;
}

DataOrSupplier::DataOrSupplier(NetworkTransmission* networkTransmission) {
	this->networkTransmission = networkTransmission;
	hasData = true;
}

NetworkTransmission* DataOrSupplier::getNetworkTransmission(NetworkTransmission* networkTransmission) {
	if (hasData) {
		return this->networkTransmission;
	}
	else {
		return supplier(networkTransmission);
	}
}

NetworkFuture::NetworkFuture() {
	ready = false;
	data = nullptr;
}

std::vector<Array<char>*>* NetworkFuture::getData() {
	return data;
}

void NetworkFuture::set(std::vector<Array<char>*>* data) {
	this->data = data;
	ready = true;
}

bool NetworkFuture::isReady() {
	return ready;
}

RequestConnection::RequestConnection(BasicConnection* basicConnection) : TransmissionConnection(basicConnection) {
	answerMap = new std::unordered_map<std::string, DataOrSupplier*>();
	requestMap = new std::unordered_map<std::string, NetworkFuture*>();
	answerMapMutex = new std::mutex();
	requestMapMutex = new std::mutex();

	std::function<NetworkTransmission* (NetworkTransmission*)> timeSupplier = [](NetworkTransmission* transmission) {
		int size = sizeof(long long);
		char* data = new char[size];
		((long long*)data)[0] = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();

		std::cout << "Milliseconds Being Sent: " << std::to_string(((long long*)data)[0]) << std::endl;

		std::vector<Array<char>*>* vec = new std::vector<Array<char>*>();
		vec->push_back(new Array<char>(data, size));

		NetworkTransmission* response = new NetworkTransmission('a', new std::string("time"), vec, false);
		response->setMarkedForDeletion(true);
		response->setDeleteData(true);

		return response;
	};

	addAnswer(new std::string("time"), new DataOrSupplier(timeSupplier));
}

RequestConnection::~RequestConnection() {
	requestMapMutex->lock();
	delete requestMap;
	requestMapMutex->unlock();
	delete requestMapMutex;

	answerMapMutex->lock();
	delete answerMap;
	answerMapMutex->unlock();
	delete answerMapMutex;
}

NetworkFuture* RequestConnection::request(std::string* id) {
	NetworkFuture* future = new NetworkFuture();
	addTransmission(new NetworkTransmission('r', id, new std::vector<Array<char>*>(), 0, false));
	addRequest(id, future);

	return future;
}

void RequestConnection::addAnswer(std::string* id, DataOrSupplier* answer) {
	answerMapMutex->lock();
	answerMap->emplace(*id, answer);
	answerMapMutex->unlock();
}

DataOrSupplier* RequestConnection::getAnswer(std::string* id) {
	answerMapMutex->lock();
	DataOrSupplier* dataOrSupplier = answerMap->at(*id);
	answerMapMutex->unlock();
	return dataOrSupplier;
}

NetworkFuture* RequestConnection::getFuture(std::string* id) {
	requestMapMutex->lock();
	NetworkFuture* networkFuture = requestMap->at(*id);
	requestMapMutex->unlock();
	return networkFuture;
}

void RequestConnection::addRequest(std::string* id, NetworkFuture* future) {
	requestMapMutex->lock();
	requestMap->emplace(*id, future);
	requestMapMutex->unlock();
}

void RequestConnection::removeRequest(std::string* id) {
	requestMapMutex->lock();
	requestMap->erase(*id);
	requestMapMutex->unlock();
}

void RequestConnection::processTransmission(NetworkTransmission* networkTransmission) {
	char transmissionType = networkTransmission->getTransmissionType();
	if (transmissionType == 'r') {
		std::string* id = networkTransmission->getID();
		DataOrSupplier* answer = getAnswer(id);
		NetworkTransmission* response;
		if (answer != nullptr) {
			response = answer->getNetworkTransmission(networkTransmission);
		}
		else {
			response = new NetworkTransmission('a', id, new std::vector<Array<char>*>(), 0, false);
			std::cout << "Unknown Data Requested" << std::endl;
		}

		addTransmission(response);

		networkTransmission->setDeleteData(true);
		delete networkTransmission;
		return;
	}
	else if (transmissionType == 'a') {
		std::string* id = networkTransmission->getID();
		NetworkFuture* requester = getFuture(id);
		if (requester != nullptr) {
			requester->set(networkTransmission->getData());
			removeRequest(id);
		}
		else {
			std::cout << "Unrequested Data Received" << std::endl;
		}

		networkTransmission->setDeleteData(false);
		delete networkTransmission;
		return;
	}
	else {
		TransmissionConnection::processTransmission(networkTransmission);
		return;
	}
}
