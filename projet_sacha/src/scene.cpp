#include "scene.hpp"
#include "models_textures.hpp"




using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 2.0f,-2.0f,1.0f }, { 0,0,0 });


	// Create the hierarchy
	// ************************************ //

	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable roue1;
	mesh_drawable roue2;
	mesh_drawable pneu;
	mesh_drawable cylindre_vis;
	mesh_drawable rond_vis;
	mesh_drawable piston;
	mesh_drawable piston02;
	mesh_drawable rond_vis2;


	





	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	pneu.initialize(cylinder_with_texture(1.0,0.1), "pneu");
	roue1.initialize(disc_with_texture(1.0), "roue1");
	roue2.initialize(disc_with_texture(1.0), "roue2");
	cylindre_vis.initialize(cylinder_with_texture(0.1,0.1), "cylindre_vis");
	rond_vis.initialize(disc_with_texture(0.1), "rond_vis");
	piston.initialize(mesh_primitive_quadrangle(vec3{0, 0, 0}, vec3{1, 0, 0}, vec3{1, 0.1, 0}, vec3{0, 0.1, 0}), "piston");
	piston2.initialize(mesh_primitive_quadrangle(vec3{0, 0, 0}, vec3{1, 0, 0}, vec3{1, 0.1, 0}, vec3{0, 0.1, 0}), "piston2");
	rond_vis2.initialize(disc_with_texture(0.1), "rond_vis2");




	// color

	vec3 color1 = { 0, 0, 0 };
	vec3 color2 = { 1, 0.5f, 0 };
	pneu.shading.color = color2;
	rond_vis2.shading.color = color2;


	

//	roue1.shading.phong = { 0.4f, 0.6f,0,1 };
//	roue2.shading.phong = { 0.4f, 0.6f,0,1 };


	// Associate the texture_image_id to the image texture used when displaying visual
	


	roue1.texture = opengl_load_texture_image("assets/roue.png");
	roue2.texture = opengl_load_texture_image("assets/roue.png");

	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.
	hierarchy.add(pneu);
	hierarchy.add(roue2, "pneu",{ 0,0,-0.05f });
	hierarchy.add(roue1, "pneu",{ 0,0,0.05f });
	hierarchy.add(cylindre_vis, "roue1", { 0.3f,0.15,0.05f }); 
	hierarchy.add(rond_vis, "cylindre_vis",{ 0,0,0.05f });
	hierarchy.add(piston, "rond_vis");
	hierarchy.add(rond_vis2, "piston",{1,0,0 });
	hierarchy.add(piston2, "rond_vis2");



}





void scene_structure::display()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	//glDepthMask(false);
	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();
	hierarchy["pneu"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, (timer.t*2)/5);
	//rond_vis2.position[0]=0;
	//hierarchy["piston"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, std::sin(timer.t*2)/5);


/*
	// Apply transformation to some elements of the hierarchy
	hierarchy["head"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, std::sin(timer.t*2)/5);
	hierarchy["Wing 1 L"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, std::sin(timer.t*5)/2);
	hierarchy["Wing 2 L"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, std::sin(timer.t*5)/1.5f);
	hierarchy["Wing 1 R"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -std::sin(timer.t*5)/2);
	hierarchy["Wing 2 R"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -std::sin(timer.t*5)/1.5f);
	hierarchy["eye 1"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, std::sin(timer.t*2)/5);
	hierarchy["eye 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, std::sin(timer.t*2)/5);

*/




	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	if (gui.display_wireframe)
		draw_wireframe(hierarchy, environment);

	//glDepthMask(true);
	glDisable(GL_BLEND);
}

/*
void scene_structure::display_semiTransparent()
{
	// Enable use of alpha component as color blending for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);


	// Re-orient the grass shape to always face the camera direction
	vec3 const front = normalize(environment.camera.front() * vec3 { 1, 1, 0 }); // front-vector of the camera without z-component
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);
	quad_1.transform.rotation = R;


	// Sort transparent shapes by depth to camera
	//   This step can be skipped, but it will be associated to visual artifacts

	// Transform matrix (the same matrix which is applied in the vertices in the shader: T = Projection x View)
	mat4 T = environment.projection.matrix() * environment.camera.matrix_view();
	// Projected vertices (center of quads) in homogeneous coordinates
	vec4 p1 = T * vec4{ 0, -0.5f, 0, 1 };
	vec4 p2 = T * vec4{ 0, +0.5f, 0, 1 };
	// Depth to camera
	float z1 = p1.z / p1.w;
	float z2 = p2.z / p2.w;

	// Display the quads relative to their depth
	if (z1 <= z2) {
		draw(quad_2, environment);
		draw(quad_1, environment);
	}
	else {
		draw(quad_1, environment);
		draw(quad_2, environment);
	}



	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}

*/


void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}
