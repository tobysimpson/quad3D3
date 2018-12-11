//
//  quadrature.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"


//quadrature on element
float quad_ele(struct problem *prb)
{
    float vlm = 0;                                                                      //reset total
    
    for(int k=0; k<prb->scm.np; k++)
    {
        for(int j=0; j<prb->scm.np; j++)
        {
            for(int i=0; i<prb->scm.np; i++)                                            //loop points/dims
            {
                prb->qpt.loc.x[0] = prb->scm.pp[i];                                     //local coords
                prb->qpt.loc.x[1] = prb->scm.pp[j];
                prb->qpt.loc.x[2] = prb->scm.pp[k];
                
                prb->qpt.loc.v = prb->scm.ww[i]*prb->scm.ww[j]*prb->scm.ww[k];          //quadrature weight
                
                float3_emul(prb->qpt.loc.x,prb->msh.ele_h,prb->qpt.glb.x);              //global coords
                float3_eadd(prb->qpt.glb.x, prb->ele.vtx[0][0][0].x, prb->qpt.glb.x);
 
                prb->qpt.glb.v = prb->qpt.loc.v;                                        //copy weight
                
                lst_add(&prb->lst2, prb->qpt.glb);                                      //store
                
                vlm += prb->qpt.loc.v;                                                  //sum for volume
            }
        }
    }
    vlm *= prb->msh.ele_vlm;                                                            //scale for element volume
    
    printf("quad_ele    %f %f\n",vlm,prb->vlm);
    
    return vlm;
}

//count int/ext faces
void quad_fac(struct problem *prb)
{
    int   fac_int[3][2] = {0,0,0,0,0,0};
    
    
    
    int vtx_int;                                                                        //is vtx interior
    
    for(int k=0; k<2; k++)                                                              //loop verts
    {
        for(int j=0; j<2; j++)
        {
            for(int i=0; i<2; i++)
            {
                vtx_int = (prb->ele.vtx[i][j][k].v < 0);                                //test vertex
                
                fac_int[0][i] += vtx_int;                                               //store sums
                fac_int[1][j] += vtx_int;
                fac_int[2][k] += vtx_int;
            }
        }
    }
    
    int flag_int_fac = 0;                                                               //flags for fully int ext faces
    int flag_ext_fac = 0;
    
    for(int dim_idx=0; dim_idx<3; dim_idx++)                                            //loop dims
    {
        for(int fac_idx=0; fac_idx<2; fac_idx++)
        {
            if(fac_int[dim_idx][fac_idx]==4)
            {
                flag_int_fac = 1;
            }
            
            if(fac_int[dim_idx][fac_idx]==0)
            {
                flag_ext_fac = 1;
            }
            
//            printf("quad_fac    %d %d | %d \n",dim_idx,fac_idx,fac_int[dim_idx][fac_idx]);
        }
    }
//    printf("flags %d %d | %d\n",flag_int_fac,flag_ext_fac,flag_int_fac||flag_ext_fac);
    
    if(!(flag_int_fac||flag_ext_fac))
    {
        printf("!!!!!!!!!!!!!this is bad!!!!!!!!!!!!!!!!!\n");
    }
    
    return;
}
