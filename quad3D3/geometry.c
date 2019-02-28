//
//  geometry.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//initialise geometry
void geo_init(struct problem *prb)
{
    srand((unsigned int)time(NULL));                                    //init rand
    
    for(int i=0; i<GEO_NC; i++)
    {
        prb->geo.cc[i][0] = (float)rand()/(float)RAND_MAX;              //init centres/radii
        prb->geo.cc[i][1] = (float)rand()/(float)RAND_MAX;
        prb->geo.cc[i][2] = (float)rand()/(float)RAND_MAX;
        prb->geo.rr[i] = (float)rand()/(float)RAND_MAX;
        
        float3_emul(prb->msh.msh_h, prb->geo.cc[i], prb->geo.cc[i]);    //put into coord system
        float3_eadd(prb->msh.xmin, prb->geo.cc[i], prb->geo.cc[i]);
        
        printf("geo     %+f %+f %+f %+f\n",prb->geo.cc[i][0],prb->geo.cc[i][1],prb->geo.cc[i][2],prb->geo.rr[i]);
    }
    
    return;
}

////signed distance function - sphere
//float geo_sdf(struct problem *prb, float x[3])
//{
//    float r = 1;                            //radius
////    float p = 2;
//
//    return (float3_nrm(x) - r);             //sphere
//
////    return float3_nrmp(x,p) - r;          //p-sphere
//}


//////signed distance function - torus
//float geo_sdf(struct problem *prb, float x[3])
//{
//    double R = 0.7;
//    double r = 0.3;
//
//    return powf(powf(x[0],2) + powf(x[1],2) + powf(x[2],2) + powf(R,2) - powf(r,2),2) - 4*powf(R,2)*(powf(x[0],2) + powf(x[1],2));
//}

//signed distance function - spheres
float geo_sdf(struct problem *prb, float x[3])
{
    float sdf_min = float3_nrm(x) - prb->geo.rr[0];
    
    float y[3];                                         //tmp coord
    
    for(int i=0; i<GEO_NC; i++)                         //loop centres
    {
        float3_esub(x, prb->geo.cc[i], y);              //shift to centre
        sdf_min = float3_nrm(y) - prb->geo.rr[i];
    }
    
    return sdf_min;
}

