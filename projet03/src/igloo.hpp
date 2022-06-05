#pragma once

#include "cgp/cgp.hpp"
#include "models_textures.hpp"

#include <list> 


struct igloo{

    cgp::hierarchy_mesh_drawable hierarchy_igloo;
    cgp::mesh_drawable mur;
    cgp::mesh_drawable porte;


    cgp::mesh_drawable trouPoisson;
    cgp::hierarchy_mesh_drawable hierachy_trou;

	void initializeUnIgloo(float h, float l, float taux);
    void initializePeche();

};

std::list<igloo> generateIgloo(int N, std::vector<cgp::vec3> positions);
void display_igloo(std::list<igloo> vecIgloo, const cgp::scene_environment_basic_camera_spherical_coords environment2);

