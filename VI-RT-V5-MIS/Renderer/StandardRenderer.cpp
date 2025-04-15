//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include <random>

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y, s;
    
    /****************************************
     
     Our Random Number Generator (rng) */
    std::random_device rdev{};
    std::mt19937 rng{rdev()};
    std::uniform_real_distribution<float>U_dist{0.0,1.0};  // uniform distribution in[0,1[
    
    // get resolution from the camera
    cam->getResolution(&W, &H);
    float const sppf = 1.f/spp;
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        fprintf (stderr,"%d\r",y);
        fflush (stderr);
        for (x=0 ; x< W ; x++) { // loop over columns
            RGB color(0.,0.,0.);
                        
            for (s=0 ; s < spp; s++) {
                Ray primary;
                Intersection isect;
                bool intersected;
                // Generate Ray (camera)
                float jitterV[2];
                
                if (jitter) {
                    jitterV[0] = U_dist(rng);
                    jitterV[1] = U_dist(rng);
                    cam->GenerateRay(x, y, &primary, jitterV);
                } else {
                    cam->GenerateRay(x, y, &primary);
                }
                
                // trace ray (scene)
                intersected = scene->trace(primary, &isect);
                
                // shade this intersection (shader) - remember: depth=0
                RGB const spp_color = shd->shade(intersected, isect, 0);
                color += spp_color;

                /*  DEBUGGING */
                
                //if (x==XX && (y<(YY-2) || y >(YY+2))) color.set (10.*spp,9.*spp,0.);
                //if ((x<(XX-2) || x > (XX+2)) && y==YY) color.set (10.*spp,9.*spp,0.);
                
            } // multiple samples
            // write the result into the image frame buffer (image)
            color *= sppf;
            img->set(x,y, color);
        } // loop over columns
    }   // loop over rows

}
