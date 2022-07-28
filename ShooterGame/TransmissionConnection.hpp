#pragma once
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <mutex>
#include <functional>
#include "Sendable.hpp"
#include "Array.hpp"

class BasicConnection;

// Add Marked For Deletion And Preallocated Data
class NetworkTransmission {

public:

	NetworkTransmission(char transmissionType, std::string* id, std::vector<Array<char>*>* data, bool received);
	NetworkTransmission(char transmissionType, std::string* id, std::vector<Array<char>*>* data, int totalDataSize, bool received);

	~NetworkTransmission();

	void setTransmissionType(char transmissionType);
	char getTransmissionType();

	void setID(std::string* id);
	std::string* getID();
	char getIDSize();

	void setData(std::vector<Array<char>*>* data);
	void setData(std::vector<Array<char>*>* data, int totalDataSize);
	std::vector<Array<char>*>* getData();
	int getDataSize();

	void setReceived(bool received);
	bool getReceived();

	bool getProcessed();
	void setProcessed(bool processed);

	bool getMarkedForDeletion();
	void setMarkedForDeletion(bool markedForDeletion);

	bool getDeleteData();
	void setDeleteData(bool deleteData);

private:

	int findTotalDataSize(std::vector<Array<char>*>* data);

	char transmissionType;
	std::string* id;
	std::vector<Array<char>*>* data;
	int totalDataSize;
	bool received;
	bool processed;
	bool markedForDeletion;
	bool deleteData;

};

class TransmissionConnection {

public:

	TransmissionConnection(BasicConnection* basicConnection);
	~TransmissionConnection();

	void addTransmission(NetworkTransmission* networkTransmission);

	void receiveAllTransmissions();
	void receiveTransmissionsContinuously();
	void receiveTransmissionsContinuously(bool* running);
	int receiveNextTransmission();

	bool hasTransmissionsToSend();
	void sendAllTransmissions();
	void sendTransmissionsContinuously();
	void sendTransmissionsContinuously(bool* running);
	int sendNextTransmission();

	bool hasTransmissionsToProcess();
	void processAllTransmissions();
	void processTransmissionsContinuously();
	void processTransmissionsContinuously(bool* running);
	void processNextTransmission();

	void continuousBasicRunCycle();
	void continuousBasicRunCycle(bool* running);
	void basicRunCycle();

protected:

	BasicConnection* basicConnection;


	// Receive Data
	const int maximumContinuousHeapAllocation = 1024; // 1 Kilobyte
	const int headerSize = 8;

	Array<char>* smallPeekBuffer;
	std::mutex* receiveDataMutex;
	int receivedHeaderDataAmount = 0;

	bool isTransmission(Array<char>* header);
	char extractTransmissionType(Array<char>* header);
	char extractIDSize(Array<char>* header);
	int extractDataSize(Array<char>* header);

	std::string* receiveID(char size);
	std::vector<Array<char>*>* receiveData(int size);

	void receiveBytes(Array<char>* data, int offset);
	void receiveBytes(char* data, int size);

	
	// Send Data
	int sendTransmission(NetworkTransmission* networkTransmission);
	std::mutex* sendDataMutex;
	NetworkTransmission* getNextUnsentTransmission();


	// Handle Transmissions
	std::deque<NetworkTransmission*>* unprocessedTransmissions;
	std::mutex* unprocessedTransmissionsMutex;
	std::deque<NetworkTransmission*>* unsentTransmissions;
	std::mutex* unsentTransmissionsMutex;

public:
	NetworkTransmission* getNextUnprocessedTransmission();
protected:
	virtual void processTransmission(NetworkTransmission* networkTransmission);

};
