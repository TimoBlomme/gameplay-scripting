#pragma once
class cubes
{
public:
	cubes();
	~cubes();

	void SendPosition(Point2f);
	void Draw(Point2f drawpos) const;

private:
	Point2f m_Position;

};

