#pragma once

#include "cgp/cgp.hpp"

struct perlin_noise_parameters
{
	float persistency = 0.574f;
	float frequency_gain = 1.515f;
	int octave = 15;
	float terrain_height = 0.504f;
};


// Initialize the mesh of the terrain
cgp::mesh create_terrain_mesh();

// donne l'altitude d'un point de coordonnees (x,y) 
float evaluate_terrain_height(float x, float y, cgp::mesh& terrain);

// Recompute the vertices of the terrain everytime a parameter is modified
//  and update the mesh_drawable accordingly
void update_terrain(cgp::mesh& terrain, cgp::mesh_drawable& terrain_visual, perlin_noise_parameters const& parameters);