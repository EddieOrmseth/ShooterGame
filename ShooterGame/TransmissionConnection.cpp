#include "TransmissionConnection.hpp"
#include "Array.hpp"
#include "Sendable.hpp"
#include "BasicConnection.hpp"

NetworkTransmission::NetworkTransmission(char transmissionType, std::string* id, std::vector<Array<char>*>* data, bool received) : NetworkTransmission(transmissionType, id, data, findTotalDataSize(data), received) {

}

NetworkTransmission::NetworkTransmission(char transmissionType, std::string* id, std::vector<Array<char>*>* data, int totalDataSize, bool received) {
	this->transmissionType = transmissionType;
	this->id = id;
	this->data = data;
	this->totalDataSize = totalDataSize;
	this->received = received;
	processed = false;
	deleteData = true;
	markedForDeletion = false;
}

NetworkTransmission::~NetworkTransmission() {
	delete id;
	if (getDeleteData()) {
		for (int i = 0; i < data->size(); i++) {
			delete data->at(i);
		}

		delete data;
	}
}

void NetworkTransmission::setTransmissionType(char transmissionType) {
	this->transmissionType = transmissionType;
}

char NetworkTransmission::getTransmissionType() {
	return transmissionType;
}

void NetworkTransmission::setID(std::string* id) {
	this->id = id;
}

std::string* NetworkTransmission::getID() {
	return id;
}

char NetworkTransmission::getIDSize() {
	return (char)id->size();
}

void NetworkTransmission::setData(std::vector<Array<char>*>* data) {
	setData(data, findTotalDataSize(data));
}

void NetworkTransmission::setData(std::vector<Array<char>*>* data, int totalDataSize) {
	this->data = data;
	this->totalDataSize = totalDataSize;
}

std::vector<Array<char>*>* NetworkTransmission::getData() {
	return data;
}

int NetworkTransmission::getDataSize() {
	return totalDataSize;
}

void NetworkTransmission::setReceived(bool received) {
	this->received = received;
}

bool NetworkTransmission::getReceived() {
	return received;
}

bool NetworkTransmission::getProcessed() {
	return processed;
}

void NetworkTransmission::setProcessed(bool processed) {
	this->processed = processed;
}

bool NetworkTransmission::getMarkedForDeletion() {
	return markedForDeletion;
}

void NetworkTransmission::setMarkedForDeletion(bool markedForDeletion) {
	this->markedForDeletion = markedForDeletion;
}

bool NetworkTransmission::getDeleteData() {
	return deleteData;
}

void NetworkTransmission::setDeleteData(bool deleteData) {
	this->deleteData = deleteData;
}

int NetworkTransmission::findTotalDataSize(std::vector<Array<char>*>* data) {
	int totalSize = 0;

	for (int i = 0; i < data->size(); i++) {
		totalSize += data->at(i)->size();
	}

	return totalSize;
}

TransmissionConnection::TransmissionConnection(BasicConnection* basicConnection) {
	this->basicConnection = basicConnection;
	smallPeekBuffer = new Array<char>(headerSize);
	unprocessedTransmissions = new std::deque<NetworkTransmission*>();
	unsentTransmissions = new std::deque<NetworkTransmission*>();

	receiveDataMutex = new std::mutex();
	sendDataMutex = new std::mutex();

	unprocessedTransmissionsMutex = new std::mutex();
	unsentTransmissionsMutex = new std::mutex();

	//basicConnection->enableSendBufferBlocking();
	basicConnection->setBlocking(false);
}

TransmissionConnection::~TransmissionConnection() {
	delete smallPeekBuffer;
	delete receiveDataMutex;
	delete sendDataMutex;

	unprocessedTransmissionsMutex->lock();
	for (int i = 0; i < unprocessedTransmissions->size(); i++) {
		delete unprocessedTransmissions->at(i);
	}
	unprocessedTransmissionsMutex->unlock();
	delete unprocessedTransmissionsMutex;

	unsentTransmissionsMutex->lock();
	for (int i = 0; i < unsentTransmissions->size(); i++) {
		delete unsentTransmissions->at(i);
	}
	unsentTransmissionsMutex->unlock();
	delete unsentTransmissionsMutex;

	delete unprocessedTransmissions;
	delete unsentTransmissions;
}

void TransmissionConnection::addTransmission(NetworkTransmission* networkTransmission) {
	if (networkTransmission->getReceived()) {
		unprocessedTransmissionsMutex->lock();
		unprocessedTransmissions->push_back(networkTransmission);
		unprocessedTransmissionsMutex->unlock();
	}
	else {
		unsentTransmissionsMutex->lock();
		unsentTransmissions->push_back(networkTransmission);
		unsentTransmissionsMutex->unlock();
	}
}

void TransmissionConnection::receiveAllTransmissions() {
	volatile int bytesReceived = 1;

	while (bytesReceived > 0) {
		bytesReceived = receiveNextTransmission();
	}
}

void TransmissionConnection::receiveTransmissionsContinuously() {
	bool running = true;
	receiveTransmissionsContinuously(&running);
}

void TransmissionConnection::receiveTransmissionsContinuously(bool* running) {
	while (*running) {
		receiveNextTransmission();
	}

	std::cout << "Ending Continuous Receiving of Transmissions" << std::endl;
}

int TransmissionConnection::receiveNextTransmission() {
	receiveDataMutex->lock();

	int justReceivedHeaderDataAmount = basicConnection->receiveData(smallPeekBuffer->getData() + receivedHeaderDataAmount, smallPeekBuffer->size() - receivedHeaderDataAmount);
	if (justReceivedHeaderDataAmount > 0) {
		receivedHeaderDataAmount += justReceivedHeaderDataAmount;
	}
	if (receivedHeaderDataAmount != headerSize) {
		receiveDataMutex->unlock();
		return receivedHeaderDataAmount;
	}
	receivedHeaderDataAmount = 0;

	if (!isTransmission(smallPeekBuffer)) {
		throw "Not Currently Accepting Non Transmission Format Data!";
	}

	char transmissionType = extractTransmissionType(smallPeekBuffer);
	char idSize = extractIDSize(smallPeekBuffer);
	int dataSize = extractDataSize(smallPeekBuffer);

	std::string* id = receiveID(idSize);
	std::vector<Array<char>*>* data = receiveData(dataSize);

	receiveDataMutex->unlock();

	addTransmission(new NetworkTransmission(transmissionType, id, data, dataSize, true));
	return idSize + dataSize;
}

bool TransmissionConnection::hasTransmissionsToSend() {
	return unsentTransmissions->size() > 0;
}

void TransmissionConnection::sendAllTransmissions() {
	while (hasTransmissionsToSend()) {
		sendNextTransmission();
	}
}

void TransmissionConnection::sendTransmissionsContinuously() {
	bool running = true;
	return sendTransmissionsContinuously(&running);
}

void TransmissionConnection::sendTransmissionsContinuously(bool* running) {
	while (*running) {
		sendNextTransmission();
	}

	std::cout << "Ending Continuous Sending of Transmissions" << std::endl;
}

int TransmissionConnection::sendNextTransmission() {
	NetworkTransmission* networkTransmission = getNextUnsentTransmission();

	if (networkTransmission != nullptr) {
		return sendTransmission(networkTransmission);
	}
	else {
		return 0;
	}
}

bool TransmissionConnection::hasTransmissionsToProcess() {
	return unprocessedTransmissions->size() > 0;
}

void TransmissionConnection::processAllTransmissions() {
	while (hasTransmissionsToProcess()) {
		processNextTransmission();
	}
}

void TransmissionConnection::processTransmissionsContinuously() {
	bool running = true;
	processTransmissionsContinuously(&running);
}

void TransmissionConnection::processTransmissionsContinuously(bool* running) {
	while (*running) {
		processNextTransmission();
	}

	std::cout << "Ending Continuous Processing of Transmissions" << std::endl;
}

void TransmissionConnection::processNextTransmission() {
	NetworkTransmission* networkTransmission = getNextUnprocessedTransmission();

	if (networkTransmission != nullptr) {
		processTransmission(networkTransmission);
	}
	else {
		return;
	}
}

void TransmissionConnection::continuousBasicRunCycle() {
	bool running = true;
	continuousBasicRunCycle(&running);
}

void TransmissionConnection::continuousBasicRunCycle(bool* running) {
	while (*running) {
		basicRunCycle();
	}

	std::cout << "Exiting Continuous Basic Run Cycle" << std::endl;
}

void TransmissionConnection::basicRunCycle() {
	receiveNextTransmission();
	processNextTransmission();
	sendNextTransmission();
}

bool TransmissionConnection::isTransmission(Array<char>* header) {
	return header->at(0) == '\\' && header->at(1) == 't';
}

char TransmissionConnection::extractTransmissionType(Array<char>* header) {
	return header->at(2);
}

char TransmissionConnection::extractIDSize(Array<char>* header) {
	return header->at(3);
}

int TransmissionConnection::extractDataSize(Array<char>* header) {
	return ((int*)smallPeekBuffer->getData())[1];
}

std::string* TransmissionConnection::receiveID(char size) {
	char* data = new char[size];
	receiveBytes(data, size);
	return new std::string(data, size);
}

std::vector<Array<char>*>* TransmissionConnection::receiveData(int size) {
	int totalBytesToBeRead = size;
	int totalBytesRead = 0;
	int nextChunkSize = size > maximumContinuousHeapAllocation ? maximumContinuousHeapAllocation : size;

	std::vector<Array<char>*>* arrays = new std::vector<Array<char>*>();

	while (totalBytesRead != totalBytesToBeRead) {
		arrays->push_back(new Array<char>(nextChunkSize));
		receiveBytes(arrays->at(arrays->size() - 1), 0);
		totalBytesRead += nextChunkSize;
	}

	return arrays;
}

void TransmissionConnection::receiveBytes(Array<char>* data, int offset) {
	receiveBytes(data->getData() + offset, data->size() - offset);
}

void TransmissionConnection::receiveBytes(char* data, int size) {
	int bytesReceived = 0;
	int justReceived;

	while (bytesReceived != size) {
		justReceived = basicConnection->receiveData(data + bytesReceived, size - bytesReceived);
		if (justReceived > 0) {
			bytesReceived += justReceived;
		}
	}
}

int TransmissionConnection::sendTransmission(NetworkTransmission* networkTransmission) {
	sendDataMutex->lock();

	char* header = new char[8];
	header[0] = '\\';
	header[1] = 't';
	header[2] = networkTransmission->getTransmissionType();
	header[3] = networkTransmission->getIDSize();
	((int*)header)[1] = networkTransmission->getDataSize();

	basicConnection->sendData(header, 8);

	std::string* id = networkTransmission->getID();
	basicConnection->sendData(id->c_str(), id->size());
	std::vector<Array<char>*>* data = networkTransmission->getData();
	for (int i = 0; i < data->size(); i++) {
		basicConnection->sendData(data->at(i));
	}

	sendDataMutex->unlock();

	int idSize = networkTransmission->getIDSize();
	int dataSize = networkTransmission->getDataSize();

	if (networkTransmission->getMarkedForDeletion()) {
		delete networkTransmission;
	}

	return 4 + idSize + dataSize;
}

NetworkTransmission* TransmissionConnection::getNextUnsentTransmission() {
	unsentTransmissionsMutex->lock();
	NetworkTransmission* networkTransmission = nullptr;
	if (unsentTransmissions->size() > 0) {
		networkTransmission = unsentTransmissions->front();
		unsentTransmissions->pop_front();
	}
	unsentTransmissionsMutex->unlock();

	return networkTransmission;
}

NetworkTransmission* TransmissionConnection::getNextUnprocessedTransmission() {
	unprocessedTransmissionsMutex->lock();
	NetworkTransmission* networkTransmission = nullptr;
	if (unprocessedTransmissions->size() > 0) {
		networkTransmission = unprocessedTransmissions->front();
		unprocessedTransmissions->pop_front();
	}
	unprocessedTransmissionsMutex->unlock();

	return networkTransmission;
}

void TransmissionConnection::processTransmission(NetworkTransmission* networkTransmission) {
	networkTransmission->setProcessed(true);
}
