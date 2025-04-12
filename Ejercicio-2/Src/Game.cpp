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
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
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

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
	// Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom(float ratio)
{
	View camara;
	// Posicionamiento y tamaño de la vista
	camara.setSize(100.0f * ratio, 100.0f);
	camara.setCenter(50.0f * ratio, 50.0f);
	wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
	// Inicializar el mundo físico con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Crear un renderer de debug para visualizar el mundo físico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Crear el suelo y las paredes estáticas del mundo físico
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 133, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(133.0f, 50.0f), 0.0f);

	// Crear un círculo que se controlará con el mouse
	controlBody1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	b2Vec2 bodyPos1 = b2Vec2(50.0f, 50.0f);
	controlBody1->SetTransform(bodyPos1, 0.0f);

	// Crear un círculo que se controlará con el mouse
	controlBody2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	b2Vec2 bodyPos2 = b2Vec2(80.0f, 60.0f);
	controlBody2->SetTransform(bodyPos2, 0.0f);


	//Debemos crear un objeto DistanceJointDef que contiene una descripción de la restricción:
	b2DistanceJointDef jointDef;

	//Luego debemos inicializar el objeto:
	//first y second son punteros a los dos objetos que se encuentran unidos por el resorte.
	//anchorFirst y anchorSecond son los puntos de anclaje de cada objeto, estos puntos deben ser dados en coordenadas del mundo.
	jointDef.Initialize(controlBody1, controlBody2, bodyPos1, bodyPos2);
	jointDef.collideConnected = true;
	jointDef.length = (bodyPos1 - bodyPos2).Length();
	jointDef.damping = 1.0f;
	jointDef.stiffness = 0.1f;
	//ESTA PARTE NO ESTÁ EXPLICITADA EN EL APUNTE
	//b2LinearStiffness(jointDef.stiffness, jointDef.damping, 10, 0.5f, jointDef.bodyA, jointDef.bodyB);
	b2DistanceJoint* distJoint = (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef);
}

// Destructor de la clase

Game::~Game(void)
{
}