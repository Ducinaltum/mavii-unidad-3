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
    SetZoom((float)ancho/(float) alto); // Configuración de la vista del juego
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
    camara.setCenter(xSize/2, 50.0f);
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
    groundBody->SetTransform(b2Vec2(133.0f/2, 100.0f), 0.0f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(133.0f, 50.0f), 0.0f);

    b2Body* fixedBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 10);
    fixedBody->SetTransform(b2Vec2(133.0f / 2, 15.0f), 0.0f);

    // Crear un círculo que se controlará con el teclado
    controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
    controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

    b2DistanceJointDef jointDef;
    jointDef.Initialize(controlBody, fixedBody, controlBody->GetPosition(), fixedBody->GetPosition());
    jointDef.collideConnected = true;
    jointDef.length = (controlBody->GetPosition() - fixedBody->GetPosition()).Length();
    jointDef.stiffness = 1.0f;
    jointDef.damping = 1.0f;
    b2LinearStiffness(jointDef.stiffness, jointDef.damping, 5.0f, 0.7f, controlBody, fixedBody);
    b2DistanceJoint* distJoint = (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef);
}

// Destructor de la clase

Game::~Game(void) { }