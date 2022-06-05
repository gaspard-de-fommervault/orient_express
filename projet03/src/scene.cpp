#include "scene.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	//skybox initialization (path contains 6 images (cube), with a certain order)
	skybox.initialize("assets/skybox_4/"); 
	
	
	// Basic set-up
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 0.0f,2.0f,110.0f }, { 0,0,0 });

	piston1.initialize();
	mont.initialize();
	//flocon1.initialize();
	perlin_noise_parameters parameters;
	//timer.event_period = 0.1f;
	//buffer<vec3> key_positions =
	//{ {-0.11f, 0.09f,0.1f}, {-0.12f, 0.5f,0}, {-0.3f, 0.55f,0.0f}, {-0.29f, 0.45f,0.0f}, {-0.27f, 0.35f,0.0f}, {-0.20f, 0.25f,0.0f}, {-0.17f, 0.18f,0.0f}, {-0.16f, 0.07f,0.0f}, {-0.18f, -0.03f,0.0f}, {-0.24f, -0.11f,0.0f}, {-0.39f, -0.22f,0.0f}, {-0.43, -0.27,0.0f}, {-0.44, -0.37,0.0f}, {-0.50, -0.47,0.0f}, {-0.53, -0.48,0.0f}, {-0.59, -0.49,0.0f}, {-0.64, -0.54,0.0f}, {-0.67, -0.65,0.0f} };

	buffer<vec3> key_positions =
	{ {-150, 85,0}, {-120, 65,0}, {-100, 55,0}, {-60, 45,0}, {-30, 35,0}, {0, 25,0}, {20, 18,0}, {30, 7,0}, {80, -3,0}, {115, -11,0}, {135, -22,0}, {170, -27,0}, {200, -37,0}, {230, -47,0}, {266, -48,0}, {300, -49,0}, {320, -54,0}, {380, -65,0} };
 
	for(int i=0;i<18;++i){
		float temp = key_positions[i][1];
		key_positions[i][1]=key_positions[i][0];
		key_positions[i][0]=temp;
	}

	for(int i=0;i<18;++i){
		key_positions[i][2]=evaluate_terrain_height_perlin(key_positions[i][0]/mont.terrain_longueur+0.5f,key_positions[i][1]/mont.terrain_longueur+0.5f, parameters);

		// probleme avec evlautateTerrainHeight
	}


	buffer<float> key_times = 
	{ 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0F, 14.0f, 15.0f, 16.0f, 17.0f };

	buffer<vec3> key_positions2=key_positions;
	// chemin1.initialize(key_positions,key_times);

	int N = key_times.size();
	timer.t_min = key_times[1];
	timer.t_max = key_times[N - 2];
	timer.t = timer.t_min;


	buffer<vec3> key_positions3=key_positions;
	for(int i=0;i<18;++i){
		key_positions3[i][1]=0;
	}

	mouvement = {0.1f,0.1f, timer.t_min+0.1f,0,0};

	vitesseMax = 0.2f;

	float hauteur = 1.0f;

	chemin1.createAllRail(1.0f, 0.1f, mont.terrain_mesh,key_positions,key_times, timer.t_min, timer.t_max, hauteur);
	

	chemin1.key_positions2 = key_positions2;
	chemin1.key_positions3 = key_positions3;

	vecIgloo = generateIgloo(20,  generate_positions_on_lac(20, mont));
		

	//trouPoisson.transform.translation = {key_positions[1][0]-10.0,key_positions[1][1],100};
	//chemin1.initialize2(mouvement,vitesseMax,hauteur);
}





void scene_structure::display()
{
	// skybox first => derriere les autres elements de la scene.
	draw(skybox, environment); 
	draw(chemin1.hierarchy_rail, environment); 


	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame){
		draw(global_frame, environment);
	}

	draw(trouPoisson, environment);
	draw(mont.terrain_drawable, environment);
	draw(mont.lac_drawable, environment);
	display_igloo(vecIgloo, environment);

	if (gui.display_wireframe){
		draw_wireframe(mont.terrain_drawable, environment);
	}

	// pts de controle
	for (int i=0; i<18; i++){
		mont.sphere2.transform.translation = chemin1.key_positions[i];
		//std::cout<< key_positions[i] <<std::endl;
		draw(mont.sphere2, environment);
	}
	// piston1.hierarchy["train"].transform.translation =vec3{mont.pts_controle[i][0], mont.pts_controle[i][1], mont.evaluate_terrain_height(mont.pts_controle[i][0], mont.pts_controle[i][1], mont.terrain_mesh)+0.01f};
	timer2.update();
	timer.update();

	float t = timer.t;

	piston1.tournerRoue(t);

	//flocon1.update();
	//flocon1.display_semiTransparent(environment);
	


	vec3 p = interpolation(mouvement[2], chemin1.key_positions, chemin1.key_times, mont.terrain_mesh);
	mouvement = piston1.acceleration(mouvement[0], mouvement[1], vitesseMax, 0.04f, mont.terrain_mesh, p, chemin1, mouvement[2], timer.t_min, timer.t_max,mouvement[3],mouvement[4]);


	// Display the interpolated position (and its trajectory)
	//piston1.update(p, environment, hauteur);
	//piston1.hierarchy["train"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 },Pi/4);
	draw(piston1.hierarchy, environment);


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


