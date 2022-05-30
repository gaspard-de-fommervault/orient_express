#pragma once


#include "cgp/cgp.hpp"
#include "models_textures.hpp"


//struct gui_parameters {
//	bool display_frame = true;
//	bool display_wireframe = false;
//};

// The element of the GUI that are not already stored in other structures

struct piston{
    cgp::mesh_drawable train;
    cgp::mesh_drawable wheel1;
    cgp::mesh_drawable wheel2;
    cgp::mesh_drawable wheel3;
	cgp::mesh_drawable wheel1_2;
    cgp::mesh_drawable wheel2_2;
    cgp::mesh_drawable wheel3_2;
	cgp::mesh_drawable base1;
	cgp::mesh_drawable base2;


	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)
	cgp::hierarchy_mesh_drawable hierarchy;
	cgp::rotation_transform rot;

	//gui_parameters gui;                        // Standard GUI element storage

	// Timer used for the animation
	cgp::mesh_drawable biele3;
	cgp::mesh_drawable biele2;
	cgp::mesh_drawable vis1;
	cgp::mesh_drawable vis2;
	cgp::mesh_drawable vis3;

	cgp::mesh_drawable biele3_2;
	cgp::mesh_drawable biele2_2;
	cgp::mesh_drawable vis1_2;
	cgp::mesh_drawable vis2_2;
	cgp::mesh_drawable vis3_2;


	//cgp::hierarchy_mesh_drawable hierarchy;
    void initialize();
    void tournerRoue(float t);
	void avancer(float vitesse);


};

