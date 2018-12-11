//
//  element.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//process element
void ele_calc(struct problem *prb)
{
    printf("ele_calc %8d | %2d %2d %2d \n",
           prb->ele.idx,
           prb->ele.pos[0],
           prb->ele.pos[1],
           prb->ele.pos[2]);
    
    ele_vtx_gen(prb);                                                                           //gen verts
    
    return;
}


//generate vertices
void ele_vtx_gen(struct problem *prb)
{
    float3_emul_int(prb->msh.ele_h, prb->ele.pos, prb->ele.vtx[0][0][0].x);                     //set reference vertex
    float3_eadd(prb->ele.vtx[0][0][0].x, prb->msh.xmin, prb->ele.vtx[0][0][0].x);
    
    for(int k=0; k<2; k++)                                                                      //loop verts
    {
        for(int j=0; j<2; j++)
        {
            for(int i=0; i<2; i++)
            {
                prb->ele.vtx[i][j][k].x[0] = prb->ele.vtx[0][0][0].x[0] + prb->msh.ele_h[0]*i;  //vtx coords
                prb->ele.vtx[i][j][k].x[1] = prb->ele.vtx[0][0][0].x[1] + prb->msh.ele_h[1]*j;
                prb->ele.vtx[i][j][k].x[2] = prb->ele.vtx[0][0][0].x[2] + prb->msh.ele_h[2]*k;
                
                void geo_sdf(struct point *pt);                                                  //test sdf
                
                printf("ele_vtx_gen %d %d %d | %+e %+e %+e | %+d\n",i,j,k,
                       prb->ele.vtx[i][j][k].x[0],
                       prb->ele.vtx[i][j][k].x[1],
                       prb->ele.vtx[i][j][k].x[2],
                       prb->ele.vtx[i][j][k].v < 0);
            }
        }
    }
    return;
}
