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
    
    /*
     ===============================
     generate vertices & test sdf
     ===============================
     */
    
    float3_emul_int(prb->msh.ele_h, prb->ele.pos, prb->ele.vtx_glb[0]);                     //set reference vertex
    float3_eadd(prb->ele.vtx_glb[0], prb->msh.xmin, prb->ele.vtx_glb[0]);
    
    prb->ele.vtx_int_tot = 0;                                                                   //reset count
    
    //    memset(&prb->ele.fac_int,0,6&sizeof(float));                                      //reset count
    
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<2; j++)
        {
            prb->ele.fac_vtx_int[i][j] = 0;                                                 //reset fac_int
        }
    }
    
    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)
    {
        char i = (vtx_idx>>0)&1;                                                            //binary digits
        char j = (vtx_idx>>1)&1;
        char k = (vtx_idx>>2)&1;
        
        prb->ele.vtx_glb[vtx_idx][0] = prb->ele.vtx_glb[0][0] + prb->msh.ele_h[0]*i;        //vtx coords
        prb->ele.vtx_glb[vtx_idx][1] = prb->ele.vtx_glb[0][1] + prb->msh.ele_h[1]*j;
        prb->ele.vtx_glb[vtx_idx][2] = prb->ele.vtx_glb[0][2] + prb->msh.ele_h[2]*k;
        
        prb->ele.vtx_sdf[vtx_idx] = geo_sdf(prb->ele.vtx_glb[vtx_idx]);                     //calc sdf
        
        prb->ele.vtx_int_tot += (prb->ele.vtx_sdf[vtx_idx] < 0);                                //count internal verts
        
        lst_add(&prb->lst1, prb->ele.vtx_glb[vtx_idx], prb->ele.vtx_sdf[vtx_idx]);          //add to list
        
        //printf("vtx_gen %d %d %d | %+e %+e %+e | %d\n",i,j,k,prb->ele.vtx_glb[vtx_idx][0],prb->ele.vtx_glb[vtx_idx][1],prb->ele.vtx_glb[vtx_idx][2],prb->ele.vtx_sdf[vtx_idx]<0);
        
        prb->ele.fac_vtx_int[0][i] += prb->ele.vtx_sdf[vtx_idx]<0;                          //calc internal verts per face
        prb->ele.fac_vtx_int[1][j] += prb->ele.vtx_sdf[vtx_idx]<0;
        prb->ele.fac_vtx_int[2][k] += prb->ele.vtx_sdf[vtx_idx]<0;
    }
    
    
    /*
     ===============================
     interpolated basis function coeffs
     ===============================
     */
    
    
//    float prb->ele.bas_aa[8];
    
    prb->ele.bas_aa[0] = + prb->ele.vtx_sdf[0];                          //1
    
    prb->ele.bas_aa[1] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[1];    //x
    prb->ele.bas_aa[2] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[2];    //y
    prb->ele.bas_aa[3] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[4];    //z
    
    prb->ele.bas_aa[4] = + prb->ele.vtx_sdf[0] - prb->ele.vtx_sdf[1] - prb->ele.vtx_sdf[2] + prb->ele.vtx_sdf[3];    //xy
    prb->ele.bas_aa[5] = + prb->ele.vtx_sdf[0] - prb->ele.vtx_sdf[1] - prb->ele.vtx_sdf[4] + prb->ele.vtx_sdf[5];    //xz
    prb->ele.bas_aa[6] = + prb->ele.vtx_sdf[0] - prb->ele.vtx_sdf[2] - prb->ele.vtx_sdf[4] + prb->ele.vtx_sdf[6];    //xz
    
    prb->ele.bas_aa[7] = - prb->ele.vtx_sdf[0] + prb->ele.vtx_sdf[1] + prb->ele.vtx_sdf[2] - prb->ele.vtx_sdf[3]
                         + prb->ele.vtx_sdf[4] - prb->ele.vtx_sdf[5] - prb->ele.vtx_sdf[6] + prb->ele.vtx_sdf[7]; //xyz

    /*
     ===============================
     debug print
     ===============================
     */
    
//    printf("vtx_int %d\n",prb->ele.vtx_int);
    
//    printf("fac_int\n");
//    for(int i=0; i<3; i++)
//    {
//        for(int j=0; j<2; j++)
//        {
//            printf("%d ",prb->ele.fac_vtx_int[i][j]);        //debug fac_int
//        }
//        printf("\n");
//    }
//    printf("\n");
    
    
    /*
     ===============================
     quadrature logic
     ===============================
     */
    
    switch (prb->ele.vtx_int_tot)
    {
        case 0:                                                     //all external
        {
            //do nothing
            break;
        }
        case 8:                                                     //all internal
        {
            prb->vlm += quad_ele(prb);                              //quad on whole element
            
            break;
        }
        default:                                                    //both
        {
            /*
             ===============================
             find int/ext faces
             ===============================
             */
            
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
             face logic
             ===============================
             */
            
            if(prb->ele.fac_ext_flg)
            {
                printf("fac_ext %d %d %d\n",prb->ele.fac_ext_flg,prb->ele.fac_ext_dim,prb->ele.fac_ext_crd);
                
                switch (prb->ele.fac_vtx_int[prb->ele.fac_ext_dim][!prb->ele.fac_ext_crd])      //count verts on opposite face
                {
                    case 1:                                                                     //external face with one internal vertex opposite
                    {
                        lst_add_ele(&prb->lst2, prb);
                        
                        ele_get_fac(prb,prb->ele.fac_ext_dim,!prb->ele.fac_ext_crd);            //get the face opposite the external one
                        
                        prb->vlm += quad_vtx1(prb);                                             //do quadrature around the one internal vertex

                        break;
                    }
                    case 2:
                    {
                        //quad along edge OR quad around two verts

                        break;
                    }
                    case 3:
                    {
                        //quad ele minus vertex

                        break;
                    }
                }
            }
            else if(prb->ele.fac_int_flg)
            {
                printf("fac_int %d %d %d\n",prb->ele.fac_int_flg,prb->ele.fac_int_dim,prb->ele.fac_int_crd);
                
//                lst_add_ele(&prb->lst3, prb);
            }
            else
            {
                printf("no int/ext\n");
                
//                lst_add_ele(&prb->lst4, prb);
            }
        }
    }
    return;
}

//get vertex indices for a given face
void ele_get_fac(struct problem *prb, int fac_dim, int fac_crd)
{
    int fac_vtx_idx = 0;                                //position in return array
    
    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)            //loop all verts of ele
    {
        if(((vtx_idx>>fac_dim)&1) == fac_crd)           //select vertex byt binary digit
        {
            prb->ele.fac_vtx[fac_vtx_idx] = vtx_idx;    //store vertex index
            
            fac_vtx_idx += 1;                           //increment strorage position
        }
    }
    return;
}




