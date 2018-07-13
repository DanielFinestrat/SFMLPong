#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <ctime>
#include <sstream>
#include "resources.h"
#include "musicPlayer.h"

using namespace sf;
using namespace std;

const int wWidth = 1000;
const int wHeight = 700;

const Vector2f paddleSize(25, 150);
const float ballRadius = 10;

const float cpuSpeed = 500;
const float distFromCenter = 2; // 2 = de -dist a + dist
const Time updateTime = seconds(0);
const float pi = 3.14159265;

const float initBallSpeed = 350;
const float minBallSpeed = 250;
const float maxBallSpeed = 700;
float ballSpeed = 350;
float factorBallSpeed = 1;
float factorChangeSpeed = 0.85; //0 a 1, más alto = menos cambia

Resources res;
MusicPlayer mus;

//Declaramos el ángulo inicial de la bola, asegurandonos de que es bastante horizontal
float setAng(){
    float ballAngle = 0;
    
    srand(time(NULL));
    do{ ballAngle = (rand() % 360) * 2 * pi / 360; }
    while (abs(cos(ballAngle)) < 0.97f);
    
    return ballAngle;
}

//Inicializamos los mensajes (Puntos, vida y veces que hemos golpeado la pelota seguidas)
void initText(Text *points, Text *life, Text *roundHits){
    //Inicializar los puntos
    points->setFont(res.getFont(res.myFont));
    points->setCharacterSize(40);
    points->setPosition(75, 10);
    points->setColor(Color::White);
    points->setString("Puntos: 0");
    
    //Inicializar las fuentes
    life->setFont(res.getFont(res.myFont));
    life->setCharacterSize(40);
    life->setPosition(375, 10);
    life->setColor(Color::White);
    life->setString("Vidas: 3");
    
    //Inicializar las veces que lo hemos golpeado
    roundHits->setFont(res.getFont(res.myFont));
    roundHits->setCharacterSize(40);
    roundHits->setPosition(675, 10);
    roundHits->setColor(Color::White);
    roundHits->setString("Golpes: 0");
}

//Actualizamos los marcadores con los nuevos datos
void updateText(Text *points, Text *life, Text *roundHits, int pointsValue, int lifeValue, int roundHitsValue){
    stringstream p, l, r;
    
    p << pointsValue;
    l << lifeValue;
    r << roundHitsValue;
    
    points->setString("Puntos: "+ p.str());
    life->setString("Vidas: " + l.str());
    roundHits->setString("Golpes: " + r.str());
}

//Inicializamos las características iniciales de nuestros GameObjects
void initGameObjects(RectangleShape *lPaddle, RectangleShape *rPaddle, CircleShape *ball, Sprite *background){
    
    //Nos traemos las texturas
    Texture* objTexture = new Texture();
    *objTexture = res.getTexture(res.myTexture);
    
    Texture* bgTexture = new Texture();
    *bgTexture = res.getTexture(res.myBackground);
    
    //Configurar la pala Izda.
    lPaddle->setSize(paddleSize);
    lPaddle->setFillColor(Color::White);
    lPaddle->setOrigin(paddleSize / 2.f);
    lPaddle->setPosition(30 + paddleSize.x / 2, wHeight / 2);
    lPaddle->setTexture(objTexture);
    lPaddle->setTextureRect(IntRect(0, 0, paddleSize.x, paddleSize.y));

    //configurar la pala Dcha.
    rPaddle->setSize(paddleSize);
    rPaddle->setFillColor(Color::White);
    rPaddle->setOrigin(paddleSize / 2.f);
    rPaddle->setPosition(wWidth - 30 - paddleSize.x / 2, wHeight / 2);
    rPaddle->setTexture(objTexture);
    rPaddle->setTextureRect(IntRect(paddleSize.x, 0, paddleSize.x, paddleSize.y));

    //Configurar la bola
    ball->setRadius(ballRadius);
    ball->setFillColor(Color::White);
    ball->setOrigin(ballRadius / 2, ballRadius / 2);
    ball->setPosition(wWidth / 2, wHeight / 2);
    ball->setTexture(objTexture);
    ball->setTextureRect(IntRect(2 * paddleSize.x, 0, 19, 19));
    
    //Configurar background;
    background->setTexture(*bgTexture);
}

//Renderizamos
void renderGameObjects( RenderWindow *window, RectangleShape lPaddle, RectangleShape rPaddle, CircleShape ball,
                        Sprite background, Text points, Text life, Text roundHits){   
    window->clear(Color::Black);
    
    window->draw(background);
    
    window->draw(lPaddle);
    window->draw(rPaddle);
    window->draw(ball);
    
    window->draw(points);
    window->draw(life);
    window->draw(roundHits);
    
    window->display();
}

//Mover la bola
void moveBall(  CircleShape *ball, RectangleShape lPaddle, RectangleShape rPaddle, float *ballAngle,
                float deltaTime, int *pointsValue, int *lifeValue, int *roundHitsValue){
    
    //Detectar colisiones con paredes
    if (ball->getPosition().x - ballRadius < 0){
        ball->setPosition(wWidth / 2, wHeight / 2);
        *ballAngle = setAng();
        *lifeValue = *lifeValue - 1;
        *roundHitsValue = 0;
        factorBallSpeed = 1;
        ballSpeed = initBallSpeed;
        mus.playSound(mus.deathSound);
    }
    if (ball->getPosition().x + ballRadius > wWidth){
        ball->setPosition(wWidth / 2, wHeight / 2);
        *ballAngle = setAng();
        *pointsValue = *pointsValue +1;
        factorBallSpeed = 1;
        ballSpeed = initBallSpeed;
        mus.playSound(mus.deathSound);
    }
    if (ball->getPosition().y - ballRadius < 0){
        *ballAngle = -*ballAngle;
         ball->setPosition(ball->getPosition().x, ballRadius + 0.1);
         mus.playSound(mus.hitSound);
    }
    if (ball->getPosition().y + ballRadius > wHeight){
        *ballAngle = -*ballAngle;
        ball->setPosition(ball->getPosition().x, wHeight - ballRadius - 0.1f);
        mus.playSound(mus.hitSound);
    }
    
    //Detectar colisiones con pala izquierda
    if (ball->getPosition().x - ballRadius < lPaddle.getPosition().x + paddleSize.x / 2 &&
        ball->getPosition().x - ballRadius > lPaddle.getPosition().x &&
        ball->getPosition().y + ballRadius >= lPaddle.getPosition().y - paddleSize.y / 2 &&
        ball->getPosition().y - ballRadius <= lPaddle.getPosition().y + paddleSize.y / 2)
    {
        *roundHitsValue = *roundHitsValue + 1;
        
        float relativeIntersect = (lPaddle.getPosition().y - ball->getPosition().y);
        float normalizedRI = relativeIntersect/(paddleSize.y/2);
        
        //Ángulo máximo en GRADOS(70) * factor de ángulo * (CONVERSION A RADIANES))
        *ballAngle = ((-70) * normalizedRI) * 2 * pi / 360;
        
        factorBallSpeed = abs(normalizedRI)*2;
        if(factorBallSpeed < 1) factorBallSpeed = factorBallSpeed-(factorBallSpeed-1*factorChangeSpeed);
        else if (factorBallSpeed > 1) factorBallSpeed = factorBallSpeed+(1-factorBallSpeed*factorChangeSpeed);
        
        ball->setPosition(lPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball->getPosition().y);
        mus.playSound(mus.hitSound);
    }
    
    //Detectar colisiones con pala derecha
    if (ball->getPosition().x + ballRadius > rPaddle.getPosition().x - paddleSize.x / 2 &&
        ball->getPosition().x + ballRadius < rPaddle.getPosition().x &&
        ball->getPosition().y + ballRadius >= rPaddle.getPosition().y - paddleSize.y / 2 &&
        ball->getPosition().y - ballRadius <= rPaddle.getPosition().y + paddleSize.y / 2)
    {
        if (ball->getPosition().y > rPaddle.getPosition().y) *ballAngle = pi - *ballAngle + (rand() % 20) * pi / 180;
        else *ballAngle = pi - *ballAngle - (rand() % 20) * pi / 180;
        
        float relativeIntersect = (rPaddle.getPosition().y - ball->getPosition().y);
        float normalizedRI = relativeIntersect/(paddleSize.y/2);
        
        //Ángulo máximo en GRADOS(70) * factor de ángulo * (CONVERSION A RADIANES))
        //Le sumamos 180 para darle la vuelta
        *ballAngle = (((70 * normalizedRI)+180) * (2 * pi / 360));
        
        factorBallSpeed = abs(normalizedRI)*2;
        if(factorBallSpeed < 1) factorBallSpeed = factorBallSpeed-(factorBallSpeed-1*factorChangeSpeed);
        else if (factorBallSpeed > 1) factorBallSpeed = factorBallSpeed+(1-factorBallSpeed*factorChangeSpeed);
        ball->setPosition(rPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball->getPosition().y);
        mus.playSound(mus.hitSound);
    }
    
    //Mover
    ballSpeed = ballSpeed * factorBallSpeed;
    factorBallSpeed = 1;
    if(ballSpeed > maxBallSpeed) ballSpeed = maxBallSpeed;
    if(ballSpeed < minBallSpeed) ballSpeed = minBallSpeed;
    
    float frameMovement = ballSpeed * deltaTime;
    if(*lifeValue > 0) ball->move(cos(*ballAngle) * frameMovement, sin(*ballAngle) * frameMovement);
}

//Mover la paleta de la CPU
void moveCPU(RectangleShape *rPaddle, float *rPaddleSpeed, CircleShape ball, float deltaTime, Clock *updateClock){  
    
    //Mover arriba o abajo dependiendo de si tenemos velocidad positiva o negativa
    if(rPaddleSpeed != 0){
        if(rPaddle->getPosition().y + paddleSize.y / 2 < wHeight - 5) rPaddle->move(0, *rPaddleSpeed * deltaTime);
        else if(rPaddle->getPosition().y - paddleSize.y / 2 > 5) rPaddle->move(0, *rPaddleSpeed * deltaTime);
    }

    //Cambia de dirección
    if(updateClock->getElapsedTime() > updateTime){
        updateClock->restart();
        if (*rPaddleSpeed != cpuSpeed && ball.getPosition().y + ballRadius > rPaddle->getPosition().y + paddleSize.y / distFromCenter) *rPaddleSpeed = cpuSpeed;
        else if (*rPaddleSpeed != -cpuSpeed && ball.getPosition().y - ballRadius < rPaddle->getPosition().y - paddleSize.y / distFromCenter) *rPaddleSpeed = -cpuSpeed;
        else if (*rPaddleSpeed != 0) *rPaddleSpeed = 0;
    }
    
}

//Mover al jugador
void movePlayer(RectangleShape *lPaddle){
    Mouse mouse;
    lPaddle->setPosition(lPaddle->getPosition().x, (float)mouse.getPosition().y-(paddleSize.y/2));
}

int main(){
    //Crear Ventana
    RenderWindow window(VideoMode(wWidth, wHeight, 32), "FVPong", Style::Titlebar | Style::Close);
    window.setVerticalSyncEnabled(true);
    
    //Inicializar y cargar los recursos
    res.initialise();
    mus.initialise();
    
    //Crear Game Objects y Variables
    RectangleShape lPaddle, rPaddle;
    CircleShape ball;
    Sprite background;

    Text points, life, roundHits; 
    int pointsValue, roundHitsValue = pointsValue = 0;
    int lifeValue = 3;

    Clock deltaClock;
    Clock updateClock;

    float rPaddleSpeed = 0;
    float ballAngle = setAng();
    
    //Configiguración inicial de GameObject
    initGameObjects(&lPaddle, &rPaddle, &ball, &background);
    initText(&points, &life, &roundHits);
        
    //Empezar Partida
    while (window.isOpen()){
        //Definimos un deltaTime para interpolar el movimiento        
        float deltaTime = deltaClock.restart().asSeconds();
        
        Event event;
        while (window.pollEvent(event)){
            //Movimiento Jugador
            movePlayer(&lPaddle);            
            //Si pulsamos ESCAPE, salir del bucle
            if ((event.type == Event::Closed) || ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))) window.close();
        }
        
        //Movimiento CPU
        moveCPU(&rPaddle, &rPaddleSpeed, ball, deltaTime, &updateClock);
        //Movimiento Bola
        moveBall(&ball, lPaddle, rPaddle, &ballAngle, deltaTime, &pointsValue, &lifeValue, &roundHitsValue);
        
        //Renderizar
        updateText(&points, &life, &roundHits, pointsValue, lifeValue, roundHitsValue);
        renderGameObjects(&window, lPaddle, rPaddle, ball, background, points, life, roundHits);
    }
    
    res.cleanUp();
    mus.cleanUp();
    return 0;
}