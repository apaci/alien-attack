#include "player.h"
#include "obj.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <math.h>
#include <iostream>

#define PI 3.14159265

using namespace std;

Player::Player(float altitude) {
    this->_altitude = altitude;
    this->_rotation = 0;
    this->_lateralDelta = 0;

    Obj* obj = Obj::makeObj("resources/fighter/SFighter.obj");
    this->_model = obj;

    // Set up the sway
    this->_swayDelta = 0.05;
    this->_maxSway = 2;
    this->_sway = 0;

    // Set up the particle system
    vector<GLfloat> lightVertex = this->_model->getVertex(1375);
    vector<GLfloat> leftExaust = this->_model->getVertex(1019);
    vector<GLfloat> rightExaust = this->_model->getVertex(2496);
    vector<GLfloat> normal; normal.push_back(0); normal.push_back(0); normal.push_back(-1);
    ParticleSystem *particleSystem = new ParticleSystem(0.5);
    particleSystem->updatePosition(leftExaust, normal);
    this->_particleSystems.push_back(particleSystem);

    particleSystem = new ParticleSystem(0.5);
    particleSystem->updatePosition(rightExaust, normal);
    this->_particleSystems.push_back(particleSystem);
}

void Player::moveLaterally(float lateralMovement) {
    this->_lateralDelta += lateralMovement;
}

void Player::moveForward() {
    this->_rotation += 1;
    if (this->_rotation >= 360) this->_rotation -= 360;

    // Increment the sway
    this->_sway += this->_swayDelta;
    //if (this->_sway > this->_maxSway) this->_swayDelta * -1;
}

void Player::setCamera() {
    // Rotate the camera up
    glRotatef(-45, 1, 0, 0);

    // Get the position of the camera
    glTranslatef(0, 0, -1 * (this->_altitude));

    // Move right / left
    glRotatef(-1 * this->_lateralDelta * 2 / 3, 0, 1, 0);

    // Move the camera up
    glRotatef(-90, 0, 0, 1);

    // Move the camera for the movement along
    glRotatef(this->_rotation, 0, 1, 0);
}

void Player::render() {
    // Move the model to the right place
    glPushMatrix();
    glRotatef(-1 * this->_rotation - 7, 0, 1, 0);

    // Sway the craft
    float sway = sin(this->_sway - 1) / 2;
    glRotatef(-1 * this->_lateralDelta + sway, 1, 0, 0);

    glTranslatef(0, 0, this->_altitude - 2);
    glRotatef(90, 1, 0, 0);
    //glRotatef(-90, 0, 1, 0);
    glRotatef(-90, 0, 1, 0);
    GLfloat scale = 0.00002;
    glScalef(scale, scale, scale);

    // Get the right wing
    glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);
    glPushAttrib(GL_TEXTURE_BIT);
    vector<GLfloat> lightVertex = this->_model->getVertex(1375);
    GLfloat tmp[4];
    tmp[0] = lightVertex[0];
    tmp[1] = lightVertex[1];
    tmp[2] = lightVertex[2];
    tmp[3] = 1;
    glTranslatef(tmp[0], tmp[1], tmp[2]);
    glLightfv(GL_LIGHT2, GL_POSITION, tmp);
    glColor3f(1, 0, 0);
    GLfloat emission[] = { 1, 0, 0, 0 };
    GLfloat emission2[] = { 1, 0, 0, 0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, emission2);
    glutSolidSphere(500, 5, 5);
    glPopMatrix();
    glPushMatrix();

    lightVertex = this->_model->getVertex(2811);
    tmp[0] = lightVertex[0];
    tmp[1] = lightVertex[1];
    tmp[2] = lightVertex[2];
    tmp[3] = 1;
    glTranslatef(tmp[0], tmp[1], tmp[2]);
    glLightfv(GL_LIGHT2, GL_POSITION, tmp);
    glColor3f(0, 1, 0);
    GLfloat emission3[] = { 0, 1, 0, 0 };
    GLfloat emission4[] = { 0, 1, 0, 0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission4);
    glMaterialfv(GL_FRONT, GL_AMBIENT, emission3);
    glutSolidSphere(500, 5, 5);
    glPopAttrib();
    glPopAttrib();
    glPopMatrix();

    // Render particle systems
    list< ParticleSystem * >::iterator it = this->_particleSystems.begin();
    for (; it != this->_particleSystems.end(); ++it) {
        (*it)->render();
    }

    this->_model->render();
    glPopMatrix();
}

float Player::getRotationRad() {
    return this->_rotation * PI / 180;
}