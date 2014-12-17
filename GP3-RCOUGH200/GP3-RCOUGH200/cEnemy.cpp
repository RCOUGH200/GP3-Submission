#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::randomise()
{
	//Start enemy on random side of the screen
	if (rand() % 2 == 0)
	{
		cModel::m_mdlPosition.x = -PLAYFIELDX;
	}
	else
	{
		cModel::m_mdlPosition.x = PLAYFIELDX;
	}
	cModel::m_mdlPosition.y = 0.0f;
	cModel::m_mdlPosition.z = (rand() / (float)RAND_MAX) * 300.0f;   // random number as a float between 0 & 1
	cModel::m_mdlRotation = (rand() / (float)RAND_MAX) * 2 * 3.14f;
	cModel::m_mdlDirection.x = -(float)glm::sin(cModel::m_mdlRotation);
	cModel::m_mdlDirection.z = (float)glm::cos(cModel::m_mdlRotation);
	cModel::m_mdlSpeed = m_EnemyMinSpeed + rand() / (float)RAND_MAX * m_EnemyMaxSpeed;
	cModel::m_IsActive = true;
}

void cEnemy::update(float elapsedTime)
{
	//Update model position
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;

	//Rotate constantl depending on speed
	cModel::m_mdlRotation += 2 * cModel::m_mdlSpeed* elapsedTime;

	//If the enemy leaves the playing field, spawn them on the other side
	if (cModel::m_mdlPosition.x > PLAYFIELDX)
		cModel::m_mdlPosition.x -= 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.x < -PLAYFIELDX)
		cModel::m_mdlPosition.x += 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.z > PLAYFIELDZ)
		cModel::m_mdlPosition.z -= 2 * PLAYFIELDZ;
	if (cModel::m_mdlPosition.z < -PLAYFIELDZ)
		cModel::m_mdlPosition.z += 2 * PLAYFIELDZ;
}

cEnemy::~cEnemy()
{

}

