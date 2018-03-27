#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float PI = 3.1415926535897;
const float ROOT2 = 1.41421356237;

namespace hive
{

float distance = 0.25;
float max_thrust = 1;

float desired_x = 0;
float desired_y = 0;
float desire = 50;

float wind_force_x = 0;
float wind_force_y = 0;

float air_resistance = 0;

float size = 0.01;

class bot
{
public:
	bot(float x, float y, std::vector<bot>* bots)
	{
		m_bots = bots;
		m_x = x;
		m_y = y;

		m_index = m_bots->size();
		m_bots->push_back(*this);
	}

	void update()
	{
		// Head towards group
		for (int i = m_index+1; i < m_bots->size(); i++)
		{
			bot& b = m_bots->at(i);

			float xdiff = m_x - b.m_x;
			float ydiff = m_y - b.m_y;

			float rr = (xdiff*xdiff + ydiff*ydiff);
			float r = sqrt(rr);
			
			float dr = (distance - r) / rr;
			float desired_dx = xdiff * dr;
			float desired_dy = ydiff * dr;

			m_d2x += desired_dx - m_dx;
			m_d2y += desired_dy - m_dy;

			b.m_d2x += -desired_dx - b.m_dx;
			b.m_d2y += -desired_dy - b.m_dy;
		}

		// Head to desired location
		float xdiff = m_x - desired_x;
		float ydiff = m_y - desired_y;

		float rr = (xdiff*xdiff + ydiff*ydiff);
		float r = sqrt(rr);
		
		float dr = (0 - r)/rr;
		float desired_dx = xdiff * dr;
		float desired_dy = ydiff * dr;

		// Dedicate most of the thrust to going towards the desired location
		m_d2x += desire * (desired_dx - m_dx);
		m_d2y += desire * (desired_dy - m_dy);

		// Limit thrust
		float thrust = sqrt(m_d2x*m_d2x + m_d2y*m_d2y);
		if (thrust > max_thrust)
		{
			m_d2x *= max_thrust / thrust;
			m_d2y *= max_thrust / thrust;
		}
 
		// Apply acceleration
		m_dx += (m_d2x + wind_force_x)/60.0 - air_resistance * m_dx;
		m_dy += (m_d2y + wind_force_y)/60.0 - air_resistance * m_dy;

		// Apply velocity
		m_x += m_dx/60.0; 
		m_y += m_dy/60.0;

		// Reset acceleration for next update cycle
		m_d2x = 0;
		m_d2y = 0;
	}

	void draw(float vertices[])
	{
		vertices[6*m_index+0] = m_x;
		vertices[6*m_index+1] = m_y + size;
		vertices[6*m_index+2] = m_x - size * ROOT2;
		vertices[6*m_index+3] = m_y - size/2;
		vertices[6*m_index+4] = m_x + size * ROOT2;
		vertices[6*m_index+5] = m_y - size/2;
	}

private:
	std::vector<bot>* m_bots = nullptr;
	int m_index;

	float m_x = 0, m_y = 0;
	float m_dx = 0, m_dy = 0;
	float m_d2x = 0, m_d2y = 0;
};

}