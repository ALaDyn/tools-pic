/*******************************************************************************
This file is part of tools_pic.

tools_pic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
tools-pic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with tools-pic.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <malloc.h>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <vector>
#include "utilities-tools.h"
#include <random>
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/variate_generator.hpp>
#define _DIMENSIONS 3

//typedef boost::uniform_real<> UniformRealDistribution;
//typedef boost::mt19937 MTGenerator;
//typedef boost::variate_generator<MTGenerator&,UniformRealDistribution> Generator;

#define MAX(x,y)	((x)>(y)?(x):(y))

struct PARTICLE{
  float coord[_DIMENSIONS];
  float radius;
};

struct GRID{
  float radius;
  float rMin[_DIMENSIONS], rMax[_DIMENSIONS];
  float step;
  float furthestX;
  float threshold;
};

void initializeParticles(PARTICLE *particle, GRID *grid){
  particle->radius = grid->radius;
  particle->coord[0] = grid->furthestX + grid->step*10;
  for(int i=1; i<_DIMENSIONS; i++){
    float buffer = (rand()*1.0/RAND_MAX);
    int Nx = (int)(grid->rMax[i] - grid->rMin[i])/grid->step;
    int xint = (int)(Nx*buffer);
    particle->coord[i] = grid->rMin[i] + xint*grid->step;
  }
}

void pushDownParticle(PARTICLE *particle, GRID *grid){
  particle->coord[0] -= grid->step;
}

void pushSideParticle(PARTICLE *particle, GRID *grid){
  float buffer = (rand()*1.0/RAND_MAX);

  float angle = 2*M_PI*buffer-M_PI;
  if(_DIMENSIONS == 2){
    particle->coord[1] += grid->step*angle/M_PI;
  }
  else if(_DIMENSIONS == 3){
    particle->coord[1] += grid->step*cos(angle);
    particle->coord[2] += grid->step*sin(angle);
  }
}

void pushSideParticleSteps(PARTICLE *particle, GRID *grid){
  float buffer = (rand()*1.0/RAND_MAX);

  float angle = 2*M_PI*buffer-M_PI;
  if(_DIMENSIONS == 2){
    if(buffer<=0.5)
      particle->coord[1] -= grid->step;
    else
      particle->coord[1] += grid->step;

  }
  else if(_DIMENSIONS == 3){
    if(buffer<=0.25)
      particle->coord[1] -= grid->step;
    else if(buffer<=0.5)
      particle->coord[1] += grid->step;
    else if(buffer<=0.75)
      particle->coord[2] -= grid->step;
    else
      particle->coord[2] += grid->step;

  }
}
void pushParticleSteps(PARTICLE *particle, GRID *grid){

  if(_DIMENSIONS == 2){
    float buffer = 1.0*(rand()*1.0/RAND_MAX);
    if(buffer<=0.45)
      particle->coord[0] -= grid->step;
    else if(buffer<=0.5)
      particle->coord[0] += grid->step;
    else if(buffer<=0.75)
      particle->coord[1] -= grid->step;
    else
      particle->coord[1] += grid->step;

  }
  else if(_DIMENSIONS == 3){
    float buffer = 3.0*(rand()*1.0/RAND_MAX);
    if(buffer<=0.55)
      particle->coord[0] -= grid->step;
    else if(buffer<=1.0)
      particle->coord[0] += grid->step;
    else if(buffer<=1.5)
      particle->coord[1] -= grid->step;
    else if(buffer<=2.0)
      particle->coord[1] += grid->step;
    else if(buffer<=2.5)
      particle->coord[2] -= grid->step;
    else
      particle->coord[2] += grid->step;

  }
}

void checkBoundaries(PARTICLE *particle, GRID *grid){
  for(int d=1; d<_DIMENSIONS; d++){
    if(particle->coord[d] < grid->rMin[d]){
      particle->coord[d] += grid->rMax[d] - grid->rMin[d];
    }
    if(particle->coord[d] >= grid->rMax[d]){
      particle->coord[d] -= grid->rMax[d] - grid->rMin[d];
    }
  }
}

float distance2(PARTICLE *part1, PARTICLE *part2){
  float r2=0;
  for(int i=0; i<_DIMENSIONS; i++){
    r2 +=  (part1->coord[i]-part2->coord[i])*(part1->coord[i]-part2->coord[i]);
  }
  return r2;
}

bool isParticleTouching(PARTICLE *particle, std::vector<PARTICLE> *foam, GRID &grid){
  float dist;
  //std::vector<PARTICLE>::const_iterator iterator;
  if(particle->coord[0]<(particle->radius))
    return true;
  for (std::vector<PARTICLE>::reverse_iterator i = foam->rbegin(); i != foam->rend(); ++i){
    dist = distance2( particle, &(*i) );
    if(dist < ( grid.threshold + particle->radius + (*i).radius) * (grid.threshold + particle->radius + (*i).radius) )
      return true;

  }
return false;
}

int main(int narg, char **args){
  std::vector<PARTICLE> foam;
  std::vector<PARTICLE>::const_iterator iterator;
  GRID grid;
  PARTICLE particle;

  //UniformRealDistribution distribution(0, 1.0);
  //MTGenerator generator;
  //Generator numberGenerator(generator, distribution);
  //generator.seed(0);
  //grid.myGenerator = numberGenerator;

  for(int i=0; i<_DIMENSIONS; i++){
    grid.rMin[i] = 0.0;
    grid.rMax[i] = 4.0;
  }
  grid.rMax[0] = 5;
  grid.step = 0.1;
  grid.radius = 0.05;
  grid.furthestX = 0;
  grid.threshold = 0.01;

  std::cout << "  grid.step = " << grid.step << "  grid.radius = " << grid.radius << "\n";
  std::cout << "  foam.size()  = " << foam.size() << "    particle.coord[0] = " << particle.coord[0] << std::endl;
  for(int i=0; i<_DIMENSIONS; i++){
    std::cout << "  grid.rMin[" << i << "]  = " << grid.rMin[i] << "  grid.rMax[" << i << "]  = " << grid.rMax[i] << std::endl;
  }
  initializeParticles(&particle, &grid);

  std::cout << "  foam.size()  = " << foam.size() << "    particle.coord[0] = " << particle.coord[0] << std::endl;
  std::cout << "  foam.size()  = " << foam.size() << "    particle.coord[0] = " << particle.coord[0] << std::endl;

  while(foam.size()<10000){
    while(1){
#ifdef OLD
      pushDownParticle(&particle,&grid);
      if(isParticleTouching(&particle,&foam, grid) ){
        foam.push_back(particle);
        grid.furthestX = MAX(grid.furthestX,particle.coord[0]);
        break;
      }

      pushSideParticleSteps(&particle,&grid);
      checkBoundaries(&particle, &grid);
      if(isParticleTouching(&particle,&foam, grid) ){
        foam.push_back(particle);
        grid.furthestX = MAX(grid.furthestX,particle.coord[0]);
        break;
      }
#else
      pushParticleSteps(&particle,&grid);
      checkBoundaries(&particle, &grid);
      if(isParticleTouching(&particle,&foam, grid) ){
        foam.push_back(particle);
        grid.furthestX = MAX(grid.furthestX,particle.coord[0]);
        break;
      }
#endif

    }

    if(!(foam.size()%100)){
      std::cout << "size = " << foam.size() << std::endl;
    }
    initializeParticles(&particle, &grid);

  }
  float boxVolume =1, foamVolume = 0;
  grid.rMax[0] = grid.furthestX;
  for(int i=0; i<_DIMENSIONS; i++){
    boxVolume *= (grid.rMax[i]-grid.rMin[i]);
  }
  if(_DIMENSIONS == 2){
//    for (int p=0; p < foam.size(); p++){
//      foamVolume += M_PI*foam[p].radius*foam[p].radius;
//    }
     foamVolume = foam.size()*M_PI*grid.radius*grid.radius;
  }
  if(_DIMENSIONS == 3){
//    for (int p=0; p < foam.size(); p++){
//      foamVolume += *4.0/3.0*M_PI*foam[p].radius*foam[p].radius*foam[p].radius;
//    }
     foamVolume = foam.size()*4.0/3.0*M_PI*grid.radius*grid.radius*grid.radius;
  }

  std::cout << "   filling factor = " << foamVolume/boxVolume << std::endl ;
  std::ofstream of1;
  of1.open("pippo.xyz", std::ofstream::out | std::ofstream::trunc);
	of1 << foam.size() << std::endl;
	of1 << "La schiuma dello Sgatto" << std::endl;
  for (int p=0; p < foam.size(); p++){
    of1 << "A ";
		for(int i=0; i<_DIMENSIONS; i++){
      of1 << foam[p].coord[i] << " ";
    }
    of1 <<  std::endl;
  }
  of1.close();
  return 0;
}
