#include "Player.h"

#define PI 3.14159265358979f

Player::Player(int iID) : m_iPlayerID(iID), m_FOV(90), m_iRotationSpeed(12), m_iMovementSpeed(2)
{
}

Player::~Player() {}

int Player::getID() const { return m_iPlayerID; }

int Player::getXPosition() const { return m_XPosition; }

int Player::getYPosition() const { return m_YPosition; }

Angle Player::getAngle() const { return m_Angle; }

void Player::setXPosition(int XPosition) { m_XPosition = XPosition; }

void Player::setYPosition(int YPosition) { m_YPosition = YPosition; }

void Player::setAngle(int Angle) { m_Angle = (float) Angle; }

Angle Player::angleToVertex(Vertex &vertex)
{
    float Vdx = vertex.XPosition - m_XPosition;
    float Vdy = vertex.YPosition - m_YPosition;

    Angle vertexAngle(atan2f(Vdy, Vdx) * 180.0f / PI);

    return vertexAngle;
}

bool Player::clipVertexesInFOV(Vertex &V1, Vertex &V2, Angle &V1Angle, Angle &V2Angle)
{
    V1Angle = angleToVertex(V1);
    V2Angle = angleToVertex(V2);

    Angle V1ToV2Span = V1Angle - V2Angle;

    if (V1ToV2Span >= 180)
    {
        return false;
    }

    // Rotate every thing.
    V1Angle = V1Angle - m_Angle;
    V2Angle = V2Angle - m_Angle;

    Angle HalfFOV = m_FOV / 2;

    // Validate and Clip V1
    // shift angles to be between 0 and 90 (now virtualy we shifted FOV to be in that range)
    Angle V1Moved = V1Angle + HalfFOV;

    if (V1Moved > m_FOV)
    {
        // now we know that V1, is outside the left side of the FOV
        // But we need to check is Also V2 is outside.
        // Lets find out what is the size of the angle outside the FOV
        Angle V1MovedAngle = V1Moved - m_FOV;

        // Are both V1 and V2 outside?
        if (V1MovedAngle >= V1ToV2Span)
        {
            return false;
        }

        // At this point V2 or part of the line should be in the FOV.
        // We need to clip the V1
        V1Angle = HalfFOV;
    }

    // Validate and Clip V2
    Angle V2Moved = HalfFOV - V2Angle;

    // Is V2 outside the FOV?
    if (V2Moved > m_FOV)
    {
        V2Angle = -HalfFOV;
    }

    V1Angle += 90;
    V2Angle += 90;

    return true;
}

void Player::rotateLeft()
{
    m_Angle += (0.1875f * m_iRotationSpeed);
}

void Player::rotateRight()
{
    m_Angle -= (0.1875f * m_iRotationSpeed);
}