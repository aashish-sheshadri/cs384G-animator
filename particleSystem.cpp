#pragma warning(disable : 4786)

#include "particleSystem.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>
using namespace std;

static float prevT;
struct particle{
    vector<float> position();
    vector<float> velocity();
    vector<float> forces();
    float mass;
    float lifetime;
};
static vector<particle> particles();


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
    int numberOfParticles = 1;  // TODO hook it with GUI
                                // TODO perturb velocity


	// Debugging info
	if( t - prevT > .04 )
		printf("(!!) Dropped Frame %lf (!!)\n", t-prevT);
	prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// TODO
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

void createNewParticles(float particle_count, vector<float> initial_velocity, vector<float> initial_position){
    for(int i = 0;i < particle_count; i++){
        particle p = particle();
        p.lifetime=10;
        p.velocity[0] = initial_velocity[0];
        p.velocity[1] = initial_velocity[1];
        p.velocity[2] = initial_velocity[2];
        p.position[0] = initial_position[0];
        p.position[1] = initial_position[1];
        p.position[2] = initial_position[2];
        particles.
    }
}

vector<float> gravity(particle p){
    vector<float> toReturn();
    toReturn[0] = p.mass*0.0;
    toReturn[1] = p.mass*9.80665;
    toReturn[2] = p.mass*0.0;
}
