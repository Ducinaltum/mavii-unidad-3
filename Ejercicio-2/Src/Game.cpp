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
void Game::DrawGame(){}

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

	if (Mouse::isButtonPressed(Mouse::Left))
	{
		Vector2i mPos = Mouse::getPosition(*wnd);
		Vector2f mousePos = wnd->mapPixelToCoords(mPos);;

		b2Vec2 pos(mousePos.x, mousePos.y);
		std::cout << mousePos.x << " " << mousePos.y << std::endl;
		if (!wasMousePressed)
		{
			wasMousePressed = true;
			if (controlBody1->GetFixtureList()->TestPoint(pos))
			{
				draggedBody = controlBody1;
			}
			else if (controlBody2->GetFixtureList()->TestPoint(pos))
			{
				draggedBody = controlBody2;
			}
		}
		else if (draggedBody != nullptr)
		{
			draggedBody->SetAwake(true);
			draggedBody->SetTransform(pos, draggedBody->GetAngle());
		}
	}
	else
	{
		wasMousePressed = false;
		draggedBody = nullptr;
	}
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
	// Posicionamiento y tama�o de la vista
	camara.setSize(100.0f * ratio, 100.0f);
	camara.setCenter(50.0f * ratio, 50.0f);
	wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicializaci�n del motor de f�sica y los cuerpos del mundo f�sico
void Game::InitPhysics()
{
	// Inicializar el mundo f�sico con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Crear un renderer de debug para visualizar el mundo f�sico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Crear el suelo y las paredes est�ticas del mundo f�sico
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 133, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(133.0f, 50.0f), 0.0f);

	// Crear un c�rculo que se controlar� con el mouse
	controlBody1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	b2Vec2 bodyPos1 = b2Vec2(50.0f, 50.0f);
	controlBody1->SetTransform(bodyPos1, 0.0f);

	// Crear un c�rculo que se controlar� con el mouse
	controlBody2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	b2Vec2 bodyPos2 = b2Vec2(80.0f, 60.0f);
	controlBody2->SetTransform(bodyPos2, 0.0f);


	//Debemos crear un objeto DistanceJointDef que contiene una descripci�n de la restricci�n:
	b2DistanceJointDef jointDef;

	//Luego debemos inicializar el objeto:
	//first y second son punteros a los dos objetos que se encuentran unidos por el resorte.
	//anchorFirst y anchorSecond son los puntos de anclaje de cada objeto, estos puntos deben ser dados en coordenadas del mundo.
	jointDef.Initialize(controlBody1, controlBody2, bodyPos1, bodyPos2);
	jointDef.collideConnected = true;
	jointDef.length = (bodyPos1 - bodyPos2).Length();
	jointDef.damping = 1.0f;
	jointDef.stiffness = 0.1f;
	//ESTA PARTE NO EST� EXPLICITADA EN EL APUNTE
	//b2LinearStiffness(jointDef.stiffness, jointDef.damping, 10, 0.5f, jointDef.bodyA, jointDef.bodyB);
	b2DistanceJoint* distJoint = (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef);
}

// Destructor de la clase

Game::~Game(void)
{
}