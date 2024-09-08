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
	void setZPosition(int ZPosition);
	void setAngle(int Angle);

	void moveForward();
	void moveBackward();
	void rotateLeft();
	void rotateRight();

	void renderAutoMap();

	int   getID() const;
	int   getXPosition() const;
	int   getYPosition() const;
	int   getZPosition() const;
	int   getFOV() const;
	Angle getAngle() const;

	bool clipVertexesInFOV(Vertex &V1, Vertex &V2, Angle &V1Angle, Angle &V2Angle, Angle &V1AngleFromPlayer, Angle &V2AngleFromPlayer);

	Angle angleToVertex(Vertex &vertex);
	float distanceToPoint(Vertex &V);

  protected:
	int m_iPlayerID;
	int m_XPosition;
	int m_YPosition;
	int m_ZPosition;
	int m_FOV;
	int m_iRotationSpeed;
	int m_iMovementSpeed;

	Angle         m_Angle;
	Angle         m_HalfFOV;
	ViewRenderer *m_pViewRenderer;
};