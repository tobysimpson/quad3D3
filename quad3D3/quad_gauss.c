//
//  quadrature.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//quadrature on whole element
float quad_ele(struct problem *prb)
{
    return 1;                                                                                   //whole element volume
}

//quadrature around one vertex
float quad_vtx1(struct problem *prb)
{
    float vlm_loc = 0;
    
    float vtx_loc[3] = {(prb->ele.vtx_idx[0]>>0)&1,
                        (prb->ele.vtx_idx[0]>>1)&1,
                        (prb->ele.vtx_idx[0]>>2)&1};  //vtx local coords
    
//    /*
//     ==========================
//     x-root
//     ==========================
//     */
//
//    float x1 = bas_root(prb, 0, vtx_loc);                                     //find root
//
//    float rx_loc[3] = {x1,vtx_loc[1],vtx_loc[2]};                                       //root in the x-direction
//
//    float rx_glb[3];
//
//    float3_emul(rx_loc, prb->msh.ele_h, rx_glb);                                        //local to global
//    float3_eadd(prb->ele.vtx_glb[0], rx_glb, rx_glb);
//
//    lst_add_pt(&prb->lst2, rx_glb, 0);
//
//    /*
//     ==========================
//     y-root
//     ==========================
//     */
//
//    float y1 = bas_root(prb, 1, vtx_loc);                                     //find root
//
//    float ry_loc[3] = {vtx_loc[0],y1,vtx_loc[2]};                                       //root in the x-direction
//
//    float ry_glb[3];
//
//    float3_emul(ry_loc, prb->msh.ele_h, ry_glb);                                        //local to global
//    float3_eadd(prb->ele.vtx_glb[0], ry_glb, ry_glb);
//
//    lst_add_pt(&prb->lst3, ry_glb, 0);
//
//    /*
//     ==========================
//     z-root
//     ==========================
//     */
//
//    float z1 = bas_root(prb, 2, vtx_loc);                                     //find root
//
//    float rz_loc[3] = {vtx_loc[0],vtx_loc[1],z1};                                       //root in the x-direction
//
//    float rz_glb[3];
//
//    float3_emul(rz_loc, prb->msh.ele_h, rz_glb);                                        //local to global
//    float3_eadd(prb->ele.vtx_glb[0], rz_glb, rz_glb);
//
//    lst_add_pt(&prb->lst4, rz_glb, 0);
    
    /*
     ==========================
     generate quadrature points
     ==========================
     */
    
    float qpt_loc[3];                                                                   //vlm quad point
    float qpt_glb[3];

    float spt_loc[3];                                                                   //srf quad point
    float spt_glb[3];

    float qpt_h[3];                                                                     //heights for scaling
    float qpt_r[3];                                                                     //roots

    qpt_r[0] = bas_root(prb, 0, vtx_loc);                                               //find root x

    qpt_h[0] = qpt_r[0] - vtx_loc[0];                                                   //store h x

    for(int qpt_i=0; qpt_i<prb->scm.np; qpt_i++)                                        //loop x
    {
        qpt_loc[0] = vtx_loc[0] + qpt_h[0]*prb->scm.pp[qpt_i];                          //update x
        qpt_loc[2] = vtx_loc[2];

        qpt_r[1] = bas_root(prb, 1, qpt_loc);                                           //find root y

        qpt_h[1] = qpt_r[1] - vtx_loc[1];                                               //store h y

        for(int qpt_j=0; qpt_j<prb->scm.np; qpt_j++)                                    //loop y
        {
            qpt_loc[1] = vtx_loc[1] + qpt_h[1]*prb->scm.pp[qpt_j];                      //update y

            qpt_r[2] = bas_root(prb, 2, qpt_loc);                                       //find root z

            qpt_h[2] = qpt_r[2] - vtx_loc[2];                                           //store h z

            spt_loc[0] = qpt_loc[0];                                                    //surface quad point
            spt_loc[1] = qpt_loc[1];
            spt_loc[2] = qpt_r[2];

            //do srf quad here

            float3_emul(spt_loc, prb->msh.ele_h, spt_glb);                              //local to global
            float3_eadd(prb->ele.vtx_glb[0], spt_glb, spt_glb);
            lst_add_pt(&prb->lst3, spt_glb, 0);

            for(int qpt_k=0; qpt_k<prb->scm.np; qpt_k++)                                //loop z
            {
                qpt_loc[2] = vtx_loc[2] + qpt_h[2]*prb->scm.pp[qpt_k];                  //update z

                vlm_loc += prb->scm.ww[qpt_i]*prb->scm.ww[qpt_j]*prb->scm.ww[qpt_k]*fabsf(float3_eprd(qpt_h));  //sum onto volume

                //printf("%d %d %d | %+f %+f %+f\n",qpt_i,qpt_j,qpt_k,qpt_loc[0],qpt_loc[1],qpt_loc[2]);

                float3_emul(qpt_loc, prb->msh.ele_h, qpt_glb);                          //local to global
                float3_eadd(prb->ele.vtx_glb[0], qpt_glb, qpt_glb);
                lst_add_pt(&prb->lst4, qpt_glb, 0);
            }
        }
    }
    return vlm_loc;                                                                     //local volume
}


//quadrature with 2 adjacent vertices
float quad_vtx2(struct problem *prb)
{
//    printf("quad_vtx2   %d %d \n",prb->ele.vtx_idx[0],prb->ele.vtx_idx[1]);
    
    float vlm_loc = 0;
    
    float vtx0_loc[3] = {(prb->ele.vtx_idx[0]>>0)&1,(prb->ele.vtx_idx[0]>>1)&1,(prb->ele.vtx_idx[0]>>2)&1};        //vtx local coords
    float vtx1_loc[3] = {(prb->ele.vtx_idx[1]>>0)&1,(prb->ele.vtx_idx[1]>>1)&1,(prb->ele.vtx_idx[1]>>2)&1};
    
//    float vtx0_glb[3];
//    float vtx1_glb[3];
//
//    float3_emul(vtx0_loc, prb->msh.ele_h, vtx0_glb);                                  //local to global
//    float3_eadd(prb->ele.vtx_glb[0], vtx0_glb, vtx0_glb);
//    lst_add_pt(&prb->lst2, vtx0_glb, 0);
//
//    float3_emul(vtx1_loc, prb->msh.ele_h, vtx1_glb);                                  //local to global
//    float3_eadd(prb->ele.vtx_glb[0], vtx1_glb, vtx1_glb);
//    lst_add_pt(&prb->lst2, vtx1_glb, 0);
    
    /*
     ==========================
     permute dimensions
     ==========================
     */

    int dim_idx[3];                                                                     //permute the dimension

    switch (prb->ele.vtx_idx[0]^prb->ele.vtx_idx[1])                                    //test the xor of the vertex coords to get the direction of the edge
    {
        case 1:                             //xyz

            dim_idx[0] = 0;
            dim_idx[1] = 1;
            dim_idx[2] = 2;

            break;

        case 2:                             //yxz

            dim_idx[0] = 1;
            dim_idx[1] = 0;
            dim_idx[2] = 2;

            break;

        case 4:                             //zxy

            dim_idx[0] = 2;
            dim_idx[1] = 0;
            dim_idx[2] = 1;

            break;

        default:

            printf("error in quad_vtx2\n");
    }

    /*
     ==========================
     generate quadrature points
     ==========================
     */

    float qpt_loc[3];                                                                               //vlm quad point
    float qpt_glb[3];

    float spt_loc[3];                                                                               //srf quad point
    float spt_glb[3];

    float qpt_h[3];                                                                                  //heights for scaling
    float qpt_r[3];                                                                                 //roots

    qpt_r[dim_idx[0]] = vtx1_loc[dim_idx[0]];                                                       //find root

    qpt_h[dim_idx[0]] = qpt_r[dim_idx[0]] - vtx0_loc[dim_idx[0]];                                   //store h x

    for(int qpt_i=0; qpt_i<prb->scm.np; qpt_i++)                                                    //loop x
    {
        qpt_loc[dim_idx[0]] = vtx0_loc[dim_idx[0]] + qpt_h[dim_idx[0]]*prb->scm.pp[qpt_i];          //update x
        qpt_loc[dim_idx[2]] = vtx0_loc[dim_idx[2]];

        qpt_r[dim_idx[1]] = bas_root(prb, dim_idx[1], qpt_loc);                                     //find root

        qpt_h[dim_idx[1]] = qpt_r[dim_idx[1]] - vtx0_loc[dim_idx[1]];                               //store h y

        for(int qpt_j=0; qpt_j<prb->scm.np; qpt_j++)                                                //loop y
        {
            qpt_loc[dim_idx[1]] = vtx0_loc[dim_idx[1]] + qpt_h[dim_idx[1]]*prb->scm.pp[qpt_j];      //update y

            qpt_r[dim_idx[2]] = bas_root(prb, dim_idx[2], qpt_loc);                                 //find root

            qpt_h[dim_idx[2]] = qpt_r[dim_idx[2]] - vtx0_loc[dim_idx[2]];                           //store h z

            spt_loc[dim_idx[0]] = qpt_loc[dim_idx[0]];                                              //surface quad point
            spt_loc[dim_idx[1]] = qpt_loc[dim_idx[1]];
            spt_loc[dim_idx[2]] = qpt_r[dim_idx[2]];

            //do srf quad here

            float3_emul(spt_loc, prb->msh.ele_h, spt_glb);                                          //local to global
            float3_eadd(prb->ele.vtx_glb[0], spt_glb, spt_glb);
            lst_add_pt(&prb->lst3, spt_glb, 0);

            for(int qpt_k=0; qpt_k<prb->scm.np; qpt_k++)                                            //loop z
            {
                qpt_loc[dim_idx[2]] = vtx0_loc[dim_idx[2]] + qpt_h[dim_idx[2]]*prb->scm.pp[qpt_k];  //update z

                vlm_loc += prb->scm.ww[qpt_i]*prb->scm.ww[qpt_j]*prb->scm.ww[qpt_k]*fabsf(float3_eprd(qpt_h));  //sum onto volume

//                printf("%d %d %d | %+f %+f %+f\n",qpt_i,qpt_j,qpt_k,qpt_loc[0],qpt_loc[1],qpt_loc[2]);

                float3_emul(qpt_loc, prb->msh.ele_h, qpt_glb);                                      //local to global
                float3_eadd(prb->ele.vtx_glb[0], qpt_glb, qpt_glb);
                lst_add_pt(&prb->lst4, qpt_glb, 0);
            }
        }
    }
    return vlm_loc;                                                                                 //local volume
}



//quadrature with 4 internal verts opposite an external face
float quad_vtx4(struct problem *prb)
{
    float vlm_loc = 0;
    
    float vtx_loc[3];
    
    /*
     ==========================
     set up dims/ref coord on internal face
     ==========================
     */
    
    int dim_idx[3];                                 //permute dimensions
    
    switch (prb->ele.bf_dim)                        //choose a starting vertex and dimension order
    {
        case 0:
            
            vtx_loc[0] = prb->ele.bf_crd;
            vtx_loc[1] = 0;
            vtx_loc[2] = 0;
            
            dim_idx[0] = 1;
            dim_idx[1] = 2;
            dim_idx[2] = 0;
            
            break;
            
        case 1:
            
            vtx_loc[0] = 0;
            vtx_loc[1] = prb->ele.bf_crd;
            vtx_loc[2] = 0;
            
            dim_idx[0] = 0;
            dim_idx[1] = 2;
            dim_idx[2] = 1;
            
            break;
            
        case 2:
            
            vtx_loc[0] = 0;
            vtx_loc[1] = 0;
            vtx_loc[2] = prb->ele.bf_crd;
            
            dim_idx[0] = 0;
            dim_idx[1] = 1;
            dim_idx[2] = 2;
            
            break;
            
        default:
            
            printf("error in quad_vtx2\n");
    }
    
    /*
     ==========================
     generate quadrature points
     ==========================
     */
    
    float qpt_loc[3];                                                                               //vlm quad point
    float qpt_glb[3];
    
    float spt_loc[3];                                                                               //srf quad point
    float spt_glb[3];
    
    float qpt_h[3];                                                                                 //heights for scaling
    float qpt_r[3];                                                                                 //roots
    
    qpt_r[dim_idx[0]] = 1;                                                                          //find root
    
    qpt_h[dim_idx[0]] = 1;                                                                          //store h x
    
    for(int qpt_i=0; qpt_i<prb->scm.np; qpt_i++)                                                    //loop x
    {
        qpt_loc[dim_idx[0]] = vtx_loc[dim_idx[0]] + qpt_h[dim_idx[0]]*prb->scm.pp[qpt_i];           //update x
        qpt_loc[dim_idx[2]] = vtx_loc[dim_idx[2]];
        
        qpt_r[dim_idx[1]] = 1;                                                                      //find root
        
        qpt_h[dim_idx[1]] = 1;                                                                      //store h y
        
        for(int qpt_j=0; qpt_j<prb->scm.np; qpt_j++)                                                //loop y
        {
            qpt_loc[dim_idx[1]] = vtx_loc[dim_idx[1]] + qpt_h[dim_idx[1]]*prb->scm.pp[qpt_j];       //update y
            
            qpt_r[dim_idx[2]] = bas_root(prb, dim_idx[2], qpt_loc);                                 //find root
            
            qpt_h[dim_idx[2]] = qpt_r[dim_idx[2]] - vtx_loc[dim_idx[2]];                            //store h z
            
            spt_loc[dim_idx[0]] = qpt_loc[dim_idx[0]];                                              //surface quad point
            spt_loc[dim_idx[1]] = qpt_loc[dim_idx[1]];
            spt_loc[dim_idx[2]] = qpt_r[dim_idx[2]];
            
            //do srf quad here
            
            float3_emul(spt_loc, prb->msh.ele_h, spt_glb);                                          //local to global
            float3_eadd(prb->ele.vtx_glb[0], spt_glb, spt_glb);
            lst_add_pt(&prb->lst3, spt_glb, 0);
            
            for(int qpt_k=0; qpt_k<prb->scm.np; qpt_k++)                                            //loop z
            {
                qpt_loc[dim_idx[2]] = vtx_loc[dim_idx[2]] + qpt_h[dim_idx[2]]*prb->scm.pp[qpt_k];   //update z
                
                vlm_loc += prb->scm.ww[qpt_i]*prb->scm.ww[qpt_j]*prb->scm.ww[qpt_k]*fabsf(float3_eprd(qpt_h));  //sum onto volume
                
                //printf("%d %d %d | %+f %+f %+f\n",qpt_i,qpt_j,qpt_k,qpt_loc[0],qpt_loc[1],qpt_loc[2]);
                
                float3_emul(qpt_loc, prb->msh.ele_h, qpt_glb);                                      //local to global
                float3_eadd(prb->ele.vtx_glb[0], qpt_glb, qpt_glb);
                lst_add_pt(&prb->lst4, qpt_glb, 0);
            }
        }
    }
    return vlm_loc;                                                                         //adjusted volume
}




//root of interpolated sdf - only uses 2 of the three coords
float bas_root(struct problem *prb, int dim_idx, float vtx_loc[3])
{
    float x = vtx_loc[0];
    float y = vtx_loc[1];
    float z = vtx_loc[2];
    
    switch (dim_idx)
    {
        case 0:     //x

            return -(prb->ele.bas_aa[0] + prb->ele.bas_aa[2]*y + prb->ele.bas_aa[3]*z + prb->ele.bas_aa[6]*y*z)/
                    (prb->ele.bas_aa[1] + prb->ele.bas_aa[4]*y + prb->ele.bas_aa[5]*z + prb->ele.bas_aa[7]*y*z);

            break;

        case 1:     //y

            return -(prb->ele.bas_aa[0] + prb->ele.bas_aa[1]*x + prb->ele.bas_aa[3]*z + prb->ele.bas_aa[5]*x*z)/
                    (prb->ele.bas_aa[2] + prb->ele.bas_aa[4]*x + prb->ele.bas_aa[6]*z + prb->ele.bas_aa[7]*x*z);

            break;

        case 2:     //z

            return -(prb->ele.bas_aa[0] + prb->ele.bas_aa[1]*x + prb->ele.bas_aa[2]*y + prb->ele.bas_aa[4]*x*y)/
                    (prb->ele.bas_aa[3] + prb->ele.bas_aa[5]*x + prb->ele.bas_aa[6]*y + prb->ele.bas_aa[7]*x*y);

            break;

        default:

            return NAN;

            break;
    }
}


