#pragma once
#include "BaseGame.h"
#include "cubes.h"
#include <vector>
#include "texture.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

	bool IsMouseOverBox(const Point2f& boxPos, float mouseX, float mouseY);
	void FuseBoxes();
	void AddNewBox();  // Function to add new boxes
	void DrawScore();



private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	float movingx1;
	float movingx2;
	float movingx3;
	float movingx4;

	float movingy1;
	float movingy2;
	float movingy3;
	float movingy4;

	std::vector<Point2f> m_BoxPositions;
	std::vector<bool> m_BoxPickedUp;
	Point2f m_MouseOffset; 
	int m_PickedBoxIndex;
	int m_NumBoxes;  // Track the number of active boxes
	int m_score;
	Texture* m_ptrTextFont;
	Texture* m_ptrTextScore;
	Texture* m_ptrTextInstructions;
	bool paused;
	std::vector<bool> m_BoxFused;  // Keep track of fused boxes



	cubes cube;
};