#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	// Inicialización de la ventana y configuración de propiedades
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	force = 100.0f;
	SetZoom((float)ancho / (float)alto); // Configuración de la vista del juego
	InitPhysics(); // Inicialización del motor de física
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
	phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
	phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
	phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
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
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
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

// Comprobación de colisiones (a implementar más adelante)
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
void Game::InitPhysics()
{
	// Inicializar el mundo físico con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 0.0f));

	// Crear un renderer de debug para visualizar el mundo físico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Crear el suelo y las paredes estáticas del mundo físico
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 133, 10);
	groundBody->SetTransform(b2Vec2(133.0f/2, 100.0f), 0.0f);

	// Crear el suelo y las paredes estáticas del mundo físico
	b2Body* ceilBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 133, 10);
	ceilBody->SetTransform(b2Vec2(133.0f / 2, 0.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(133, 50.0f), 0.0f);

	// Crear un círculo que se controlará con el teclado
	controlBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld,5.0f, 5.0f, 1.0f, 0.25f, 0.5f);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
}

// Destructor de la clase

Game::~Game(void)
{
}