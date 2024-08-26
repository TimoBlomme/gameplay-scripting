#include "pch.h"
#include "Game.h"
#include <iostream>
#include <string>
#include "utils.h"


Game::Game(const Window& window)
	:BaseGame{ window },
	movingx1{ 0 },
	movingx2{ 0 },
	movingx3{ 0 },
	movingx4{ 0 },
	movingy1{ 0 },
	movingy2{ 0 },
	movingy3{ 0 },
	movingy4{ 0 },
	m_PickedBoxIndex{ -1 },
	m_NumBoxes{ 4 },
	m_BoxPickedUp{ 0 },
	m_BoxPositions{ 0 },
	m_BoxFused{ 0 },
	m_score{ 0 },
	m_ptrTextFont{ new Texture("You Won!", "Summer Village Demo.otf", 50, Color4f(0.0f, 0.0f, 0.0f, 1.0f))},
	m_ptrTextScore{ new Texture("0", "Summer Village Demo.otf", 50, Color4f(0.0f, 0.0f, 0.0f, 1.0f)) },
	m_ptrTextInstructions{new Texture("get to 50 fusions!", "Summer Village Demo.otf", 50, Color4f(0.0f,0.0f,0.0f,1.0f))},
	paused{0}



{
	//Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

//void Game::Initialize( )
//{
	/*m_BoxPositions[0] = Point2f(190, movingy1);
	m_BoxPositions[1] = Point2f(225, movingy2);
	m_BoxPositions[2] = Point2f(260, movingy3);
	m_BoxPositions[3] = Point2f(295, movingy4);*/

	//for (int i = 0; i < 4; ++i) {
		//m_BoxPickedUp[i] = false;
		//m_BoxFused[i] = false;  
	//}
//}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	if (paused == true)
	{
		return;
	}


	if (m_score >= 50) {
		std::cout << "Congratulations! You won!!" << std::endl;
		return;  // Stop further updates
	}

	for (size_t i = 0; i < m_BoxPositions.size(); ++i) {
		if (!m_BoxPickedUp[i]) {
			m_BoxPositions[i].y -= rand() % 10;  // Small decrement for each update
			if (m_BoxPositions[i].y < 0.f) {
				// Remove the box if it reaches the bottom
				m_BoxPositions.erase(m_BoxPositions.begin() + i);
				m_BoxPickedUp.erase(m_BoxPickedUp.begin() + i);
				m_BoxFused.erase(m_BoxFused.begin() + i);  // Also erase the fused state
				--i;  // Adjust index after erasure
				continue;
			}
		}
	}

	/*for (size_t i = 0; i < m_BoxPositions.size(); ++i) {
		if (!m_BoxPickedUp[i]) {
			m_BoxPositions[i].y -= rand() % 10;  // Small decrement for each update
			if (m_BoxPositions[i].y < 0.f) {
				// Remove the box if it reaches the bottom
				m_BoxPositions.erase(m_BoxPositions.begin() + i);
				m_BoxPickedUp.erase(m_BoxPickedUp.begin() + i);
				--i;  // Adjust index after erasure
				continue;
			}
		}
	}*/


	FuseBoxes();

	static float timeSinceLastBox = 0.f;
	timeSinceLastBox += elapsedSec;
	if (timeSinceLastBox >= 0.5f) {  // Adjust the time interval as needed
		AddNewBox();
		timeSinceLastBox = 0.f;
	}

	DrawScore();
	std::cout << "score =" << m_score << std::endl;;
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}



}

void Game::Draw( ) const
{
	

	ClearBackground( );
	if (m_score < 50) {
		for (const auto& pos : m_BoxPositions) {
			glColor3f(1.0f, 1.0f, 1.0f);  // Draw normal boxes in white
			cube.Draw(pos);
		}
		m_ptrTextInstructions->Draw(Rectf(20, 430, 200, 50));
	}
	else
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		utils::FillRect(0, 0, 1000, 1000);
		m_ptrTextFont->Draw(Rectf(200, 210, 600, 80));
	}


}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_r:
		m_score = 0;
		break;
	case SDLK_SPACE:
		paused = true;
		break;

	default:
		break;
	}

	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_SPACE:
		paused = false;
		break;
	default:
		break;
	}
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	if (m_PickedBoxIndex != -1 && m_PickedBoxIndex < m_BoxPositions.size()) {
		m_BoxPositions[m_PickedBoxIndex] = Point2f(static_cast<float>(e.x) + m_MouseOffset.x, static_cast<float>(e.y) + m_MouseOffset.y);
	}
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (e.button == SDL_BUTTON_LEFT) {
		for (size_t i = 0; i < m_BoxPositions.size(); ++i) {
			if (IsMouseOverBox(m_BoxPositions[i], static_cast<float>(e.x), static_cast<float>(e.y))) {
				m_BoxPickedUp[i] = true;
				m_PickedBoxIndex = static_cast<int>(i);
				m_MouseOffset = Point2f(m_BoxPositions[i].x - e.x, m_BoxPositions[i].y - e.y);
				break;
			}
		}
	}
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{

	if (e.button == SDL_BUTTON_LEFT) {
		if (m_PickedBoxIndex != -1 && m_PickedBoxIndex < m_BoxPositions.size()) {
			m_BoxPickedUp[m_PickedBoxIndex] = false;
		}
		m_PickedBoxIndex = -1;
	}

	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

bool Game::IsMouseOverBox(const Point2f& boxPos, float mouseX, float mouseY)
{
	float boxWidth = 20.f; // Assume box width
	float boxHeight = 20.f; // Assume box height
	return mouseX >= boxPos.x && mouseX <= boxPos.x + boxWidth &&
		mouseY >= boxPos.y && mouseY <= boxPos.y + boxHeight;
}

void Game::FuseBoxes()
{
	for (size_t i = 0; i < m_BoxPositions.size(); ++i) {
		if (m_BoxFused[i]) continue;  // Skip already fused boxes

		for (size_t j = i + 1; j < m_BoxPositions.size(); ) {
			if (m_BoxFused[j]) {
				++j;
				continue;  // Skip already fused boxes
			}

			float distance = std::sqrt(std::pow(m_BoxPositions[i].x - m_BoxPositions[j].x, 2) +
				std::pow(m_BoxPositions[i].y - m_BoxPositions[j].y, 2));
			if (distance < 20.f) {  // If boxes are close enough, fuse them
				m_BoxPositions[i] = Point2f((m_BoxPositions[i].x + m_BoxPositions[j].x) / 2,
					(m_BoxPositions[i].y + m_BoxPositions[j].y) / 2);  // Merge to midpoint

				// Mark both boxes as fused
				m_BoxFused[i] = true;
				m_BoxFused[j] = true;

				// Erase the fused box
				m_BoxPositions.erase(m_BoxPositions.begin() + j);
				m_BoxPickedUp.erase(m_BoxPickedUp.begin() + j);
				m_BoxFused.erase(m_BoxFused.begin() + j);

				// Increment score when boxes are fused
				m_score++;
			}
			else {
				++j;  // Only increment j if no fusion occurs
			}
		}
	}

	/*for (size_t i = 0; i < m_BoxPositions.size(); ++i) {
		for (size_t j = i + 1; j < m_BoxPositions.size(); ) {
			float distance = std::sqrt(std::pow(m_BoxPositions[i].x - m_BoxPositions[j].x, 2) +
				std::pow(m_BoxPositions[i].y - m_BoxPositions[j].y, 2));
			if (distance < 20.f) {  // If boxes are close enough, fuse them
				m_BoxPositions[i] = Point2f((m_BoxPositions[i].x + m_BoxPositions[j].x) / 2,
					(m_BoxPositions[i].y + m_BoxPositions[j].y) / 2);  // Merge to midpoint

				// Erase the fused box
				m_BoxPositions.erase(m_BoxPositions.begin() + j);
				m_BoxPickedUp.erase(m_BoxPickedUp.begin() + j);

				// Increment score when boxes are fused
				m_score++;
			}
			else {
				++j;  // Only increment j if no fusion occurs
			}
		}
	}*/
}

void Game::AddNewBox()
{
	float startX = static_cast<float>(rand() % 800);  // Random X position between 0 and 800
	float startY = 600.f;  // Start from the top
	m_BoxPositions.push_back(Point2f(startX, startY));
	m_BoxPickedUp.push_back(false);
	m_BoxFused.push_back(false);  // Initially, the box is not fused

}

void Game::DrawScore()
{
	// Example position and size for dots
	float dotRadius = 5.0f;  // Radius of the dot
	float dotSpacing = 20.0f;  // Spacing between dots
	float startX = 10.0f;  // Starting x-position
	float y = 580.0f;  // Fixed y-position

	// Set OpenGL states for drawing dots
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw dots based on the current score
	for (int i = 0; i < m_score; ++i) {
		glColor3f(1.0f, 0.0f, 0.0f);  // Red color
		utils::FillEllipse(Point2f(startX + i * dotSpacing, y), dotRadius, dotRadius);
	}
}



void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
