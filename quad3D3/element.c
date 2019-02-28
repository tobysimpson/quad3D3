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
//    printf("ele_calc %10d | %3d %3d %3d \n",
//            prb->ele.idx,
//            prb->ele.pos[0],
//            prb->ele.pos[1],
//            prb->ele.pos[2]);

    /*
     ===============================
     reset
     ===============================
     */
    
    prb->ele.vtx_int_tot = 0;                                                               //reset count
    
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<2; j++)
        {
            prb->ele.fac_vtx_int[i][j] = 0;                                                 //reset fac_int
        }
    }
    
    if(prb->ele.pos[0]==(MSH_ELE_DIM_0-1))                                                  //do running total
    {
        prb->vlm[1] += prb->vlm[0];                                                         //sum onto row total
        prb->vlm[0] = 0;                                                                    //reset subtotal
        
        if(prb->ele.pos[1]==(MSH_ELE_DIM_1-1))                                              //do running total
        {
            prb->vlm[2] += prb->vlm[1];                                                     //sum onto main total
            prb->vlm[1] = 0;                                                                //reset subtotal
        }
    }
    
    prb->ele.vlm_loc = 0;                                                                   //volume subtotal
    
    /*
     ===============================
     generate vertices & test sdf
     ===============================
     */
    
    float3_emul_int(prb->msh.ele_h, prb->ele.pos, prb->ele.vtx_glb[0]);                     //set reference vertex
    float3_eadd(prb->ele.vtx_glb[0], prb->msh.xmin, prb->ele.vtx_glb[0]);
    
    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)
    {
        int i = (vtx_idx>>0)&1;                                                            //binary digits xyz coords
        int j = (vtx_idx>>1)&1;
        int k = (vtx_idx>>2)&1;
        
        prb->ele.vtx_glb[vtx_idx][0] = prb->ele.vtx_glb[0][0] + prb->msh.ele_h[0]*i;        //vtx coords
        prb->ele.vtx_glb[vtx_idx][1] = prb->ele.vtx_glb[0][1] + prb->msh.ele_h[1]*j;
        prb->ele.vtx_glb[vtx_idx][2] = prb->ele.vtx_glb[0][2] + prb->msh.ele_h[2]*k;
        
        prb->ele.vtx_sdf[vtx_idx] = geo_sdf(prb, prb->ele.vtx_glb[vtx_idx]);                     //calc sdf
        
        prb->ele.vtx_int_tot += (prb->ele.vtx_sdf[vtx_idx] < 0);                            //count internal verts
        
//        lst_add(&prb->lst1, prb->ele.vtx_glb[vtx_idx], prb->ele.vtx_sdf[vtx_idx]);          //add to list
        
//        printf("vtx_gen %1.0f %1.0f %1.0f | %+e %+e %+e | %d\n",vtx_loc[0],vtx_loc[1],vtx_loc[2],
//               prb->ele.vtx_glb[vtx_idx][0],
//               prb->ele.vtx_glb[vtx_idx][1],
//               prb->ele.vtx_glb[vtx_idx][2],
//               prb->ele.vtx_sdf[vtx_idx]<0);
        
        prb->ele.fac_vtx_int[0][i] += prb->ele.vtx_sdf[vtx_idx]<0;                          //calc internal verts per face
        prb->ele.fac_vtx_int[1][j] += prb->ele.vtx_sdf[vtx_idx]<0;
        prb->ele.fac_vtx_int[2][k] += prb->ele.vtx_sdf[vtx_idx]<0;
    }
    
    /*
     ===============================
     interpolated basis function coeffs
     ===============================
     */
    
    prb->ele.bas_aa[0] = + prb->ele.vtx_sdf[0];                                                                         //1
    
    prb->ele.bas_aa[1] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[1];                                                   //x
    prb->ele.bas_aa[2] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[2];                                                   //y
    prb->ele.bas_aa[3] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[4];                                                   //z
    
    prb->ele.bas_aa[4] = + prb->ele.vtx_sdf[0] - prb->ele.vtx_sdf[1] - prb->ele.vtx_sdf[2] + prb->ele.vtx_sdf[3];       //xy
    prb->ele.bas_aa[5] = + prb->ele.vtx_sdf[0] - prb->ele.vtx_sdf[1] - prb->ele.vtx_sdf[4] + prb->ele.vtx_sdf[5];       //xz
    prb->ele.bas_aa[6] = + prb->ele.vtx_sdf[0] - prb->ele.vtx_sdf[2] - prb->ele.vtx_sdf[4] + prb->ele.vtx_sdf[6];       //xz
    
    prb->ele.bas_aa[7] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[1] + prb->ele.vtx_sdf[2] - prb->ele.vtx_sdf[3]
                         + prb->ele.vtx_sdf[4] - prb->ele.vtx_sdf[5] - prb->ele.vtx_sdf[6] + prb->ele.vtx_sdf[7];       //xyz

    /*
     ===============================
     debug print vtx_int
     ===============================
     */
    
//    printf("vtx_int %d\n",prb->ele.vtx_int_tot);
//
//    printf("fac_int\n");
//    for(int i=0; i<3; i++)
//    {
//        for(int j=0; j<2; j++)
//        {
//            printf("%d ",prb->ele.fac_vtx_int[i][j]);        //debug fac_int
//        }
//        printf("\n");
//    }
    

    /*
     ===============================
     quadrature logic
     ===============================
     */
    
    prb->ele.ctr.vtx_int[0][prb->ele.vtx_int_tot] += 1;                     //increment counter
    
    int pth_con = ele_pth_test(prb);                                        //test path connectedness
    
    prb->ele.ctr.vtx_int[1][prb->ele.vtx_int_tot] += pth_con;               //count path conn
    
    if(!pth_con)
    {
        lst_add_ele(&prb->lst2, prb);                                        //have a look
    }

    
    switch (prb->ele.vtx_int_tot)
    {
        case 0:                                                             //all external
        {
            break;
        }
        case 8:                                                             //all internal
        {
            break;
        }
        default:                                                            //both
        {
            lst_add_ele(&prb->lst1, prb);                                        //have a look

//            /*
//             ===============================
//             find int/ext faces
//             ===============================
//             */
//
//            prb->ele.fac_ext_flg = 0;                               //reset flags
//            prb->ele.fac_int_flg = 0;
//
//            for(int dim_idx=0; dim_idx<3; dim_idx++)                //loop dims
//            {
//                for(int crd_idx=0; crd_idx<2; crd_idx++)            //loop coords
//                {
//                    if(prb->ele.fac_vtx_int[dim_idx][crd_idx]==0)   //ext face
//                    {
//                        prb->ele.fac_ext_flg = 1;
//                        prb->ele.fac_ext_dim = dim_idx;
//                        prb->ele.fac_ext_crd = crd_idx;
//                    }
//
//                    if(prb->ele.fac_vtx_int[dim_idx][crd_idx]==4)   //int face
//                    {
//                        prb->ele.fac_int_flg = 1;
//                        prb->ele.fac_int_dim = dim_idx;
//                        prb->ele.fac_int_crd = crd_idx;
//                    }
//                }
//            }
//
//            /*
//             ===============================
//             max verts per face
//             ===============================
//             */
//
//            int fac_max_int = prb->ele.fac_vtx_int[0][0];                   //init max
//            int fac_max_dim = 0;
//            int fac_max_crd = 0;
//
//
//            for(int dim_idx=0; dim_idx<3; dim_idx++)                        //loop dims
//            {
//                for(int crd_idx=0; crd_idx<2; crd_idx++)                    //loop coords
//                {
//                    int tmp_max = prb->ele.fac_vtx_int[dim_idx][crd_idx];
//
//                    if(tmp_max>fac_max_int)
//                    {
//                        fac_max_int = tmp_max;
//                        fac_max_dim = dim_idx;
//                        fac_max_crd = crd_idx;
//                    }
//                }
//            }
//

            /*
             ===============================
             vertex logic
             ===============================
             */

            switch (prb->ele.vtx_int_tot)                                                   //internal verts
            {
                case 1:                                                                     //1 int vtx
                {
                    break;                                                                  //break case
                }
                case 2:                                                                     //2 int vtx
                {
                    break;                                                                  //break case
                }
                case 3:                                                                     //3 int vtx
                {
//                    printf("fac_int\n");
//                    for(int i=0; i<3; i++)
//                    {
//                        for(int j=0; j<2; j++)
//                        {
//                            printf("%d ",prb->ele.fac_vtx_int[i][j]);                       //print fac_int
//                        }
//                        printf("\n");
//                    }
//
//                    printf("fac_max %d %d %d\n",fac_max_dim,fac_max_crd,fac_max_int);       //disp

                    break;                                                                  //break case
                }
                case 4:                                                                     //4 int vtx
                {


                    break;                                                                  //break case
                }
                case 5:                                                                     //5 int vtx
                {
                    break;                                                                  //break case
                }
                case 6:                                                                     //6 int vtx
                {
                    break;                                                                  //break case
                }
                case 7:                                                                     //7 int vtx
                {
                    break;                                                                  //break case
                }
            }
        }
    }

    /*
     ===============================
     debug
     ===============================
     */
    
    return;
}





//get some vertex indices for a given face (specify which face, how many, int/ext)
void fac_get_vtx(struct problem *prb, int fac_dim, int fac_crd, int vtx_int, int vtx_num)
{
    int j = 0;                                                                                  //position in return array
    
    for(int i=0; i<8; i++)                                                                      //loop all verts of ele
    {
        if( (((i>>fac_dim)&1) == fac_crd) && ((prb->ele.vtx_sdf[i]<0)==vtx_int))                //test face, (ie dim,coord) and int/ext
        {
            prb->ele.vtx_idx[j] = i;                                                            //store vertex index
            
            j += 1;                                                                             //increment strorage position
        }
        if(j==vtx_num)                                                                          //have you got them all?
        {
            break;                                                                              //exit search
        }
    }
    return;
}

//test if two verts are adjacent on the cube
int vtx_adj(struct problem *prb)
{
    float f = log2f(prb->ele.vtx_idx[0]^prb->ele.vtx_idx[1]);                                   //hamming distance==1 => XOR should be power of 2
    
    return (ceilf(f) == f);                                                                     //test for integer power of two
}


