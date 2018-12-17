//
//  geometry.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

////signed distance function - sphere
//void geo_sdf(struct point *pt)
//{
//    float r = 1;                              //radius
////    float p = 4;
//
//    pt->v = (float3_nrm(pt->x) - r);          //sphere
//
////    pt->v = (float3_nrmp(pt->x,p) - r);         //p-sphere
//
//    return;
//}


////signed distance function - torus
void geo_sdf(struct point *pt)
{
    double R = 0.7;
    double r = 0.3;
    
    pt->v = powf(powf(pt->x[0],2) +
                    powf(pt->x[1],2) +
                    powf(pt->x[2],2) +
                    powf(R,2) - powf(r,2),2) - 4*powf(R,2)*(powf(pt->x[0],2) + powf(pt->x[1],2));
    
    //   printf("prb_lvl   %f\n",pt->eval);
    
    return;
}
