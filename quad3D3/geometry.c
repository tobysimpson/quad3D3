//
//  geometry.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

////signed distance function - sphere
//float geo_sdf(float x[3])
//{
//    float r = 1;                            //radius
////    float p = 2;
//
//    return (float3_nrm(x) - r);             //sphere
//
////    return float3_nrmp(x,p) - r;          //p-sphere
//}


////signed distance function - torus
float geo_sdf(float x[3])
{
    double R = 0.7;
    double r = 0.3;

    return powf(powf(x[0],2) + powf(x[1],2) + powf(x[2],2) + powf(R,2) - powf(r,2),2) - 4*powf(R,2)*(powf(x[0],2) + powf(x[1],2));
}

