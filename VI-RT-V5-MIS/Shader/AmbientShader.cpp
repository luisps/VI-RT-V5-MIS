//
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "AmbientShader.hpp"
#include "BRDF.hpp"
#include "AmbientLight.hpp"

RGB AmbientShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
    
    /*if (isect.pix_x==320 && isect.pix_y==240) {
        fprintf (stderr, "AMBIENT SHADER. intersected = %s !\n", (intersected?"TRUE":"FALSE"));
        fflush(stderr);
    }*/

    // if no intersection, return background
    if (!intersected) {
        return (background);
    }
    if (isect.isLight) { // intersection with a light source
        return isect.Le;
    }
    
    // verify whether the intersected object has an ambient component
    BRDF *f = isect.f;
    if (f->Ka.isZero()) return color;
    RGB Ka = f->Ka;

    // ambient shade
    // Loop over scene's light sources and process Ambient Lights
    for (auto l : scene->lights) {
        
        if (l->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            AmbientLight *al = (AmbientLight *)l;
            color += Ka * al->L();
            continue;
        }
    }
        
    return color;
};
