#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include "GameConstants.h"
#include <windows.h>
#include "windowOGL.h"
#include "cWNDManager.h"
#include "libdrawtext-0.2.1\drawtext.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cEnemy.h"
#include "cPlayer.h"
#include "cLaser.h"
#include "cSound.h"
#include "Gamepad.h"
#include <vector>
#include <time.h>

#define FONT_SZ	18

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;

    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

    //The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL()) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	srand(time(NULL));

	//The Meteors
	cModelLoader meteorMdl;
	meteorMdl.loadModel("Models/meteor.obj");
	
	//The player
	cModelLoader shipMdl;
	shipMdl.loadModel("Models/ship.obj");

	//The players lasers
	cModelLoader laserMdl;
	laserMdl.loadModel("Models/laserBolt.obj");

	//The Health pickups
	cModelLoader healthMdl;
	healthMdl.loadModel("Models/healthKit.obj");

	//The list of enemies
	std::vector<cEnemy*> enemyList;
	std::vector<cEnemy*>::iterator eIndex;

	//Create 20 enemies
	for (int loop = 0; loop < 20; loop++)
	{
		cEnemy* enemy = new cEnemy();
		enemy->randomise();
		enemy->setMdlDimensions(meteorMdl.getModelDimensions());
		enemyList.push_back(enemy);
	}

	//The repair tools
	std::vector<cEnemy*> healthList;
	std::vector<cEnemy*>::iterator hIndex;

	//Starts with 3 Repair tools in scene
	for (int loop = 0; loop < 3; loop++)
	{
		//Of type enemy so it moves randomly and wraps around screen
		cEnemy* health = new cEnemy();
		health->randomise();
		health->setMdlDimensions(healthMdl.getModelDimensions());
		healthList.push_back(health);
	}

	//The player
	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, 0), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(shipMdl.getModelDimensions());

	//The list of lasers
	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;

	//Create a player using the xbox gamepad
	Gamepad* Player1;
	Player1 = new Gamepad(1);

	// Load font
	struct dtx_font *fntmain;

	fntmain = dtx_open_font("Fonts/BAVEUSE.ttf", 0);
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);             /* ASCII */
	dtx_use_font(fntmain, FONT_SZ);

	// Load FX
	cSound themeMusic;
	themeMusic.createContext();
	themeMusic.loadWAVFile("Audio/SpaceOddity.wav");
	themeMusic.playAudio(AL_LOOPING);

	//explosion sound
	cSound explosionFX;
	explosionFX.loadWAVFile("Audio/grfrag.wav");

	//firing sound
	cSound firingFX;
	firingFX.loadWAVFile("Audio/trprsht1.wav");

	//repair sound
	cSound repairFX;
	repairFX.loadWAVFile("Audio/famas_reload_cock.wav");

	//engine sound
	cSound engineFX;
	engineFX.loadWAVFile("Audio/engine_2.wav");


    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{

		pgmWNDMgr->processWNDEvents(); //Process any window events

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL();
		glClearColor(0.04f, 0.04f, 0.04f, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//If an xbox controller is connected
		if (Player1->IsConnected())
		{
			//Controller is connected still, check for inputs
			ctrlConnected = true;
			Player1->CheckInput();
		}
		
		//If the theme is being unmuted
		if (!mute){
			//Check if the song is already on, if not play
			if (!playingTheme){
				themeMusic.playAudio(AL_TRUE);
				playingTheme = true;
			}
		}
		//If the theme is muted
		else{
			//Make sure the theme is playing then stop
			if (playingTheme){
				themeMusic.stopAudio();
				playingTheme = false;
			}
		}

		//Main game menu
		if (menuNum == 1){
			//update timer
			timer -= elapsedTime;
			//Top down view
			if (camToggled){
				gluLookAt(0, 150, 0, 0, 0, 0, 0, 0, 1);
			}
			//First person view
			else{
				//Work out where the player is facing
				glm::vec3 facingDir;
				facingDir.x = -(float)glm::sin(glm::radians(thePlayer.getRotation()));
				facingDir.y = 0.0f;
				facingDir.z = (float)glm::cos(glm::radians(thePlayer.getRotation()));
				facingDir *= -1;
				//Get the position of the player
				glm::vec3 pos;
				pos = thePlayer.getPosition();
				//Place camera at position of player, facing the same direction
				gluLookAt(pos.x, 2, -pos.z, pos.x + facingDir.x, 2, -(pos.z + facingDir.z), 0, 1, 0);
			}

			//Render the meteors and check for collisions with player
			for (eIndex = enemyList.begin(); eIndex != enemyList.end(); ++eIndex)
			{
				// Meteor must be active
				if ((*eIndex)->isActive()) 
				{
					//Render and update
					meteorMdl.renderMdl((*eIndex)->getPosition(), (*eIndex)->getRotation(), (*eIndex)->getScale());
					(*eIndex)->update(elapsedTime);
					//Check for collisions with the player
					if ((*eIndex)->SphereSphereCollision(thePlayer.getPosition(), thePlayer.getMdlRadius()))
					{
						//Vibrate the controller if there is one
						if (ctrlConnected)
							Player1->Vibrate(20000, 20000);
						//Decrease HP, set inactive and play explosion sound effect
						health--;
						(*eIndex)->setIsActive(false);
						explosionFX.playAudio(AL_FALSE);
					}
				}
			}
			//Check if the player is currently moving
			if (zDir != 0){
				//If the music isn't already playing, play music and set playing to be true
				if (!playingEng){
					engineFX.playAudio(AL_LOOPING);
					playingEng = true;
				}
			}
			//If the player isn't moving
			else{
				//Stop any engine sounds and set playing to false
				engineFX.stopAudio();
				playingEng = false;
			}
			//Check if player is shooting
			if (fire)
			{
				//Only fire if the shot delay has run out
				if (shotDelay <= 0){
					cLaser* laser = new cLaser();
					glm::vec3 mdlLaserDirection;
					//Fet the direction of the player
					mdlLaserDirection.x = -(float)glm::sin(glm::radians(thePlayer.getRotation()));
					mdlLaserDirection.y = 0.0f;
					mdlLaserDirection.z = (float)glm::cos(glm::radians(thePlayer.getRotation()));
					mdlLaserDirection *= -1;
					//Set the direction of the laser to the forward direction of player
					laser->setDirection(mdlLaserDirection);
					laser->setRotation(thePlayer.getRotation());
					laser->setScale(glm::vec3(3, 3, 3));
					//Set the speed of the laser, place is where the player currently is and update
					laser->setSpeed(50.0f);
					laser->setPosition(thePlayer.getPosition() + mdlLaserDirection);
					laser->setIsActive(true);
					laser->setMdlDimensions(laserMdl.getModelDimensions());
					laser->update(elapsedTime);
					laserList.push_back(laser);
					//Set the delay between this shot and the next
					shotDelay = 0.2;
					//Play the firing sound effect
					firingFX.playAudio(AL_FALSE);
				}
			}

			//Lower the shot delay if there is one each frame
			if (shotDelay > 0){
				shotDelay -= elapsedTime;
			}

			//Loop through the lasers and perform any actions
			for (index = laserList.begin(); index != laserList.end(); ++index)
			{
				if ((*index)->isActive())
				{
					//Render the laser then update
					laserMdl.renderMdl((*index)->getPosition(), (*index)->getRotation(), (*index)->getScale());
					(*index)->update(elapsedTime);
					// check for collisions
					for (eIndex = enemyList.begin(); eIndex != enemyList.end(); ++eIndex)
					{
						//Only check active enemies
						if ((*eIndex)->isActive())
						{
							//Check for collisons with the enemy
							if ((*index)->SphereSphereCollision((*eIndex)->getPosition(), (*eIndex)->getMdlRadius()))
							{
								//Increase score and set both objects inactive
								score++;
								(*eIndex)->setIsActive(false);
								(*index)->setIsActive(false);
								//Play explosion sound effect
								explosionFX.playAudio(AL_FALSE);
								break; // No need to check for other bullets.
							}
						}
					}
				}
			}

			//Render the players ship and update
			shipMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
			thePlayer.update(elapsedTime);

			//Render the health
			for (hIndex = healthList.begin(); hIndex != healthList.end(); ++hIndex)
			{
				//Only check active repair kits
				if ((*hIndex)->isActive())
				{
					//Render repair kits and update
					healthMdl.renderMdl((*hIndex)->getPosition(), (*hIndex)->getRotation(), (*hIndex)->getScale());
					(*hIndex)->update(elapsedTime);
					//Check for collisons with player
					if ((*hIndex)->SphereSphereCollision(thePlayer.getPosition(), thePlayer.getMdlRadius()))
					{
						//Only repair if health isn't full
						if (health < 10){
							//Increase health and set the object to inactive
							health++;
							(*hIndex)->setIsActive(false);
							//Play sound effect for repair
							repairFX.playAudio(AL_FALSE);
						}
					}
				}
			}

			//Reset indexs
			eIndex = enemyList.begin();
			index = laserList.begin();
			hIndex = healthList.begin();

			//Erase inactive Enemies
			while (eIndex != enemyList.end())
			{
				if (!(*eIndex)->isActive())
					eIndex = enemyList.erase(eIndex);
				else
					++eIndex;
			}
			//Erase inactive Lasers
			while (index != laserList.end())
			{
				if (!(*index)->isActive())
					index = laserList.erase(index);
				else
					++index;
			}
			//Erase inactive health
			while (hIndex != healthList.end())
			{
				if (!(*hIndex)->isActive())
					hIndex = healthList.erase(hIndex);
				else
					++hIndex;
			}

			//If the player has ran out of health or time, change to final screen
			if (health == 0 || timer < 0.0f){
				menuNum = 3;
			}

			//Spawn new enemies for those lost
			if (enemyList.size() < 20){
				//Loops for all missing enemies
				int diff = 20 - enemyList.size();
				for (int i = 0; i < diff; i++){
					cEnemy* enemy = new cEnemy();
					enemy->randomise();
					enemy->setIsActive(true);
					enemy->setMdlDimensions(meteorMdl.getModelDimensions());
					enemyList.push_back(enemy);
				}
			}
			//Spawn new health if picked up
			if (healthList.size() < 3){
				//Loops for all missing repair kits
				int diff = 3 - healthList.size();
				for (int i = 0; i < diff; i++){
					cEnemy* hp = new cEnemy();
					hp->randomise();
					hp->setIsActive(true);
					hp->setMdlDimensions(healthMdl.getModelDimensions());
					healthList.push_back(hp);
				}
			}
		}
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, windowWidth, 0, windowHeight);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glPushAttrib(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);

		//Text for the main menu
		if (menuNum == 0){
			//Controller version
			if (ctrlConnected){
				glTranslatef(380, 500, 0);
				char const *cPlay = "Press 'Start' to Play";
				dtx_string(cPlay);

				glTranslatef(-50, -200, 0);
				char const *cIns = "Press 'Back' for instructions";
				dtx_string(cIns);
			}
			//Keyboard version
			else{
				glTranslatef(400, 500, 0);
				char const *cPlay = "Press 'P' to Play";
				dtx_string(cPlay);

				glTranslatef(-50, -200, 0);
				char const *cIns = "Press 'T' for instructions";
				dtx_string(cIns);
			}
		}

		//Text for Instructions menu
		if (menuNum == 2){
			glTranslatef(160, 650, 0);
			char const *cAv = "Avoid Meteors and shoot them to increase your score";
			dtx_string(cAv);

			glTranslatef(180, -100, 0);
			char const *cRep = "Collect Repair kits to heal";
			dtx_string(cRep);
			//Controller version
			if (ctrlConnected){
				glTranslatef(20, -100, 0);
				char const *cMov = "Use D-Pad for movement";
				dtx_string(cMov);

				glTranslatef(50, -100, 0);
				char const *cIns = "Press 'A' to fire";
				dtx_string(cIns);

				glTranslatef(-70, -100, 0);
				char const *cTog = "Press 'Y' to Toggle Cameras";
				dtx_string(cTog);

				glTranslatef(20, -100, 0);
				char const *cMute = "Press 'B' to Toggle Mute";
				dtx_string(cMute);

				glTranslatef(20, -100, 0);
				char const *cPlay = "Press 'Start' to Play";
				dtx_string(cPlay);
			}
			//Keyboard version
			else{
				glTranslatef(-20, -100, 0);
				char const *cMov = "Use Arrow Keys for movement";
				dtx_string(cMov);

				glTranslatef(50, -100, 0);
				char const *cIns = "Press 'Space' to fire";
				dtx_string(cIns);

				glTranslatef(-50, -100, 0);
				char const *cTog = "Press 'Q' to Toggle Cameras";
				dtx_string(cTog);

				glTranslatef(20, -100, 0);
				char const *cMute = "Press 'M' to Toggle Mute";
				dtx_string(cMute);

				glTranslatef(60, -100, 0);
				char const *cPlay = "Press 'P' to Play";
				dtx_string(cPlay);
			}
		}

		//Text for main game menu
		if (menuNum == 1){
			//Position text on screen and display health
			glTranslatef(30, 730, 0);
			//Creates a health bar that increases and decreases with hp and shows percentage
			std::string hb = "";
			for (int i = 0; i < health; i++){
				hb += std::string("/");
			}
			int perc = 10 * health;
			std::string sn = "Health:  [" + hb + "]  " + std::to_string(perc) + "%";
			char const *cHealth = sn.c_str();
			dtx_string(cHealth);

			//Reposition for score then display
			glTranslatef(0, -30, 0);
			std::string s = "Score: " + std::to_string(score);
			char const *cScore = s.c_str();
			dtx_string(cScore);

			//Reposition for timer then display
			glTranslatef(0, -30, 0);
			std::string st = "Time: " + std::to_string((int)timer);
			char const *cTime = st.c_str();
			dtx_string(cTime);
		}

		//Text for game over menu
		if (menuNum == 3){

			glTranslatef(440, 450, 0);
			char const *cScore = "Game Over";
			dtx_string(cScore);

			if (health <= 0){
				glTranslatef(-20, -200, 0);
				char const *cHP = "Out of health";
				dtx_string(cHP);
				glTranslatef(0, -50, 0); //Reposition for the final score
			}
			else{
				glTranslatef(0, -200, 0);
				char const *cScore = "Out of Time";
				dtx_string(cScore);
				glTranslatef(-15, -50, 0); //Reposition for the final score
			}
			
			std::string s = "Final Score: " + std::to_string(score);
			char const *cFS = s.c_str();
			dtx_string(cFS);
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		pgmWNDMgr->swapBuffers();
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
