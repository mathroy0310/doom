#include "Player.h"
#include <math.h>

Player::Player(ViewRenderer *pViewRenderer, int iID)
    : m_pViewRenderer(pViewRenderer), m_iPlayerID(iID), m_FOV(90), m_iRotationSpeed(12), m_iMovementSpeed(16), m_ZPosition(41) {}

Player::~Player() {}

void Player::init(Thing thing) {
	setXPosition(thing.XPosition);
	setYPosition(thing.YPosition);
	setAngle(thing.Angle);
	m_HalfFOV = m_FOV / 2;
}

int Player::getID() const { return m_iPlayerID; }

int Player::getXPosition() const { return m_XPosition; }

int Player::getYPosition() const { return m_YPosition; }

int Player::getZPosition() const { return m_ZPosition; }

Angle Player::getAngle() const { return m_Angle; }

void Player::setXPosition(int XPosition) { m_XPosition = XPosition; }

void Player::setYPosition(int YPosition) { m_YPosition = YPosition; }

void Player::setZPosition(int ZPosition) { m_ZPosition = ZPosition; }

void Player::setAngle(int Angle) { m_Angle = (float) Angle; }

Angle Player::angleToVertex(Vertex &vertex) {
	float Vdx = vertex.XPosition - m_XPosition;
	float Vdy = vertex.YPosition - m_YPosition;

	Angle vertexAngle(atan2f(Vdy, Vdx) * 180.0f / M_PI);

	return vertexAngle;
}

bool Player::clipVertexesInFOV(Vertex &V1, Vertex &V2, Angle &V1Angle, Angle &V2Angle, Angle &V1AngleFromPlayer, Angle &V2AngleFromPlayer) {
	V1Angle = angleToVertex(V1);
	V2Angle = angleToVertex(V2);

	Angle V1ToV2Span = V1Angle - V2Angle;

	if (V1ToV2Span >= 180) {
		return false;
	}

	// Rotate every thing.
	V1AngleFromPlayer = V1Angle - m_Angle;
	V2AngleFromPlayer = V2Angle - m_Angle;

	// Validate and Clip V1
	// shift angles to be between 0 and 90 (now virtualy we shifted FOV to be in that range)
	Angle V1Moved = V1AngleFromPlayer + m_HalfFOV;

	if (V1Moved > m_FOV) {
		// now we know that V1, is outside the left side of the FOV
		// But we need to check is Also V2 is outside.
		// Lets find out what is the size of the angle outside the FOV
		Angle V1MovedAngle = V1Moved - m_FOV;

		// Are both V1 and V2 outside?
		if (V1MovedAngle >= V1ToV2Span) {
			return false;
		}

		// At this point V2 or part of the line should be in the FOV.
		// We need to clip the V1
		V1AngleFromPlayer = m_HalfFOV;
	}

	// Validate and Clip V2
	Angle V2Moved = m_HalfFOV - V2AngleFromPlayer;

	// Is V2 outside the FOV?
	if (V2Moved > m_FOV) {
		V2AngleFromPlayer = -m_HalfFOV;
	}

	V1AngleFromPlayer += 90;
	V2AngleFromPlayer += 90;

	return true;
}

void Player::moveForward() {
	m_XPosition += m_Angle.getCosValue() * m_iMovementSpeed;
	m_YPosition += m_Angle.getSinValue() * m_iMovementSpeed;
}

void Player::moveBackward() {
	m_XPosition -= m_Angle.getCosValue() * m_iMovementSpeed;
	m_YPosition -= m_Angle.getSinValue() * m_iMovementSpeed;
}

void Player::rotateLeft() { m_Angle += (0.1875f * m_iRotationSpeed); }

void Player::rotateRight() { m_Angle -= (0.1875f * m_iRotationSpeed); }

void Player::renderAutoMap() {
	m_pViewRenderer->setDrawColor(255, 0, 0);

	m_pViewRenderer->drawLine(m_XPosition, m_YPosition, m_XPosition + 5, m_YPosition + 5);
}

float Player::distanceToPoint(Vertex &V) {
	// We have two points, where the player is and the vertex passed.
	// To calculate the distance just use "The Distance Formula"
	// distance = square root ((X2 - X1)^2 + (y2 - y1)^2)
	return sqrt(pow(m_XPosition - V.XPosition, 2) + pow(m_YPosition - V.YPosition, 2));
}

int Player::getFOV() const { return m_FOV; }