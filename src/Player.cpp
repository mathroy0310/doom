#include "Player.h"

Player::Player(int iID) : m_iPlayerID(iID) {}

Player::~Player() {}

int Player::getID() const { return m_iPlayerID; }

int Player::getXPosition() const { return m_XPosition; }

int Player::getYPosition() const  { return m_YPosition; }

int  Player::getAngle() const { return m_Angle; }

void Player::setXPosition(int XPosition) { m_XPosition = XPosition; }

void Player::setYPosition(int YPosition) { m_YPosition = YPosition; }

void Player::setAngle(int Angle) { m_Angle = Angle; }
