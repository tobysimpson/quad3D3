//
//  quadrature.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//root finding
float bas_rx(struct problem *prb, float y, float z);
float bas_ry(struct problem *prb, float x, float z);
float bas_rz(struct problem *prb, float x, float y);

//quadrature on whole element
float quad_ele(struct problem *prb)
{
    return prb->msh.ele_vlm;                                //whole element volume
}

//quadrature around one vertex
float quad_vtx1(struct problem *prb)
{
    printf("opp face %d %d %d %d\n",
           prb->ele.fac_vtx[0],
           prb->ele.fac_vtx[1],
           prb->ele.fac_vtx[2],
           prb->ele.fac_vtx[3]);
    
    printf("opp ivtx %d %d %d %d\n",
           prb->ele.vtx_sdf[prb->ele.fac_vtx[0]]<0,
           prb->ele.vtx_sdf[prb->ele.fac_vtx[1]]<0,
           prb->ele.vtx_sdf[prb->ele.fac_vtx[2]]<0,
           prb->ele.vtx_sdf[prb->ele.fac_vtx[3]]<0);
    

    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)                    //loop verts
    {
        float vtx_loc[3];
        
        vtx_loc[0] = (vtx_idx>>0)&1;                            //vtx local coords
        vtx_loc[1] = (vtx_idx>>1)&1;
        vtx_loc[2] = (vtx_idx>>2)&1;
        
        if(prb->ele.vtx_sdf[vtx_idx] < 0)                       //find internal vtx
        {
            float x1 = bas_rx(prb, vtx_loc[1], vtx_loc[2]);     //find root
            
            lst_add(&prb->lst3, prb->ele.vtx_glb[vtx_idx], 0);
            
            float r1[3];
            
            r1[0] = x1;
            r1[1] = vtx_loc[1];
            r1[2] = vtx_loc[2];
            
            lst_add(&prb->lst4, r1, 0);
            
//            break;
        }
    }
    

    

    return 0*prb->msh.ele_vlm;                              //adjusted volume
}

//basis root x co-ord
float bas_rx(struct problem *prb, float y, float z)
{
    return -(prb->ele.bas_aa[0] + prb->ele.bas_aa[2]*y + prb->ele.bas_aa[3]*z + prb->ele.bas_aa[6]*y*z)/
            (prb->ele.bas_aa[1] + prb->ele.bas_aa[4]*y + prb->ele.bas_aa[5]*z + prb->ele.bas_aa[7]*y*z);
}

//basis root y co-ord
float bas_ry(struct problem *prb, float x, float z)
{
    return -(prb->ele.bas_aa[0] + prb->ele.bas_aa[1]*x + prb->ele.bas_aa[3]*z + prb->ele.bas_aa[5]*x*z)/
            (prb->ele.bas_aa[2] + prb->ele.bas_aa[4]*x + prb->ele.bas_aa[6]*z + prb->ele.bas_aa[7]*x*z);
}

//basis root z co-ord
float bas_rz(struct problem *prb, float x, float y)
{
    return -(prb->ele.bas_aa[0] + prb->ele.bas_aa[1]*x + prb->ele.bas_aa[2]*y + prb->ele.bas_aa[4]*x*y)/
            (prb->ele.bas_aa[3] + prb->ele.bas_aa[5]*x + prb->ele.bas_aa[6]*y + prb->ele.bas_aa[7]*x*y);
}
