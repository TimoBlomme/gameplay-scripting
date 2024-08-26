#include "pch.h"
#include "cubes.h"
#include "utils.h"
#include <iostream>

cubes::cubes():
	m_Position(0,0)
{

}

cubes::~cubes()
{
}

void cubes::SendPosition(Point2f)
{


}

void cubes::Draw(Point2f drawpos) const
{
	utils::FillRect(drawpos.x, drawpos.y, 20, 20);
	std::cout << "drawing at: " << drawpos.x << ", " << drawpos.y << std::endl;;

}


