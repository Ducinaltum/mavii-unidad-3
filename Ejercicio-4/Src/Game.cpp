#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	// Inicializaci�n de la ventana y configuraci�n de propiedades
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	force = 100.0f;
	SetZoom((float)ancho / (float)alto); // Configuraci�n de la vista del juego
	InitPhysics(); // Inicializaci�n del motor de f�sica
}

// Bucle principal del juego
void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor); // Limpiar la ventana
		DoEvents(); // Procesar eventos de entrada
		CheckCollitions(); // Comprobar colisiones
		UpdatePhysics(); // Actualizar la simulaci�n f�sica
		DrawGame(); // Dibujar el juego
		wnd->display(); // Mostrar la ventana
	}
}

// Actualizaci�n de la simulaci�n f�sica
void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Simular el mundo f�sico
	phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
	phyWorld->DebugDraw(); // Dibujar el mundo f�sico para depuraci�n
}

// Dibujo de los elementos del juego
void Game::DrawGame() {}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cerrar la ventana si se presiona el bot�n de cerrar
			break;
		}
	}

	// Controlar el movimiento del cuerpo de control con el teclado
	// Segun la numeracion usada, cuando mas cerca de cero mas 
	// lento es el desplazamiento sobre ese eje
	controlBody->SetAwake(true);
	if (Keyboard::isKeyPressed(Keyboard::Left))
		controlBody->ApplyForceToCenter(b2Vec2(-force, 0.0f), true);
	if (Keyboard::isKeyPressed(Keyboard::Right))
		controlBody->ApplyForceToCenter(b2Vec2(force, 0.0f), true);
	if (Keyboard::isKeyPressed(Keyboard::Down))
		controlBody->ApplyForceToCenter(b2Vec2(0.0f, force), true);
	if (Keyboard::isKeyPressed(Keyboard::Up))
		controlBody->ApplyForceToCenter(b2Vec2(0.0f, -force), true);
}

// Comprobaci�n de colisiones (a implementar m�s adelante)
void Game::CheckCollitions()
{
	// Implementaci�n de la comprobaci�n de colisiones
}

// Configuraci�n de la vista del juego
void Game::SetZoom(float ratio)
{
	View camara;
	float xSize = 100.0f * ratio;
	// Posicionamiento y tama�o de la vista
	camara.setSize(xSize, 100.0f);
	camara.setCenter(xSize / 2, 50.0f);
	wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicializaci�n del motor de f�sica y los cuerpos del mundo f�sico
void Game::InitPhysics()
{
	// Inicializar el mundo f�sico con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 0.0f));

	// Crear un renderer de debug para visualizar el mundo f�sico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Crear el suelo y las paredes est�ticas del mundo f�sico
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 133, 10);
	groundBody->SetTransform(b2Vec2(133.0f/2, 100.0f), 0.0f);

	// Crear el suelo y las paredes est�ticas del mundo f�sico
	b2Body* ceilBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 133, 10);
	ceilBody->SetTransform(b2Vec2(133.0f / 2, 0.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(133, 50.0f), 0.0f);

	// Crear un c�rculo que se controlar� con el teclado
	controlBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld,5.0f, 5.0f, 1.0f, 0.25f, 0.5f);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
}

// Destructor de la clase

Game::~Game(void)
{
}