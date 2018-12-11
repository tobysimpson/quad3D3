//
//  geometry.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//signed distance function
void geo_sdf(struct point *pt)
{
    float r = 1;                              //radius
//    float p = 4;
    
    pt->v = (float3_nrm(pt->x) - r);          //sphere
    
//    pt->v = (float3_nrmp(pt->x,p) - r);         //p-sphere
    
    return;
}
