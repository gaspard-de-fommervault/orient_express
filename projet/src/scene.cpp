#include "scene.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	//skybox initialization (path contains 6 images (cube), with a certain order)
	skybox.initialize("assets/skybox_4/"); 
	
	
	// Basic set-up
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 0.0f,2.0f,2.0f }, { 0,0,0 });

	piston1.initialize();
	mont.initialize();
	flocon1.initialize();

	//timer.event_period = 0.1f;


}






void scene_structure::display()
{
	// skybox first => derriere les autres elements de la scene.
	draw(skybox, environment); 

	global_frame.transform.scaling=100;

	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame){
		draw(global_frame, environment);
	}


	draw(mont.terrain_drawable, environment);
	if (gui.display_wireframe){
		draw_wireframe(mont.terrain_drawable, environment);
	}

	// pts de controle
	for (int i=0; i<18; i++){
		mont.sphere2.transform.translation = vec3{mont.pts_controle[i][0], mont.pts_controle[i][1], mont.evaluate_terrain_height(mont.pts_controle[i][0], mont.pts_controle[i][1], mont.terrain_mesh)+0.01f};
		draw(mont.sphere2, environment);
	}
	int i =12;
	piston1.hierarchy["train"].transform.translation =vec3{mont.pts_controle[i][0], mont.pts_controle[i][1], mont.evaluate_terrain_height(mont.pts_controle[i][0], mont.pts_controle[i][1], mont.terrain_mesh)+0.01f};
	timer2.update();
	float t = timer2.t;

	piston1.tournerRoue(t);
	draw(piston1.hierarchy, environment);

	flocon1.update();
	flocon1.display_semiTransparent(environment);

}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

/*

	// valeurs par defaut :	
	parameters.persistency = 0.574f;
	parameters.frequency_gain = 1.515f;
	parameters.octave = 15;
	parameters.terrain_height = 0.504f;
	bool update = false;

	update |= ImGui::SliderFloat("Persistance", &parameters.persistency, 0.1f, 0.6f);
	update |= ImGui::SliderFloat("Frequency gain", &parameters.frequency_gain, 1.5f, 2.5f);
	update |= ImGui::SliderInt("Octave", &parameters.octave, 1, 15);
	update |= ImGui::SliderFloat("Height", &parameters.terrain_height, 0.1f, 1.5f);

	if (update)// if any slider has been changed - then update the terrain
		update_terrain(terrain_mesh, terrain_drawable, parameters);

*/
}


