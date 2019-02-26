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
    
    if(prb->ele.pos[0]==(MSH_ELE_DIM_0-1))                                                                  //do running total
    {
        prb->vlm[1] += prb->vlm[0];                                                         //sum onto row total
        prb->vlm[0] = 0;                                                                    //reset subtotal
        
        if(prb->ele.pos[1]==(MSH_ELE_DIM_1-1))                                                              //do running total
        {
            prb->vlm[2] += prb->vlm[1];                                                     //sum onto main total
            prb->vlm[1] = 0;                                                                //reset subtotal
            
//            printf("ele_calc %10d | %3d %3d %3d | %d %d %d | %+f \n",
//                   prb->ele.idx,
//                   prb->ele.pos[0],
//                   prb->ele.pos[1],
//                   prb->ele.pos[2],
//                   prb->ele.vtx_int_tot,
//                   prb->ele.fac_ext_flg,
//                   prb->ele.fac_int_flg,
//                   prb->vlm[2]);
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
        
        prb->ele.vtx_sdf[vtx_idx] = geo_sdf(prb->ele.vtx_glb[vtx_idx]);                     //calc sdf
        
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
    

    
    switch (prb->ele.vtx_int_tot)
    {
        case 0:                                                     //all external
        {
            prb->ele.ctr.ele_ext += 1;                              //increment counter
            
            //do nothing
            break;
        }
        case 8:                                                     //all internal
        {
            prb->ele.ctr.ele_int += 1;                              //increment counter
            
            prb->ele.vlm_loc += quad_ele(prb);                      //quad on whole element
            
            break;
        }
        default:                                                    //both
        {
            /*
             ===============================
             find int/ext faces
             ===============================
             */
            
            prb->ele.fac_ext_flg = 0;                               //reset flags
            prb->ele.fac_int_flg = 0;
            
            for(int dim_idx=0; dim_idx<3; dim_idx++)                //loop dims
            {
                for(int crd_idx=0; crd_idx<2; crd_idx++)            //loop coords
                {
                    if(prb->ele.fac_vtx_int[dim_idx][crd_idx]==0)   //ext face
                    {
                        prb->ele.fac_ext_flg = 1;
                        prb->ele.fac_ext_dim = dim_idx;
                        prb->ele.fac_ext_crd = crd_idx;
                    }
                    
                    if(prb->ele.fac_vtx_int[dim_idx][crd_idx]==4)   //int face
                    {
                        prb->ele.fac_int_flg = 1;
                        prb->ele.fac_int_dim = dim_idx;
                        prb->ele.fac_int_crd = crd_idx;
                    }
                }
            }
            
            /*
             ===============================
             external face logic
             ===============================
             */
            
            if(prb->ele.fac_ext_flg)
            {
                prb->ele.ctr.fac_ext += 1;                                                      //increment counter
                
//                printf("fac_ext %d %d %d\n",prb->ele.fac_ext_flg,prb->ele.fac_ext_dim,prb->ele.fac_ext_crd);
                
                switch (prb->ele.fac_vtx_int[prb->ele.fac_ext_dim][!prb->ele.fac_ext_crd])      //count internal verts on opposite face
                {
                    case 1:                                                                     //external face with one internal vertex opposite
                    {
                        prb->ele.ctr.fac_ext_v1 += 1;
                        
//                        lst_add_ele(&prb->lst2, prb);
                    
                        fac_get_vtx(prb, prb->ele.fac_ext_dim, !prb->ele.fac_ext_crd, 1, 1);    //find 1 int vtx on opp fac
                        
                        prb->ele.vlm_loc += quad_vtx1(prb);                                             //do quadrature around the one internal vertex

                        break;                                                                  //break case
                    }
                    case 2:                                                                     //2 ext verts opposite ext face
                    {
                        prb->ele.ctr.fac_ext_v2 += 1;
                        
//                        lst_add_ele(&prb->lst2, prb);
                        
                        fac_get_vtx(prb, prb->ele.fac_ext_dim, !prb->ele.fac_ext_crd, 1, 2);    //find two internal vertices

                        if(vtx_adj(prb))                                                        //test if vtxs are adjacent
                        {
                            prb->ele.vlm_loc += quad_vtx2(prb);                                         //adjacent do quadrature
                        }
                        else
                        {
                            printf("ext_v2  2 non-adjacent verts\n");                                     //opposite implement later
                        }
                        break;                                                                  //break case
                    }
                    case 3:                                                                     //3 ext verts opp ext face
                    {
                        prb->ele.ctr.fac_ext_v3 += 1;
                        
//                        lst_add_ele(&prb->lst2, prb);
                     
                        prb->ele.fac_int_dim = prb->ele.fac_ext_dim;                            //set the 3-face as internal
                        prb->ele.fac_int_crd = !prb->ele.fac_ext_crd;
                        
                        prb->ele.vlm_loc += quad_vtx4(prb);                                             //do quadrature on the 3-internal face
                        
                        fac_get_vtx(prb, prb->ele.fac_ext_dim, !prb->ele.fac_ext_crd, 0, 1);    //find external vertex in opposite face
                        
                        prb->ele.vlm_loc -= quad_vtx1(prb);                                             //subtract quadrature around the one external vertex
                        
                        break;
                    }
                    case 4:                                                                     //both internal and external face
                    {
                        prb->ele.ctr.fac_ext_v4 += 1;
                        
//                        lst_add_ele(&prb->lst2, prb);
                        
                        prb->ele.vlm_loc += quad_vtx4(prb);                                             //quad on face
                        
                        break;
                    }
                }
            }
            
            /*
             ===============================
             internal face logic
             ===============================
             */

            else if(prb->ele.fac_int_flg)
            {
                prb->ele.ctr.fac_int += 1;                                                      //increment counter
                
//                printf("fac_int %d %d %d\n",prb->ele.fac_int_flg,prb->ele.fac_int_dim,prb->ele.fac_int_crd);
                
                switch (prb->ele.fac_vtx_int[prb->ele.fac_int_dim][!prb->ele.fac_int_crd])      //count internal verts on opposite face
                {
                    case 1:                                                                     //internal face with one internal vertex opposite
                    {
                        prb->ele.ctr.fac_int_v1 += 1;
                        
//                        lst_add_ele(&prb->lst2, prb);
                        
                        prb->ele.vlm_loc += quad_vtx4(prb);                                             //do quadrature on the 4-internal face
                        
                        fac_get_vtx(prb, prb->ele.fac_int_dim, !prb->ele.fac_int_crd, 1, 1);    //find internal vertex opposite internal face
                        
                        prb->ele.vlm_loc -= quad_vtx1(prb);                                             //subtract quadrature around the one external vertex

                        break;
                    }
                    case 2:                                                                     //two internal vertex opposite internal face
                    {
                        prb->ele.ctr.fac_int_v2 += 1;
                        
//                        lst_add_ele(&prb->lst2, prb);
                        
                        prb->ele.vlm_loc += quad_ele(prb);                                              //quad on whole element
                        
                        fac_get_vtx(prb, prb->ele.fac_int_dim, !prb->ele.fac_int_crd, 0, 2);    //find two external vertices
                        
                        if(vtx_adj(prb))                                                        //test if vtxs are adjacent
                        {
                            prb->ele.vlm_loc -= quad_vtx2(prb);                                         //adjacent subtract quadrature
                        }
                        else
                        {
                            printf("int_v2  2 non-adjacent verts\n");                                     //opposite implement later
                        }
                        break;
                    }
                    case 3:                                                                     //internal face with three internal vertex opposite
                    {
                        prb->ele.ctr.fac_int_v3 += 1;
                        
//                        lst_add_ele(&prb->lst2, prb);
                        
                        prb->ele.vlm_loc += quad_ele(prb);                                              //quad on whole element
                        
                        fac_get_vtx(prb, prb->ele.fac_int_dim, !prb->ele.fac_int_crd, 0, 1);    //find external vertex in opposite face
                        
                        prb->ele.vlm_loc -= quad_vtx1(prb);                                             //subtract quadrature around the one external vertex

                        break;
                    }
                }
            }
            
            /*
             ===============================
             no internal or external face
             ===============================
             */
            
            else                                                                                //no int or ext
            {
                prb->ele.ctr.fac_oth += 1;                                                      //increment counter
                
//                printf("no int/ext\n");
//                lst_add_ele(&prb->lst2, prb);
                
                
                prb->ele.fac_ext_flg = 0;                               //reset flags
                prb->ele.fac_int_flg = 0;
                
                
                //calc gradient of sdf approx in centre of cube - for numerical stability
                float grad[3];
                
                grad[0] = prb->ele.bas_aa[1] + 0.5*(prb->ele.bas_aa[4] + prb->ele.bas_aa[5]);           //grad at centre
                grad[1] = prb->ele.bas_aa[2] + 0.5*(prb->ele.bas_aa[4] + prb->ele.bas_aa[6]);
                grad[2] = prb->ele.bas_aa[3] + 0.5*(prb->ele.bas_aa[5] + prb->ele.bas_aa[6]);
                
                float   grad_max = fabsf(grad[0]);                                                      //init max abs grad
                int     grad_idx = 0;
                
                for(int dim_idx=0; dim_idx<3; dim_idx++)                                                //loop dims
                {
                    if(fabsf(grad[dim_idx])>grad_max)
                    {
                        grad_max = fabsf(grad[dim_idx]);
                        grad_idx = dim_idx;
                    }
                }
                
                prb->ele.fac_int_flg = 1;
                prb->ele.fac_int_dim = grad_idx;
                prb->ele.fac_int_crd = grad[grad_idx]<0;
                
                prb->ele.vlm_loc += quad_vtx4(prb);                                                     //do integral of the base
                
                fac_get_vtx(prb, prb->ele.fac_int_dim, prb->ele.fac_int_crd, 0, 1);                     //find the external vertex on the base
                
                prb->ele.vlm_loc -= quad_vtx1(prb);                                                     //add quad around the vertex below
                
                fac_get_vtx(prb, prb->ele.fac_int_dim, !prb->ele.fac_int_crd, 1, 1);                    //find the internal vertex on opposite face
                
                prb->ele.vlm_loc -= quad_vtx1(prb);                                                     //subtract quad around the vertex
                
//                //need a face with 3-int verts opp 3 ext verts - and you need the gradient as well - do you get 3 verts for sure?
//                for(int dim_idx=0; dim_idx<3; dim_idx++)                //loop dims
//                {
//                    for(int crd_idx=0; crd_idx<2; crd_idx++)            //loop coords
//                    {
//                        if(prb->ele.fac_vtx_int[dim_idx][crd_idx]==3)   //find face with 3 int verts
//                        {
//                            prb->ele.fac_int_flg = 1;
//                            prb->ele.fac_int_dim = dim_idx;
//                            prb->ele.fac_int_crd = crd_idx;
//
//                            break;
//                        }
//                    }
//                }
//                if(prb->ele.fac_int_flg)
//                {
//                    prb->ele.vlm_loc += quad_vtx4(prb);                                                     //do integral of the base
//
//                    fac_get_vtx(prb, prb->ele.fac_int_dim, prb->ele.fac_int_crd, 0, 1);             //find the external vertex on the base
//
//                    prb->ele.vlm_loc -= quad_vtx1(prb);                                                     //add quad around the vertex below
//
//                    fac_get_vtx(prb, prb->ele.fac_int_dim, !prb->ele.fac_int_crd, 1, 1);            //find the internal vertex on opposite face
//
//                    prb->ele.vlm_loc -= quad_vtx1(prb);                                                     //subtract quad around the vertex above
//                }
//                else
//                {
//                    printf("no 3-face!\n");
//                }
            }
        }
    }
    
    /*
     ===============================
     debug
     ===============================
     */
    

    prb->vlm[0] += prb->ele.vlm_loc;                                                                           //add to running total
    
//    if(prb->ele.vtx_int_tot>0)//&&(prb->ele.vtx_int_tot<8))
//    if(prb->ele.pos[2]==0)
//    {
//        printf("ele_calc %10d | %3d %3d %3d | %d %d %d | %+f %+f %+f +%f\n",
//               prb->ele.idx,
//               prb->ele.pos[0],
//               prb->ele.pos[1],
//               prb->ele.pos[2],
//               prb->ele.vtx_int_tot,
//               prb->ele.fac_ext_flg,
//               prb->ele.fac_int_flg,
//               prb->ele.vlm_loc,
//               prb->vlm[0],
//               prb->vlm[1],
//               prb->vlm[2]);
//    }
    
    return;
}

//get some vertex indices for a given face (specify which face, how many, int/ext)
void fac_get_vtx(struct problem *prb, int fac_dim, int fac_crd, int vtx_intext, int vtx_num)
{
    int j = 0;                                                                                  //position in return array
    
    for(int i=0; i<8; i++)                                                                      //loop all verts of ele
    {
        if( (((i>>fac_dim)&1) == fac_crd) && ((prb->ele.vtx_sdf[i]<0)==vtx_intext))             //test face, (ie dim,coord) and int/ext
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



