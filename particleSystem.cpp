#pragma warning(disable : 4786)

#include "particleSystem.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
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
	// TODO

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
    createNewParticles(1.0, Vec3d(10,10,0), Vec3d(2,0,0));
    CheckDeath checkDeath(t-_prevT);
    std::vector<Particle>::iterator newEnd = std::remove_if(particles.begin(),particles.end(),checkDeath);
    particles.erase(newEnd,particles.end());

    for(std::vector<Particle>::iterator it = particles.begin();it!=particles.end();++it){
        (*it)._position = (*it)._position + (t-_prevT)*((*it)._velocity);
        //Need all forces here
        (*it)._forces = Vec3d(0.0f,0.0f,0.0f);
        (*it)._forces += gravity(*it);
        (*it)._velocity = (*it)._velocity + (t-_prevT) * (((*it)._forces)/((*it)._mass));

    }

    _prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
    for(std::vector<Particle>::iterator it = particles.begin();it!=particles.end();++it){
        glPushMatrix();
            glTranslatef((*it)._position[0],(*it)._position[1],(*it)._position[2]);
            drawSphere(0.2);
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

void ParticleSystem::createNewParticles(float particle_count, Vec3d initial_velocity, Vec3d initial_position){
    std::cout<<"CURRENT PARTICLES"<< particles.size()<<"\n";
    for(int i = 0;i < particle_count; i++){
        Particle p = Particle(initial_position,initial_velocity,Vec3d(0.0f,0.0f,0.0f),1,10);
        // TODO RANDOM PERTURBATION TO VELOCITY
        // TODO RANDOM INITIAL POSITION
        particles.push_back(p);
    }
}

Vec3d ParticleSystem::gravity(Particle p){
    return Vec3d(0.0f,p._mass*-9.80665,0.0f);}
