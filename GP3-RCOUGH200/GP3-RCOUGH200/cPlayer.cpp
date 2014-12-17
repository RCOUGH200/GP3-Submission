#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{

}

void cPlayer::update(float elapsedTime)
{
	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = -(float)glm::sin(glm::radians(getRotation()));
	mdlVelocityAdd.y = 0.0f;
	mdlVelocityAdd.z = (float)glm::cos(glm::radians(getRotation()));

	//Rotation depending on xDir value, -1 for left, 1 for right and 0 for no rotation
	cModel::m_mdlRotation += (rotationAngle*xDir);

	//Translate the player depending on the zDir value, -1 for back, 1 for forward and 0 for no change
	mdlVelocityAdd *= -(translationZ*zDir);
	cModel::m_mdlDirection += mdlVelocityAdd;

	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed *elapsedTime;
	cModel::m_mdlDirection *= 0.95f;

	//If the player leaves the playing field, spawn them on the other side
	if (cModel::m_mdlPosition.x > PLAYFIELDX)
		cModel::m_mdlPosition.x -= 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.x < -PLAYFIELDX)
		cModel::m_mdlPosition.x += 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.z > PLAYFIELDZ)
		cModel::m_mdlPosition.z -= 2 * PLAYFIELDZ;
	if (cModel::m_mdlPosition.z < -PLAYFIELDZ)
		cModel::m_mdlPosition.z += 2 * PLAYFIELDZ;
}

cPlayer::~cPlayer()
{

}