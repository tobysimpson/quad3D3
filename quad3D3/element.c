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
    
    ele_vtx_gen(prb);                                                                               //gen verts
    
    printf("vtx_int %d\n",prb->ele.vtx_int);
    
    /*
     ===============================
     quadrature logic
     ===============================
     */
    
    switch (prb->ele.vtx_int)
    {
        case 0:                                                     //all external
            
            //do nothing
            
            break;
            
        case 8:                                                     //all internal
        
            prb->vlm += quad_ele(prb);                              //do quadrature on element
            
            break;
            
        default:                                                    //both
            
            quad_fac(prb);                                          //send to logic
            
            break;
    }
    
    return;
}


//generate vertices
void ele_vtx_gen(struct problem *prb)
{
    float3_emul_int(prb->msh.ele_h, prb->ele.pos, prb->ele.vtx[0][0][0].x);                         //set reference vertex
    float3_eadd(prb->ele.vtx[0][0][0].x, prb->msh.xmin, prb->ele.vtx[0][0][0].x);
 
    prb->ele.vtx_int = 0;                                                                           //reset count
    
    for(int k=0; k<2; k++)                                                                          //loop verts
    {
        for(int j=0; j<2; j++)
        {
            for(int i=0; i<2; i++)
            {
                prb->ele.vtx[i][j][k].x[0] = prb->ele.vtx[0][0][0].x[0] + prb->msh.ele_h[0]*i;      //vtx coords
                prb->ele.vtx[i][j][k].x[1] = prb->ele.vtx[0][0][0].x[1] + prb->msh.ele_h[1]*j;
                prb->ele.vtx[i][j][k].x[2] = prb->ele.vtx[0][0][0].x[2] + prb->msh.ele_h[2]*k;
                
                geo_sdf(&prb->ele.vtx[i][j][k]);                                                    //calc sdf
                
                prb->ele.vtx_int += (prb->ele.vtx[i][j][k].v < 0);                                  //count internal verts
                
//                prb->ele.vtx[i][j][k].v = (prb->ele.vtx[i][j][k].v < 0);                            //store boolean value
                
                lst_add(&prb->lst1, prb->ele.vtx[i][j][k]);                                         //add to list
                
//                printf("ele_vtx_gen %d %d %d | %+e %+e %+e | %+e\n",i,j,k,
//                       prb->ele.vtx[i][j][k].x[0],
//                       prb->ele.vtx[i][j][k].x[1],
//                       prb->ele.vtx[i][j][k].x[2],
//                       prb->ele.vtx[i][j][k].v);
            }
        }
    }

    return;
}


