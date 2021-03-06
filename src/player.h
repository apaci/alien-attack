/**
 * Class representing the player
 *
 * TODO:
 *  * refactor transform player to not need arguments
 */
#pragma once

#include <vector>
#include "renderable.h"
#include "obj.h"
#include "collision.h"
#include "projectiles.h"
#include "exhaust.h"
#include "matrix.h"

#include <SDL/SDL_mixer.h>

using namespace std;

class Exhaust;
class Projectiles;

class Player : public Renderable, public Collidable {

    public:
        Player(float altitude);
        void moveLaterally();
        void moveForward();
        void setCamera();
        float getRotationRad();
        float getRotation();
        float getAltitude();
        float getLateralDelta();
        float getSway();
        virtual void render();
        virtual void renderShadow() {};
        void transformPlayer(GLfloat rotation, GLfloat sway, GLfloat lateralDelta, GLfloat altitude);
        Matrix* getTransformationMatrix();
        static float scale;

        // Game related methods
        void increaseScore();
        unsigned int getScore();
        unsigned int getLives();

        // Collision method
        void checkOpponentCollision();

        // Get the position of the gun nozzle (for projectiles)
        float* getGunPosition();
        void fire();
        void checkOpponentHit();

        // Get the position and normal of the exhausts
        void getExhaustPosition(int vertex, float *position, float *normal);

        // Add a static member so we can get the player anywhere
        static Player *player;

        // Movement
        void moveLeft();
        void moveRight();
        void cancelMoveLeft();
        void cancelMoveRight();

    private:
        float _altitude;
        float _rotation;
        float _lateralDelta;
        float _sway;
        float _swayDelta;
        float _maxSway;
        Obj *_model;
        list< Exhaust* > _exhauseParticleSystems;
        Projectiles *_gunParticleSystem;
        void _renderLights();
        float *_modelViewMatrix;
        Matrix *_transformationMatrix;

        // Game members
        unsigned int _score;
        unsigned int _lives;

        // Helpers
        void _getTransformedVertex(unsigned int vertex, float *position);

        // Movement deltas
        bool _moveLeft;
        bool _moveRight;
        float _movementDelta;

        GLuint _spriteTexture;

        // Sounds
        Mix_Chunk *_fireSound;
        Mix_Chunk *_engineSound;
};
