#pragma warning(disable : 4786)

#include "particleSystem.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "vec.h"
#include "mat.h"
using namespace std;

float _prevT;


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	// TODO
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	// TODO

	// These values are used by the UI ...
	// negative bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    // TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    particles.clear();
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	// Debugging info
    if( t - _prevT > .04 )
        printf("(!!) Dropped Frame %lf (!!)\n", t-_prevT);
    if(t-_prevT < 0.005)
        return;
    CheckDeath checkDeath(t-_prevT);
    std::vector<Particle>::iterator newEnd = std::remove_if(particles.begin(),particles.end(),checkDeath);
    particles.erase(newEnd,particles.end());

    for(std::vector<Particle>::iterator it = particles.begin();it!=particles.end();++it){
        //Need all forces here
        (*it)._forces = Vec3d(0.0f,0.0f,0.0f);
        (*it)._forces += gravity(*it);
        (*it)._forces += drag(*it);

        (*it)._position = (*it)._position + (t-_prevT)*((*it)._velocity);
        (*it)._velocity = (*it)._velocity + (t-_prevT) * (((*it)._forces)/((*it)._mass));

        (*it)._lifetime -= t-_prevT;
    }

    _prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
    if(!simulate)
        return;
    for(std::vector<Particle>::iterator it = particles.begin();it!=particles.end();++it){
        glPushMatrix();
        switch((*it)._weapon){
            case Weapons::CANNON_BALL:
                glTranslatef((*it)._position[0],(*it)._position[1],(*it)._position[2]);
                glScalef(0.1f, 0.1f, 0.1f);
                paintCannonBall();
            break;
            case Weapons::ICE_CUBE:

            break;
            case Weapons::ARROW:

            break;
        }
        glPopMatrix();
    }
}




/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO
}

void ParticleSystem::createNewParticles(float particle_count, Mat4f matrix, Vec3d head, Vec3d tail, double cannon_radius, double cannon_length){
    if(!simulate){
        return;
    }
    for(int i = 0;i < particle_count; i++){
        double rX, rY;
        do{
            rX = (2*(rand() / double(RAND_MAX))-1)*cannon_radius;
            rY = (2*(rand() / double(RAND_MAX))-1)*cannon_radius;
        }while(rX*rX + rY*rY > cannon_radius*cannon_radius);
        Vec4f start = matrix*Vec4f(rX, rY, 0.0,1.0);
        Particle p = Particle(Vec3d(start[0],start[1],start[2]),3*cannon_length*Vec3d(start[0]-tail[0],start[1]-tail[1],start[2]-tail[2]),Vec3d(0.0f,0.0f,0.0f),1,1,Weapons::CANNON_BALL);
        particles.push_back(p);
    }
}

Vec3d ParticleSystem::gravity(Particle p){
    return Vec3d(0.0f,p._mass*-9.80665,0.0f);}

Vec3d ParticleSystem::drag(Particle p){
    double k = 0.47;
    return Vec3d(-k*p._velocity);
}

void ParticleSystem::paintCannonBall(){
    drawSphere(1);
}
