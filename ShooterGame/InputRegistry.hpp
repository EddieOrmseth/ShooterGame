#pragma once
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>

#define Listener std::function<void(bool pressed)>*
struct KeyInfo;

class InputRegistry {

public:

	InputRegistry();
	~InputRegistry();

	void addListener(Listener listener, int keyCode);

	void resgisterKeyInput(KeyInfo* info);

	void handleAllKeyInput();
	void handleKeyInput();
	void handleKeyInput(KeyInfo* info);

	void updateListeners();
	void updateListeners(KeyInfo* info);
	void completelyUpdateListeners();

	bool readKey(int keyCode);

	static InputRegistry* getDefault();

private:

	std::queue<KeyInfo*>* inputQueue;
	std::mutex* queueMutex;

	std::unordered_map<int, std::vector<Listener>*>* listeners;
	std::unordered_map<int, bool>* keyStates;

	static InputRegistry* defaultRegistry;

};

struct KeyInfo {

	KeyInfo(int keyCode, bool pressed) {
		this->keyCode = keyCode;
		this->pressed = pressed;
	}

	~KeyInfo() {
		
	}

	int keyCode;
	bool pressed;
};

// Mouse Key Codes
#define Mouse_Left_Button 0x01
#define Mouse_Middle_Button 0x04
#define Mouse_Right_Button 0x02

// Virtual Key Codes
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

#define VK_ENTER 0xD

//#define VK_UP 38
//#define VK_RIGHT 39
//#define VK_LEFT 40
//#define VK_DOWN 37