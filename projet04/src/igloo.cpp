#include "igloo.hpp"

using namespace cgp;

void igloo::initializeUnIgloo(float h, float l){

    mesh mesh_mur = mesh_primitive_ellipsoid({h*1.4,h*1.1,h}, {0,0,0}, 10, 10);
    mesh mesh_porte = cylinder_with_hole(h/2, h/2-0.4, h/4);
    mesh_porte.position+={0,0,h};
    mesh_mur.push_back(mesh_porte);
    mur.initialize(mesh_mur,"mur");

    mur.transform.rotation= rotation_transform::from_axis_angle({ 0,1,0 },Pi/2);

    GLuint const texture_igloo_id = opengl_load_texture_image("assets/igloo.jpeg",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
	
	mur.texture = texture_igloo_id;

}

std::list<igloo> generateIgloo(int N, std::vector<cgp::vec3> positions){
   // for (vec3& particle : particles){

}
