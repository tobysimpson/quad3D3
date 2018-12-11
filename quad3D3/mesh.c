//
//  mesh.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"


//initialize mesh
void msh_init(struct problem *prb)
{
    /*
     ===============================
     init mesh dims
     ===============================
     */

    prb->msh.xmin[0] = MSH_XMIN_0;                                                                  //mesh dimensions
    prb->msh.xmin[1] = MSH_XMIN_1;
    prb->msh.xmin[2] = MSH_XMIN_2;
    
    prb->msh.xmax[0] = MSH_XMAX_0;
    prb->msh.xmax[1] = MSH_XMAX_1;
    prb->msh.xmax[2] = MSH_XMAX_2;
    
    float3_esub(prb->msh.xmax, prb->msh.xmin, prb->msh.msh_h);                                      //mesh size
    
    prb->msh.ele_h[0] = prb->msh.msh_h[0]/MSH_ELE_DIM_0;                                            //element size
    prb->msh.ele_h[1] = prb->msh.msh_h[1]/MSH_ELE_DIM_1;
    prb->msh.ele_h[2] = prb->msh.msh_h[2]/MSH_ELE_DIM_2;
    
    prb->msh.ele_vlm = float3_eprd(prb->msh.ele_h);                                                 //element volume
    
    printf("MSH_ELE_TOT     %d\n",MSH_ELE_TOT);
    
    printf("msh_h           %f %f %f\n",prb->msh.msh_h[0],prb->msh.msh_h[1],prb->msh.msh_h[2]);
    printf("ele_h           %f %f %f\n",prb->msh.ele_h[0],prb->msh.ele_h[1],prb->msh.ele_h[2]);
    printf("ele_vlm         %f\n",prb->msh.ele_vlm);
    
    return;
}
