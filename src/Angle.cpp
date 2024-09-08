#include "Angle.h"

#include <cmath>

Angle::Angle() : m_Angle(0) {}

Angle::Angle(float angle) {
	m_Angle = angle;
	normalize360();
}
Angle::~Angle() {}

Angle Angle::operator=(const float &rhs) {
	m_Angle = rhs;
	normalize360();
	return *this;
}

Angle Angle::operator+(const Angle &rhs) { return Angle(m_Angle + rhs.m_Angle); }

Angle Angle::operator-(const Angle &rhs) { return Angle(m_Angle - rhs.m_Angle); }

Angle Angle::operator-() { return Angle(360 - m_Angle); }

void Angle::normalize360() {
	m_Angle = fmodf(m_Angle, 360);
	if (m_Angle < 0) m_Angle += 360;
}

float Angle::getValue() { return m_Angle; }

float Angle::getCosValue() { return cosf(m_Angle * M_PI / 180.0f); }

float Angle::getSinValue() { return sinf(m_Angle * M_PI / 180.0f); }

float Angle::getTanValue() { return tanf(m_Angle * M_PI / 180.0f); }

float Angle::getSignedValue() { return (m_Angle > 180) ? m_Angle - 360 : m_Angle; }

Angle &Angle::operator+=(const float &rhs) {
	m_Angle += rhs;
	normalize360();
	return *this;
}

Angle &Angle::operator-=(const float &rhs) {
	m_Angle -= rhs;
	normalize360();
	return *this;
}

bool Angle::operator<(const Angle &rhs) { return (m_Angle < rhs.m_Angle); }

bool Angle::operator<(const float &rhs) { return (m_Angle < rhs); }

bool Angle::operator<=(const Angle &rhs) { return (m_Angle <= rhs.m_Angle); }

bool Angle::operator<=(const float &rhs) { return (m_Angle <= rhs); }

bool Angle::operator>(const Angle &rhs) { return (m_Angle > rhs.m_Angle); }

bool Angle::operator>(const float &rhs) { return (m_Angle > rhs); }

bool Angle::operator>=(const Angle &rhs) { return (m_Angle >= rhs.m_Angle); }

bool Angle::operator>=(const float &rhs) { return (m_Angle >= rhs); }