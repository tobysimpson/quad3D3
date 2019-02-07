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
    float vtx_loc[3] = {(vtx_idx>>0)&1,(vtx_idx>>1)&1,(vtx_idx>>2)&1};      //vtx local coords
    
    /*
     ==========================
     x-root
     ==========================
     */
    
    float x1 = bas_rx(prb, vtx_loc[1], vtx_loc[2]);         //find root
    
    float rx_loc[3] = {x1,vtx_loc[1],vtx_loc[2]};           //root in the x-direction

    
//    rx_loc[0] = x1;                                         //local root
//    rx_loc[1] = vtx_loc[1];
//    rx_loc[2] = vtx_loc[2];
    
    float rx_glb[3];
    
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
    
//    if( z1<0 || z1>1 )
//    {
//        lst_add_ele(&prb->lst3, prb);
//        lst_add(&prb->lst4, rz_glb, 0);
//
//        printf("!!!!!!!!!!!!!HERE!!!!!!\n");
//    }
    
    
    /*
     ==========================
     quadrature points
     ==========================
     */
    
    for(int qpt_i=0; qpt_i<prb->scm.np; qpt_i++)                                    //loop points/dims
    {
        for(int qpt_j=0; qpt_j<prb->scm.np; qpt_j++)                                    //loop points/dims
        {
            for(int qpt_k=0; qpt_k<prb->scm.np; qpt_k++)                                    //loop points/dims
            {
                
            }
        }
    }
    return 0*prb->msh.ele_vlm;                              //adjusted volume
}








////quadrature with root-finding
//void quad_slv(struct mesh *msh)
//{
//    double qpt_h[3] = {0,0,0};                                      //modified heights for coords and weights
//
//    struct point spt1;                                              //solve points
//    struct point spt2;
//
//    int d[3];
//    d[0] = 0;
//    d[1] = 1;
//    d[2] = 2;
//
//
//    qpt_h[d[0]] = msh->ele.sub_h[d[0]];                             //set first dimension
//
//
//
//    spt1 = msh->ele.sub_ref;                                    //init
//
//    spt2 = spt1;                                                //init
//
//    spt2.x[d[0]] += msh->ele.sub_h[d[0]];                       //extend
//
//    //        prb_lvl(&spt1);                                             //should be inside
//    prb_lvl(&spt2);
//
//    if(sign_test(spt1.eval, spt2.eval))                         //edge crossed
//    {
//        spt2 = ipt_root(spt1, spt2);                            //solve
//    }
//
//    qpt_h[d[0]] = spt2.x[d[0]] - spt1.x[d[0]];                  //store h
//
//
//
//
//    for(int i=0; i<QUAD_NP; i++)                                    //loop points/dims
//    {
//        spt1 = msh->ele.sub_ref;                                    //init
//
//        spt1.x[d[0]] += msh->scm.pp[i]*qpt_h[d[0]];                 //increment along dim
//
//        spt2 = spt1;                                                //init
//
//        spt2.x[d[1]] += msh->ele.sub_h[d[1]];                       //extend
//
//        //        prb_lvl(&spt1);                                             //should be inside
//        prb_lvl(&spt2);
//
//        if(sign_test(spt1.eval, spt2.eval))                         //edge crossed
//        {
//            spt2 = ipt_root(spt1, spt2);                            //solve
//        }
//
//        qpt_h[d[1]] = spt2.x[d[1]] - spt1.x[d[1]];                  //store h
//
//        //        lst_add_pt(&msh->lst_sub_spt1, spt1);
//        //        lst_add_pt(&msh->lst_sub_spt2, spt2);
//
//
//        for(int j=0; j<QUAD_NP; j++)
//        {
//            spt1 = msh->ele.sub_ref;
//
//            spt1.x[d[0]] += msh->scm.pp[i]*qpt_h[d[0]];
//            spt1.x[d[1]] += msh->scm.pp[j]*qpt_h[d[1]];
//
//            spt2 = spt1;
//
//            spt2.x[d[2]] += msh->ele.sub_h[d[2]];                           //extend
//
//            //            prb_lvl(&spt1);
//            prb_lvl(&spt2);
//
//
//            if(sign_test(spt1.eval, spt2.eval))                                             //edge crossed
//            {
//                spt2 = ipt_root(spt1, spt2);                                                //solve
//
//                //                printf("quad_slv yes   %+f %+f %+f | %+f | %+d\n",spt2.x[0],spt2.x[1],spt2.x[2],spt2.eval,msh->ele.sub_int_ext);
//
//                //                lst_add_pt(&msh->lst_sub_spt1, spt2);                                     //surface points
//            }
//            else                                                                            //the quadrature point is in an internal surface of the subcube, not the surface of the object
//            {
//                //                printf("quad_slv no    %+f %+f %+f | %+f | %+d\n",spt2.x[0],spt2.x[1],spt2.x[2],spt2.eval,msh->ele.sub_int_ext);
//
//                //                lst_add_pt(&msh->lst_sub_spt1, spt1);
//                //                lst_add_pt(&msh->lst_sub_spt2, spt2);
//
//            }
//
//            //            lst_add_pt(&msh->lst_sub_spt1, spt1);
//            lst_add_pt(&msh->lst_sub_spt2, spt2);
//
//
//            qpt_h[d[2]] = spt2.x[d[2]] - spt1.x[d[2]];                  //store h
//
//
//            //do surface quadrature
//
//            //prod weights ijk
//
//            //divide norm of gradient by the k-dimension element of the grad
//
//
//            for(int k=0; k<QUAD_NP; k++)
//            {
//                msh->ele.qpt.x[d[0]] = msh->ele.sub_ref.x[d[0]] + msh->scm.pp[i]*qpt_h[d[0]];
//                msh->ele.qpt.x[d[1]] = msh->ele.sub_ref.x[d[1]] + msh->scm.pp[j]*qpt_h[d[1]];
//                msh->ele.qpt.x[d[2]] = msh->ele.sub_ref.x[d[2]] + msh->scm.pp[k]*qpt_h[d[2]];
//
//
//                msh->ele.qpt.eval = msh->scm.ww[i]*msh->scm.ww[j]*msh->scm.ww[k]*fabs(double3_eprd(qpt_h));        //calc weight and scale
//
//                lst_add_pt(&msh->lst_sub_qpt2, msh->ele.qpt);
//
//                msh->ele.sub_vlm += msh->ele.qpt.eval;                                                          //sum onto subcube total
//
//                //                printf("quad_slv qpt2    %+12.8f %+12.8f\n",msh->vlm ,msh->ele.qpt.eval);
//            }
//        }
//    }
//
//    msh->ele.ele_vlm += msh->ele.sub_vlm * msh->ele.sub_int_ext;                                          //sum the subcube total onto the element total and adjust sign for internal/external quadrature
//
//    //    printf("quad_slv qpt2    %+18.16f %+18.16f %+18.16f\n",msh->vlm, msh->ele.ele_vlm ,msh->ele.sub_vlm);
//
//    return;
//}
//
//


















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
