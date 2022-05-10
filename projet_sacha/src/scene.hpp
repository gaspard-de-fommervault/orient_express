#pragma once


#include "cgp/cgp.hpp"
#include "scene.hpp"


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = false;
};



// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                        // Standard GUI element storage

	// Timer used for the animation
	cgp::timer_basic timer;

	// The entire hierarchy
	cgp::hierarchy_mesh_drawable hierarchy;

	cgp::mesh_drawable wheel;
	cgp::mesh_drawable vis;
	cgp::mesh_drawable roue1;
	cgp::mesh_drawable roue2;
	cgp::mesh_drawable pneu;
	cgp::mesh_drawable rond_vis;
	cgp::mesh_drawable rond_vis2;
	cgp::mesh_drawable piston;
	cgp::mesh_drawable piston2;

	




	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
	void display_semiTransparent();


};





