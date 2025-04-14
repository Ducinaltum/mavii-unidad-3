#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo) : ballPosition(75, 50), ballVelocity(0, 0)
{
	srand(NULL);
	// Inicialización de la ventana y configuración de propiedades
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom((float)ancho / (float)alto); // Configuración de la vista del juego
	
	ballVelocity.x = ((rand() % 20) + 20);
	ballVelocity.y = ((rand() % 20) + 20);
	ballRadious = 5;
	ballHardness = 20.0f;
}

// Bucle principal del juego
void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor); // Limpiar la ventana
		DoEvents(); // Procesar eventos de entrada
		CheckCollitions(); // Comprobar colisiones
		UpdatePhysics(); // Actualizar la simulación física
		DrawGame(); // Dibujar el juego
		wnd->display(); // Mostrar la ventana
	}
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
	//Ley de hook
	//F = -k*l
	//k = dureza del resorte
	//l = desplazamiento desde la posición de equilibrio a la punta del resorte
	//	Que tan apretada está la bola

	Vector2f l_Vector(0, 0);
	float lx = (ballPosition.x + ballRadious) - 120;
	if (lx > 0) {
		l_Vector.x = lx;
	}

	lx = (ballPosition.x - ballRadious) - 5;
	if (lx < 0) {
		l_Vector.x = lx;
	}

	float ly = (ballPosition.y) - 80;
	if (ly > 0) {
		l_Vector.y = ly;
	}

	ly = (ballPosition.y - ballRadious) - 5;
	if (ly < 0) {
		l_Vector.y = ly;
	}

	float fx = -ballHardness * l_Vector.x;
	ballVelocity.x += (fx * frameTime);

	float fy = -ballHardness * l_Vector.y;
	ballVelocity.y += (fy * frameTime);

	ballPosition += ballVelocity * frameTime;
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
	// Dibujar el cuerpo de control (círculo)
	sf::CircleShape controlShape(ballRadious);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(ballPosition);
	wnd->draw(controlShape);

	// Dibujar el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujar el suelo
	sf::RectangleShape ceilShape(sf::Vector2f(500, 5));
	ceilShape.setFillColor(sf::Color::Red);
	ceilShape.setPosition(0, 0);
	wnd->draw(ceilShape);

	// Dibujar las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(5, 100));
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(0, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(5, 100)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(129, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);
}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
			break;
		}
	}


}

void Game::CheckCollitions()
{
	// Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom(float ratio)
{
	View camara;
	float xSize = 100.0f * ratio;
	// Posicionamiento y tamaño de la vista
	camara.setSize(xSize, 100.0f);
	camara.setCenter(xSize / 2, 50.0f);
	wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics() {}

// Destructor de la clase
Game::~Game(void) {}