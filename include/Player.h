#pragma once

class Player {
  public:
	Player(int iID);
	~Player();

	void setXPosition(int XPosition);
	void setYPosition(int YPosition);
	void setAngle(int Angle);

	int getID() const;
	int getXPosition() const;
	int getYPosition() const;
	int getAngle() const;

  protected:
	int m_iPlayerID;
	int m_XPosition;
	int m_YPosition;
	int m_Angle;
};