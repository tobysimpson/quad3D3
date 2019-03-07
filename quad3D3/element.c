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
     reset ele
     ===============================
     */
    
    prb->ele.vtx_int_num = 0;                                                               //reset count
    
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<2; j++)
        {
            prb->ele.fac_vtx_int[i][j] = 0;                                                 //reset fac_int
        }
    }
    
    /*
     ===============================
     volume subtotal
     ===============================
     */
    
    prb->ele.vlm_loc = 0;                                                                   //volume subtotal
    
    if(prb->ele.pos[0]==(MSH_ELE_DIM_0-1))                                                  //final element of row
    {
        prb->vlm[1] += prb->vlm[0];                                                         //sum onto row total
        prb->vlm[0] = 0;                                                                    //reset subtotal
        
        if(prb->ele.pos[1]==(MSH_ELE_DIM_1-1))                                              //final element of slice
        {
            prb->vlm[2] += prb->vlm[1];                                                     //sum onto main total
            prb->vlm[1] = 0;                                                                //reset subtotal
        }
    }
    
    /*
     ===============================
     generate vertices & test sdf
     ===============================
     */
    
    float3_emul_int3(prb->msh.ele_h, prb->ele.pos, prb->ele.vtx_glb[0]);                                            //set reference vertex
    float3_eadd(prb->ele.vtx_glb[0], prb->msh.xmin, prb->ele.vtx_glb[0]);
    
    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)                                                                        //loop verts
    {
        int i = (vtx_idx>>0)&1;                                                                                     //binary digits xyz coords
        int j = (vtx_idx>>1)&1;
        int k = (vtx_idx>>2)&1;
        
        prb->ele.vtx_glb[vtx_idx][0] = prb->ele.vtx_glb[0][0] + prb->msh.ele_h[0]*i;                                //vtx coords
        prb->ele.vtx_glb[vtx_idx][1] = prb->ele.vtx_glb[0][1] + prb->msh.ele_h[1]*j;
        prb->ele.vtx_glb[vtx_idx][2] = prb->ele.vtx_glb[0][2] + prb->msh.ele_h[2]*k;

//        prb->ele.vtx_sdf[vtx_idx] = geo_sdf(prb, prb->ele.vtx_glb[vtx_idx]);                                      //calc sdf
        prb->ele.vtx_sdf[vtx_idx] = prb->geo.dof_sdf[prb->ele.pos[0]+i][prb->ele.pos[1]+j][prb->ele.pos[2]+k];      //retrieve sdf from array
        
        prb->ele.vtx_int_num += (prb->ele.vtx_sdf[vtx_idx] < 0);                                                    //count internal verts
        
        prb->ele.fac_vtx_int[0][i] += prb->ele.vtx_sdf[vtx_idx]<0;                                                  //calc internal verts per face
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
    + prb->ele.vtx_sdf[4] - prb->ele.vtx_sdf[5] - prb->ele.vtx_sdf[6] + prb->ele.vtx_sdf[7];                            //xyz
    
    /*
     ===============================
     debug print fac_vtx_int
     ===============================
     */
    
    //    printf("vtx_int %d\n",prb->ele.vtx_int_tot);
    //
    //    printf("fac_vtx_int\n");
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
     test path connectedness
     ===============================
     */
    
    //    prb->ele.ctr.vtx_int[0][prb->ele.vtx_int_num] += 1;                     //increment counter
    
    //    int pth_con = ele_pth_test(prb);                                        //test path connectedness
    //
    //    prb->ele.ctr.vtx_int[1][prb->ele.vtx_int_num] += pth_con;               //count path conn
    //
    //    if(!pth_con)
    //    {
    //        lst_add_ele(&prb->lst2, prb);                                        //have a look
    //    }
    
    /*
     ===============================
     quadrature logic
     ===============================
     */
    
    
    switch (prb->ele.vtx_int_num)
    {
        case 0:                                                             //all external
        {
            prb->ele.ctr.vtx_int[0][0] += 1;                                //increment counter
            
            break;
        }
        case 8:                                                             //all internal
        {
            prb->ele.ctr.vtx_int[4][4] += 1;                                //increment counter
            
            break;
        }
        default:                                                            //both
        {
            //            lst_add_ele(&prb->lst1, prb);                                   //have a look
            
            //            printf("vtx_int %d\n",prb->ele.vtx_int_num);
            
            
            /*
             ===============================
             select dimension for integration
             ===============================
             */
            
            int   diff[3];                                                                              //change in int/ext verts per dim

            prb->ele.bf_dim = 0;                                                                       //integration dim (max chg)
            
            for(int dim_idx=0; dim_idx<3; dim_idx++)                                                    //loop dims
            {
                diff[dim_idx] = prb->ele.fac_vtx_int[dim_idx][1] - prb->ele.fac_vtx_int[dim_idx][0];    //calc diff per dim
                
                if(abs(diff[dim_idx])>=abs(diff[prb->ele.bf_dim]))                                     //look for max change in vertex count (>=)
                {
                    prb->ele.bf_dim = dim_idx;                                                     //store dim index
                }
//                printf("fac_vtx_int     %d | %d %d | %+d %d\n",
//                       dim_idx,
//                       prb->ele.fac_vtx_int[dim_idx][0],
//                       prb->ele.fac_vtx_int[dim_idx][1],
//                       diff[dim_idx],
//                       diff[dim_idx]>0);
            }
            prb->ele.bf_crd = (diff[prb->ele.bf_dim] > 0);                                           //set base face coord
            
//            printf("dim_max         %d | %d %d | %+d | %d %d \n",
//                   prb->ele.bas_dim,
//                   prb->ele.fac_vtx_int[prb->ele.bas_dim][0],
//                   prb->ele.fac_vtx_int[prb->ele.bas_dim][1],
//                   diff[prb->ele.bas_dim],
//                   prb->ele.bas_crd,
//                   prb->ele.fac_vtx_int[prb->ele.bas_dim][prb->ele.bas_crd] > prb->ele.fac_vtx_int[prb->ele.bas_dim][!prb->ele.bas_crd]);
            
            
            /*
             ===============================
             vertex logic
             ===============================
             */
            
            
            int i = prb->ele.fac_vtx_int[prb->ele.bf_dim][prb->ele.bf_crd];                 //indices for counter
            int j = prb->ele.fac_vtx_int[prb->ele.bf_dim][!prb->ele.bf_crd];
            
            prb->ele.ctr.vtx_int[i][j] += 1;                                                //count different configurations
            
            switch (prb->ele.fac_vtx_int[prb->ele.bf_dim][prb->ele.bf_crd])                 //test internal verts on base face
            {
                case 1:                                                                     //1 base
                {
//                    lst_add_ele(&prb->lst1, prb);
                    
//                    fac_get_vtx(prb, prb->ele.bf_dim, prb->ele.bf_crd, 1);                  //find internal vertex on base face
                    
//                    prb->ele.vlm_loc += quad_vtx1(prb);                                     //add internal corner
    
                    break;                                                                  //break base case
                }
                case 2:                                                                     //2 base
                {
//                    lst_add_ele(&prb->lst1, prb);
//
//                    fac_get_vtx(prb, prb->ele.bf_dim, prb->ele.bf_crd, 1);                  //find internal verts on base face
//
//                    prb->ele.vlm_loc += quad_vtx2(prb);                                     //do quad on 2 points
                    
                    break;                                                                  //break base case
                }
                case 3:                                                                     //3 base
                {
                    switch (prb->ele.fac_vtx_int[prb->ele.bf_dim][!prb->ele.bf_crd])        //test internal verts on opposite face
                    {
                        case 0:                                                             //0 opp
                        {
//                            lst_add_ele(&prb->lst1, prb);
//
//                            prb->ele.vlm_loc += quad_vtx4(prb);                             //add quad on 4 verts
//
//                            fac_get_vtx(prb, prb->ele.bf_dim, prb->ele.bf_crd, 0);          //find ext vert on base face
//
//                            prb->ele.vlm_loc -= quad_vtx1(prb);                             //subtract external corner
                            
                            break;                                                          //break opposite case
                        }
                        case 1:                                                             //1 opp
                        {
//                            lst_add_ele(&prb->lst1, prb);
//                            
//                            prb->ele.vlm_loc += quad_vtx4(prb);                             //add quad on 4 verts
//
//                            fac_get_vtx(prb, prb->ele.bf_dim, prb->ele.bf_crd, 0);          //find ext vert on base face
//
//                            prb->ele.vlm_loc -= quad_vtx1(prb);                             //subtract external corner
//
//                            fac_get_vtx(prb, prb->ele.bf_dim, !prb->ele.bf_crd, 1);          //find int vert on opp face
//
//                            prb->ele.vlm_loc -= quad_vtx1(prb);                             //subtract internal corner
                            
                            break;                                                          //break opposite case
                        }
                    }
                    break;                                                                  //break base case
                }
                case 4:                                                                     //4 base
                {
                    switch (prb->ele.fac_vtx_int[prb->ele.bf_dim][!prb->ele.bf_crd])        //test internal verts on opposite face
                    {
                        case 0:                                                             //0 opp
                        {
//                            lst_add_ele(&prb->lst1, prb);
//
//                            prb->ele.vlm_loc += quad_vtx4(prb);                             //add quad on 4 verts
                            
                            break;                                                          //break opposite case
                        }
                        case 1:                                                             //1 opp
                        {
                            break;                                                          //break opposite case
                        }
                        case 2:                                                             //2 opp
                        {
//                            lst_add_ele(&prb->lst1, prb);
//
//                            fac_get_vtx(prb, prb->ele.bf_dim, !prb->ele.bf_crd, 0);         //find external verts on opposite face
//
//                            prb->ele.vlm_loc += (1 - quad_vtx2(prb));                       //subtract quad on 2 external points
                            
                            break;                                                          //break opposite case
                        }
                        case 3:                                                             //3 opp
                        {
//                            lst_add_ele(&prb->lst1, prb);
//
//                            fac_get_vtx(prb, prb->ele.bf_dim, !prb->ele.bf_crd, 0);         //find external vertex on opposite face
//
//                            prb->ele.vlm_loc += (1 - quad_vtx1(prb));                       //subtract external corner
                            
                            break;                                                          //break opposite case
                        }
                    }
                    break;                                                                  //break base case
                }
            }
        }
    }
    
    prb->vlm[0] += prb->ele.vlm_loc;                                                        //add to running total
    
    /*
     ===============================
     debug
     ===============================
     */
    
    return;
}


//get some vertex indices for a given face (specify which face, how many, int/ext)
int fac_get_vtx(struct problem *prb, int fac_dim, int fac_crd, int vtx_int)
{
    int j = 0;                                                                                  //position in return array
    
    for(int i=0; i<8; i++)                                                                      //loop all verts of ele
    {
        if( (((i>>fac_dim)&1) == fac_crd) && ((prb->ele.vtx_sdf[i]<0)==vtx_int))                //test face, (ie dim,coord) and int/ext
        {
            prb->ele.vtx_idx[j] = i;                                                            //store vertex index
            
            j += 1;                                                                             //increment strorage position
        }
    }
    return j;                                                                                   //return how many you found
}

//test if two verts are adjacent on the cube
int vtx_adj(struct problem *prb)
{
    float f = log2f(prb->ele.vtx_idx[0]^prb->ele.vtx_idx[1]);                                   //hamming distance==1 => XOR should be power of 2
    
    return (ceilf(f) == f);                                                                     //test for integer power of two
}

