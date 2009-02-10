#include "opponent.h"
#include <iostream>

using namespace std;

const float Opponent::generateProbability = 0.3;
const unsigned int Opponent::maxOpponents = 20;
list< Opponent* > Opponent::opponents;

Opponent::Opponent() {
    // Load the model
    this->_model = Obj::makeObj("resources/missile/GBU-12-reduced.obj");

    // Set up the constants
    this->_altitude = 12.6;
    this->_rotation = rand() % 360;
    this->_lateralRotation = 10 - rand() % 20;

}

void Opponent::render() {
    float* modelViewMatrix = new float[16];
    float scale = 0.25;

    glPushMatrix();
    glRotatef(-1 * this->_rotation - 7, 0, 1, 0);
    glRotatef(-1 * this->_lateralRotation, 1, 0, 0);
    glTranslatef(0, 0, this->_altitude - 2.5);
    glRotatef(90, 0, 0, 1);
    glScalef(scale, scale, scale);

    // Save the modelview matrix before rendering
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

    this->_model->render();
    glPopMatrix();

    // Calculate the bounds while we have the modelview matrix
    this->_model->calculateBounds(modelViewMatrix);

    // clean up
    delete modelViewMatrix;
}

void Opponent::moveForward() {
    this->_rotation += 0.2;
}

Opponent* Opponent::generate() {
    if (Opponent::opponents.size() < Opponent::maxOpponents) {
        float prob = ( rand() % 100 ) / 100.0;
        if ( prob > Opponent::generateProbability) {
            Opponent* opp = new Opponent();
            Opponent::opponents.push_back(opp);
            return opp;
        }
    }
    return NULL;
}

/******************************************************************************
 * Collision Detection
 *****************************************************************************/
float* Opponent::getBounds() {
    return this->_model->getBounds();
}
