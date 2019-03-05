//
//  geometry.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//prototypes
float smin( float a, float b, float k );


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

/*
 ======================================
 blob sdf
 ======================================
 */

//initialise geometry
void geo_init(struct problem *prb)
{
    srand((unsigned int)time(NULL));                                    //init rand

    for(int i=0; i<GEO_NC; i++)
    {
        prb->geo.cc[i][0] = (float)rand()/(float)RAND_MAX;              //init centres/radii
        prb->geo.cc[i][1] = (float)rand()/(float)RAND_MAX;
        prb->geo.cc[i][2] = (float)rand()/(float)RAND_MAX;
        prb->geo.rr[i] = 0.4*(float)rand()/(float)RAND_MAX;

        float3_emul(prb->msh.msh_h, prb->geo.cc[i], prb->geo.cc[i]);    //put into coord system
        float3_eadd(prb->msh.xmin, prb->geo.cc[i], prb->geo.cc[i]);

        prb->geo.rr[i] = prb->geo.rr[i]/prb->msh.msh_h[0];

        //        printf("geo     %+f %+f %+f %+f\n",prb->geo.cc[i][0],prb->geo.cc[i][1],prb->geo.cc[i][2],prb->geo.rr[i]);
    }

    return;
}


//signed distance function - blob
float geo_sdf(struct problem *prb, float x[3])
{
    float sdf_min = float3_nrm(x) - prb->geo.rr[0];

    float y[3];                                             //tmp coord

    for(int i=0; i<GEO_NC; i++)                             //loop centres
    {
        float3_esub(x, prb->geo.cc[i], y);                  //shift to centre

        float sdf_eval = float3_nrm(y) - prb->geo.rr[i];   //eval

//        if(sdf_eval<sdf_min)
//        {
//            sdf_min = sdf_eval;                             //store min
//        }

        sdf_min = smin(sdf_min,sdf_eval,0.3);
    }
    return sdf_min;
}


// polynomial smooth min (k = 0.1);
float smin( float a, float b, float k )
{
    float h = MAX(k-fabs(a-b), 0.0 )/k;
    return MIN(a,b) - h*h*k*(0.25);
}
