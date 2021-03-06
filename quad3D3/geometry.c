//
//  geometry.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"


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
//    double R = 0.75;
//    double r = 0.25;
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
        prb->geo.rr[i] = 0.25*(float)rand()/(float)RAND_MAX;

        float3_emul(prb->msh.msh_h, prb->geo.cc[i], prb->geo.cc[i]);    //put into coord system
        float3_eadd(prb->msh.xmin, prb->geo.cc[i], prb->geo.cc[i]);

        prb->geo.rr[i] = prb->geo.rr[i]/prb->msh.msh_h[0];

        //        printf("geo     %+f %+f %+f %+f\n",prb->geo.cc[i][0],prb->geo.cc[i][1],prb->geo.cc[i][2],prb->geo.rr[i]);
    }

    /*
     ======================================
     pre-calculate
     ======================================
     */
    
    for(int k=0; k<(MSH_ELE_DIM_2+1); k++)                              //loop space
    {
        for(int j=0; j<(MSH_ELE_DIM_1+1); j++)
        {
            for(int i=0; i<(MSH_ELE_DIM_0+1); i++)
            {
                float x[3];
                
                x[0] = prb->msh.xmin[0] + i*prb->msh.ele_h[0];          //gen coords
                x[1] = prb->msh.xmin[1] + j*prb->msh.ele_h[1];
                x[2] = prb->msh.xmin[2] + k*prb->msh.ele_h[2];
                
                prb->geo.dof_sdf[i][j][k] = geo_sdf(prb,x);             //eval sdf
            
            }
        }
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

        float sdf_eval = float3_nrm(y) - prb->geo.rr[i];    //eval

//        if(sdf_eval<sdf_min)
//        {
//            sdf_min = sdf_eval;                             //store min
//        }

        sdf_min = geo_smin(sdf_min,sdf_eval,0.3);               //smoothed minimum
    }
    return sdf_min;
}


// polynomial smooth min (k = 0.1);
float geo_smin( float a, float b, float k )
{
    float h = MAX(k-fabs(a-b), 0.0 )/k;
    return MIN(a,b) - h*h*k*(0.2);                              // (0.2/200 centres)
}
