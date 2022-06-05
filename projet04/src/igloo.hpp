#pragma once

#include "cgp/cgp.hpp"
#include "models_textures.hpp"

#include <list> 


struct igloo{

    cgp::hierarchy_mesh_drawable hierarchy_igloo;
    cgp::mesh_drawable mur;
    cgp::mesh_drawable porte;
	void initializeUnIgloo(float h, float l);

};

std::list<igloo> generateIgloo(int N, std::vector<cgp::vec3> positions);
