#include <iostream>
#include <thread>
#include <string>
#include "Network.hpp"
#include "BasicConnection.hpp"
#include "Array.hpp"
#include "Sendable.hpp"
#include "RequestConnection.hpp"

//#define SERVER
#define SENDING_TRANSMISSION

void sendAndReceiveData(BasicConnection* connection);
void sendLotsOfData(BasicConnection* connection);
void sendReadData(BasicConnection* connection);
void receiveLotsOfData(BasicConnection* connection);

void receiveAlphabetTransmission(TransmissionConnection* transmissionConnection);
void sendAlphabetTransmission(TransmissionConnection* transmissionConnection);

void sendRequestForSendable(RequestConnection* requestConnection);
void answerSendable(RequestConnection* requestConnection);

void continuousCycleFunction(TransmissionConnection* transmissionConnection, bool* running);

void getTime(RequestConnection* requestConnection);

int main() {

    //std::cout << "int_t Max: " << std::to_string(sizeof(intmax_t)) << std::endl;
    //std::cout << "uint_t Max: " << std::to_string(sizeof(uintmax_t)) << std::endl;

    //uint16_t* test = new uint16_t[1];
    //test[0] = 255;

    //char* chars = (char*)test;

    //char i1 = chars[1];
    //char i0 = chars[0];

    //int cat = 12;


    //TimeStamp constructed = TimeStamp();
    //std::cout << "Constructed TimeStamp: " << std::to_string(constructed.getMillisecondsSinceEpoch()) << std::endl;

    //int size = sizeof(long long);
    //char* data = new char[size];
    //((long long*)data)[0] = constructed.getMillisecondsSinceEpoch();

    //Array<char>* arr = new Array<char>(data, size);

    //std::vector<Array<char>*>* vec = new std::vector<Array<char>*>();
    //vec->push_back(arr);
    //TimeStamp* reconstructed = TimeStamp::FromArray(vec);
    //std::cout << "Reconstructed TimeStamp: " << std::to_string(reconstructed->getMillisecondsSinceEpoch()) << std::endl;


    //Sendable* created = new Sendable("Testing Testing 1 2 3", 47);
    //std::cout << "Created Sendable: " << std::endl << created->toString() << std::endl << std::endl;

    //Array<char>* testData = created->toArray();
    //std::vector<Array<char>*>* vec = new std::vector<Array<char>*>();
    //vec->push_back(testData);
    //Sendable* reconstructed = Sendable::FromArray(vec);
    //std::cout << "Reconstructed Sendable: " << std::endl << reconstructed->toString() << std::endl << std::endl;

    //delete created;
    //delete reconstructed;

    //std::cout << "Here" << std::endl;

    //Array<long>* arr = new Array<long>(5);
    //arr->at(0) = 'A';
    //arr->at(1) = 'B';
    //arr->at(2) = 'C';
    //arr->at(3) = 'D';
    //arr->at(4) = 'E';

    //char* test0 = (char*)(arr->getData());
    //std::cout << test0[0] << std::endl;

    //std::cout << arr->at(1) << std::endl;
    //std::cout << arr->at(2) << std::endl;
    //std::cout << arr->at(3) << std::endl;

    //NetworkFuture* future = new NetworkFuture();
    //future->set((char*) arr->getData(), arr->size());
    //Sendable* test = future->getObject<Sendable>();

    ////int test2 = future->getPrimitive<int>();
    //long test2 = future->getPrimitive<long>();
    //std::cout << "test2: " << test2 << std::endl;

    Network::InitializeNetwork();

    BasicConnection* connection;

#ifdef SERVER
    std::cout << "Waiting For Connection" << std::endl;
    connection = Network::WaitForConnection(AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE, std::string("50800"));
#else
    std::cout << "Requesting Connection" << std::endl;
    connection = Network::TransmissionConnection(AF_UNSPEC, SOCK_STREAM, IPPROTO::IPPROTO_TCP, 0, std::string("50801"), std::string("159.118.186.175"));
#endif

    RequestConnection* requestConnection = new RequestConnection(connection);




    // Public Key Power: e = 65537




    getTime(requestConnection);


//#ifdef SENDING_TRANSMISSION
//
//    sendRequestForSendable(requestConnection);
//
//#else
//
//    answerSendable(requestConnection);
//
//#endif


//    TransmissionConnection* transmissionConnection = new TransmissionConnection(connection);
//
//#ifdef SENDING_TRANSMISSION
//    sendAlphabetTransmission(transmissionConnection);
//#else
//    receiveAlphabetTransmission(transmissionConnection);
//#endif


    //sendAndReceiveData(connection);
    //sendLotsOfData(connection);
    //sendReadData(connection);
    //receiveLotsOfData(connection);


    std::cout << "Complete" << std::endl;

    Sleep(2000);

    Network::CloseNetwork();

    return 0;
}

void sendAndReceiveData(BasicConnection* connection) {

    char* recv = new char[32];
    char* send = new char[32];

    memset(recv, 0, 32 * sizeof(char));

    memset(send, 'A', 32 * sizeof(char));
    connection->sendData(send, 32);

    std::cout << std::endl << "Sent Data: " << std::endl;

    for (int i = 0; i < 32; i++) {
        std::cout << send[i];
    }

    memset(send, 'B', 32 * sizeof(char));
    connection->sendData(send, 32);

    for (int i = 0; i < 32; i++) {
        std::cout << send[i];
    }

    Sleep(1000);

    connection->receiveData(recv, 32);

    std::cout << std::endl << "Recieved Data: " << std::endl;

    for (int i = 0; i < 32; i++) {
        std::cout << recv[i];
    }

    connection->receiveData(recv, 32);

    std::cout << std::endl << "Recieved Data: " << std::endl;

    for (int i = 0; i < 32; i++) {
        std::cout << recv[i];
    }

    delete[] recv;
    delete[] send;
}

void sendLotsOfData(BasicConnection* connection) {

    //char zero = 0;
    //setsockopt(*connection->getSocket(), SOL_SOCKET, SO_SNDBUF, &zero, zero);

    //char* send = new char[32];
    char character = 'A';

    //for (int i = 0; i < 26; i++) {
    //    memset(send, character + i, 32 * sizeof(char));
    //    connection->sendData(send, 32);
    //}

    //for (int i = 0; i < 26; i++) {
    //    memset(send, character + i, 32 * sizeof(char));
    //    connection->sendData(send, 32);
    //}

    //for (int i = 0; i < 26; i++) {
    //    memset(send, character + i, 32 * sizeof(char));
    //    connection->sendData(send, 32);
    //}

    Array<char>* send = new Array<char>(32);

    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < 26; i++) {
            memset((void*)(send->getData()), character + i, send->size() * sizeof(char));
            connection->sendData(send);
        }
    }

}

void sendReadData(BasicConnection* connection) {

    std::string input;

    while (true) {
        std::cin >> input;
        connection->sendData(input.c_str(), input.size());
    }

}

void receiveLotsOfData(BasicConnection* connection) {
    //char* receive = new char[32];
    //int dataSize;

    //while (true) {
    //    dataSize = connection->receiveData(receive, 32);
    //    for (int i = 0; i < dataSize; i++) {
    //        std::cout << receive[i];
    //    }
    //    if (dataSize > 0) {
    //        std::cout << std::endl;
    //    }
    //}

    Array<char>* receive = new Array<char>(32);
    int dataSize;

    while (true) {
        dataSize = connection->receiveData(receive);
        for (int i = 0; i < dataSize; i++) {
            std::cout << receive->at(i);
        }
        if (dataSize > 0) {
            std::cout << std::endl;
        }
    }
    
}

void sendAlphabetTransmission(TransmissionConnection* transmissionConnection) {
    Array<char>* alphabet = new Array<char>(26 * 2);
    for (int i = 0; i < 26 * 2; i++) {
        if (i < 26) {
            alphabet->at(i) = 'A' + i;
        }
        else {
            alphabet->at(i) = 'a' + i - 26;
        }
    }

    std::vector<Array<char>*>* data = new std::vector<Array<char>*>();
    data->push_back(alphabet);

    std::string* id = new std::string("alphabet");

    NetworkTransmission* transmission = new NetworkTransmission('m', id, data, 26 * 2, false);
    transmissionConnection->addTransmission(transmission);
    transmissionConnection->sendNextTransmission();
}

void receiveAlphabetTransmission(TransmissionConnection* transmissionConnection) {
    Sleep(100);

    transmissionConnection->receiveNextTransmission();
    NetworkTransmission* transmission = transmissionConnection->getNextUnprocessedTransmission();

    std::cout << "Transmission ID: " << *transmission->getID() << std::endl << "Data: " << std::endl;

    Array<char>* alphabet = transmission->getData()->at(0);
    for (int i = 0; i < alphabet->size(); i++) {
        std::cout << alphabet->at(i);
    }
    std::cout << std::endl;
}

void sendRequestForSendable(RequestConnection* requestConnection) {
    Sleep(50);

    NetworkFuture* responseFuture = requestConnection->request(new std::string("test"));
    requestConnection->sendNextTransmission();

    Sleep(100);

    requestConnection->receiveNextTransmission();
    requestConnection->processNextTransmission();

    std::cout << "Received Sendable: " << std::endl << responseFuture->getObject<Sendable>()->toString() << std::endl;
}

void answerSendable(RequestConnection* requestConnection) {
    Sendable answer = Sendable("This Is The Answer", 73);
    Array<char>* data = answer.toArray();
    std::vector<Array<char>*>* vec = new std::vector<Array<char>*>();
    vec->push_back(data);

    std::cout << "Sendable Data: " << std::endl << answer.toString() << std::endl;

    NetworkTransmission* responseTransmission = new NetworkTransmission('a', new std::string("test"), vec, data->size(), false);

    requestConnection->addAnswer(responseTransmission->getID(), new DataOrSupplier(responseTransmission));

    Sleep(100);

    requestConnection->receiveNextTransmission();
    requestConnection->processNextTransmission();
    requestConnection->sendNextTransmission();
}

void continuousCycleFunction(TransmissionConnection* transmissionConnection, bool* running) {
    std::cout << "Starting Coninuous Function" << std::endl;
    transmissionConnection->continuousBasicRunCycle(running);
}

void getTime(RequestConnection* requestConnection) {
    bool threadRunning = true;
    std::thread newThread = std::thread(continuousCycleFunction, requestConnection, &threadRunning);

    for (int i = 0; i < 3; i++) {
        NetworkFuture* future = requestConnection->request(new std::string("time"));
        std::cout << "Request Registered" << std::endl;
        volatile int stopper = 0;
        while (!future->isReady()) {
            stopper++;
        }

        std::cout << "Data Has Arrived" << std::endl;
        long long timeArrived = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
        long long millisecondsSinceSent = future->getObject<TimeStamp>()->getMillisecondsSinceEpoch();
        std::cout << "Received Time: " << millisecondsSinceSent << " milliseconds" << std::endl;
        std::cout << "One Way Time: " << (timeArrived - millisecondsSinceSent) << " milliseconds" << std::endl;
    }

    Sleep(2000);

    threadRunning = false;
    Sleep(100);
    newThread.join();

}