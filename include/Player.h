#pragma once

#include "Angle.h"
#include "DataTypes.h"
#include <SDL.h>

class Player {
  public:
	Player(int iID);
	~Player();

	void setXPosition(int XPosition);
	void setYPosition(int YPosition);
	void setAngle(int Angle);
	void rotateLeft();
	void rotateRight();

	int   getID() const;
	int   getXPosition() const;
	int   getYPosition() const;
	
	Angle getAngle() const;

	bool  clipVertexesInFOV(Vertex &V1, Vertex &V2, Angle &V1Angle, Angle &V2Angle);
	Angle angleToVertex(Vertex &vertex);

  protected:
	int m_iPlayerID;
	int m_XPosition;
	int m_YPosition;
	int m_FOV;
	int m_iRotationSpeed;
	int m_iMovementSpeed;

	Angle m_Angle;
};