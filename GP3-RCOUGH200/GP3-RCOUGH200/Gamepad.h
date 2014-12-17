#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// No MFC
#define WIN32_LEAN_AND_MEAN

// We need the Windows Header and the XInput Header
#include <windows.h>
#include <XInput.h>

// Now, the XInput Library
#pragma comment(lib, "XInput9_1_0.lib")

// XBOX Controller Class Definition
class Gamepad
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum; //Which controller this is
public:
	Gamepad(int playerNumber); //Creates a gamepad for the player
	XINPUT_STATE GetState(); //Used to get state of buttons
	bool IsConnected(); //Check if controller is connected
	void Vibrate(int leftVal = 0, int rightVal = 0); //Vibratsed the left and right side of controller
	void CheckInput(); //Check for specified inputs
};

#endif