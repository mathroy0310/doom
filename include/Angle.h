#pragma once

class Angle {
  public:
	Angle();
	Angle(float angle);
	~Angle();

	Angle  operator=(const float &rhs);
	Angle  operator+(const Angle &rhs);
	Angle  operator-(const Angle &rhs);
	Angle  operator-();
	Angle &operator+=(const float &rhs);
	Angle &operator-=(const float &rhs);

	bool operator<(const Angle &rhs);
	bool operator<(const float &rhs);
	bool operator<=(const Angle &rhs);
	bool operator<=(const float &rhs);
	bool operator>(const Angle &rhs);
	bool operator>(const float &rhs);
	bool operator>=(const Angle &rhs);
	bool operator>=(const float &rhs);

	float getValue();
	float getCosValue();
	float getSinValue();
	float getTanValue();
	float getSignedValue();

  private:
	float m_Angle;

  protected:
	void normalize360();
};