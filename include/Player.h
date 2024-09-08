#pragma once

#include "Angle.h"
#include "DataTypes.h"
#include "Things.h"
#include "ViewRenderer.h"

class Player {
  public:
	Player(ViewRenderer *pViewRenderer, int iID);
	~Player();

	void init(Thing thing);
	void setXPosition(int XPosition);
	void setYPosition(int YPosition);
	void setAngle(int Angle);
	void rotateLeft();
	void rotateRight();

	void renderAutoMap();

	int getID() const;
	int getXPosition() const;
	int getYPosition() const;

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

	Angle         m_Angle;
	ViewRenderer *m_pViewRenderer;
};