#pragma once

#include "cgp/cgp.hpp"

#include "terrain2.hpp"

#include <list> // utilise pour le stockage des positions - vitesses des flocons

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame      = false;
	bool display_wireframe  = false;
};

struct particle_structure
{
	cgp::vec3 p; // Position
	cgp::vec3 v; // Speed
	float scale;
};


// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                       // Standard GUI element storage
	
	// montagne
	cgp::mesh terrain_mesh;
	cgp::mesh_drawable terrain_drawable;
	perlin_noise_parameters parameters;

	// flocons
	std::list<particle_structure> particles; // Storage of all currently active particles
	cgp::mesh_drawable sphere;

	// points de controle
	cgp::mesh_drawable sphere2;
	std::array<cgp::vec2, 18> pts_controle;

	// billboards flocons
	cgp::mesh_drawable quad_1;

	// Timer used for the interpolation of the position
	cgp::timer_event_periodic timer;

	// skybox
	cgp::skybox_drawable skybox;

	

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

	// Display function for semi-transparent shapes
	void display_semiTransparent();

};





