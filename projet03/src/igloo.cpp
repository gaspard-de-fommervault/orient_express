#include "igloo.hpp"

using namespace cgp;

void igloo::initializeUnIgloo(float h, float l, float taux){

    mesh mesh_mur = mesh_primitive_ellipsoid({h*1.4,h*taux,h}, {0,0,0}, 10, 10);
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
    std::list<igloo> listeIgloo;
    for (vec3& position : positions){
        float h = rand_interval(1.5, 3.0);
        float taux = rand_interval(1, 2);
        igloo igloo1;
        igloo1.initializeUnIgloo( h, h/2, taux);
        igloo1.mur.transform.translation=position;
        listeIgloo.push_back(igloo1);
    }
    std::cout<< "je suis ici" <<std::endl;
    return listeIgloo;
}

void display_igloo(std::list<igloo> vecIgloo, const cgp::scene_environment_basic_camera_spherical_coords environment2){
    for (igloo igloo1 : vecIgloo){
        draw(igloo1.mur, environment2);
    }
}


hierarchy_mesh_drawable trouPoisson(){
    mesh_drawable trouPoisson;
    trouPoisson.initialize(trouPourPoisson(5.0, 1.0, 0.4),"trou poisson");  
    mesh_drawable fond;
    fond.initialize(disc_with_texture(1.0),"eau");
    
    GLuint const texture_lac = opengl_load_texture_image("assets/lac.jpeg",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
	
	trouPoisson.texture = texture_lac;

    fond.shading.color={1,1,1};
    hierarchy_mesh_drawable hierachy_trou;
    hierachy_trou.add(trouPoisson);
    hierachy_trou.add(fond,"trou poisson");

    return hierachy_trou;
}

void igloo::initializePeche(){
   // hierachy_trou = trouPoisson();

}