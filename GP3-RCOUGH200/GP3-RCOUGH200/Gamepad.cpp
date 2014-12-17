#include "Gamepad.h"
#include "GameConstants.h"

Gamepad::Gamepad(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE Gamepad::GetState()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool Gamepad::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if (Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Gamepad::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}

void Gamepad::CheckInput()
{
	//If the player is pressing or holding A
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
		fire = true; //Firing
	//If the player is not holding A
	else
		fire = false; //Not firing

	//If the player is not holding either direction
	if (!(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) && !(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
		zDir = 0; //No movement in z axis

	//If player is holding up
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		zDir = 1; //Move forward

	//If player is holding down
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		zDir = -1; //Move back

	//If the player is not holding left or right
	if (!(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) && !(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
		xDir = 0; //No rotation

	//If player is holding left
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		xDir = 1; //Rotate left

	//If player is holding right
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		xDir = -1;//Rotate right

	//If Start was pressed
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START) 
		//If the current menu is main menu or instructions
		if (menuNum == 0 || menuNum == 2)
			menuNum = 1; //Go to main game

	//If Back was pressed
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)  
		menuNum = 2; //Go to instructions menu

	//If Y was pressed
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		//Check the button is not still being held still
		if (!holdingCamTog){
			holdingCamTog = true;
			//If in third person top down view
			if (camToggled)
				camToggled = false; //Go first person
			//If in first person
			else
				camToggled = true; //Go third person
		}
	}
	//If the cam toggle is not being held
	else
	{
		holdingCamTog = false; //M is not being held
	}

	//If B was pressed
	if (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		//Make sure mute has stopped being held
		if (!holdingMute){
			holdingMute = true; //Mute is being held
			//If muted, unmute and vice versa
			if (mute)
				mute = false;
			else
				mute = true;
		}
	}
	//If mute is not being held
	else
	{
		holdingMute = false; //Mute is not being held
	}
}