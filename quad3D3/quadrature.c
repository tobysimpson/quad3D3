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
    return prb->msh.ele_vlm;                                        //whole element volume
}

//quadrature around one vertex
float quad_vtx1(struct problem *prb, int vtx_idx)
{
    float vtx_loc[3];
    
    vtx_loc[0] = (vtx_idx>>0)&1;                                //vtx local coords
    vtx_loc[1] = (vtx_idx>>1)&1;
    vtx_loc[2] = (vtx_idx>>2)&1;
    
    /*
     ==========================
     x-root
     ==========================
     */
    
    float x1 = bas_rx(prb, vtx_loc[1], vtx_loc[2]);         //find root
    
    float rx_loc[3];                                        //root in the x-direction
    float rx_glb[3];
    
    rx_loc[0] = x1;                                         //local root
    rx_loc[1] = vtx_loc[1];
    rx_loc[2] = vtx_loc[2];
    
    float3_emul(rx_loc, prb->msh.ele_h, rx_glb);            //local to global
    float3_eadd(prb->ele.vtx_glb[0], rx_glb, rx_glb);
    
    lst_add(&prb->lst3, rx_glb, 0);
    
    /*
     ==========================
     y-root
     ==========================
     */
    
    float y1 = bas_ry(prb, vtx_loc[0], vtx_loc[2]);         //find root
    
    float ry_loc[3];                                        //root in the x-direction
    float ry_glb[3];
    
    ry_loc[0] = vtx_loc[0];                                 //local root
    ry_loc[1] = y1;
    ry_loc[2] = vtx_loc[2];
    
    float3_emul(ry_loc, prb->msh.ele_h, ry_glb);            //local to global
    float3_eadd(prb->ele.vtx_glb[0], ry_glb, ry_glb);
    
    lst_add(&prb->lst3, ry_glb, 0);
    
    /*
     ==========================
     z-root
     ==========================
     */
    
    float z1 = bas_rz(prb, vtx_loc[0], vtx_loc[1]);         //find root
    
    float rz_loc[3];                                        //root in the x-direction
    float rz_glb[3];
    
    rz_loc[0] = vtx_loc[0];                                 //local root
    rz_loc[1] = vtx_loc[1];
    rz_loc[2] = z1;
    
    float3_emul(rz_loc, prb->msh.ele_h, rz_glb);            //local to global
    float3_eadd(prb->ele.vtx_glb[0], rz_glb, rz_glb);
    
    lst_add(&prb->lst3, rz_glb, 0);
    
    if( z1<0 || z1>1 )
    {
        lst_add_ele(&prb->lst3, prb);
        lst_add(&prb->lst4, rz_glb, 0);

        printf("!!!!!!!!!!!!!HERE!!!!!!\n");
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
