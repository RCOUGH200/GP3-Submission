#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

#define GLM_FORCE_RADIANS

#include <stdlib.h>
#include <Windows.h>
#include <GL\glut.h>
//#include <assimp\Importer.hpp>    // C++ importer interface
//#include <assimp\scene.h>           // Output data structure
//#include <assimp\postprocess.h>     // Post processing flags
#include <OpenAL\al.h>
#include <OpenAL\alc.h>
#include <OpenAL\alut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\geometric.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\quaternion.hpp>
#include "glm-0.3.2\glm\glm.h"
#include <vector>


//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "Meteors"

struct mdlDimensions{
	float s_mdlWidth, s_mdlheight, s_mdldepth;
};
//Playfield is right at the edge of the screen with a slight buffer for model sizes
const float PLAYFIELDX  = 62.0f;
const float PLAYFIELDZ = 62.0f;
extern float shotDelay; //Delay between shots
extern float timer;	//The timer fpr the main game

extern int drawMode; //The way the objects are rendered
extern float rotationAngle; //Rotation speed
extern float translationZ; //Movement speed
extern bool fire; //Player is firing
extern bool camToggled; //Camera is either first or third person

extern int score; //Current score
extern int health; //Current health
extern int menuNum; //Current menu
extern int xDir; //Direction player is rotating
extern int zDir; //Direction player is moving
extern bool playingEng; //If engine sound is playing
extern bool playingTheme; //If currently playing theme
extern bool holdingMute; //If currently holding the mute toggle
extern bool holdingCamTog; //If currently holding the cam toggle
extern bool mute; //If theme music is muted
extern bool ctrlConnected; //If the controller is connected

#endif