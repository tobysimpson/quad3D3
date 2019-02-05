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
//    float vlm = 0;                                                                      //reset total
//
//    for(int k=0; k<prb->scm.np; k++)
//    {
//        for(int j=0; j<prb->scm.np; j++)
//        {
//            for(int i=0; i<prb->scm.np; i++)                                            //loop points/dims
//            {
//                prb->qpt.loc.x[0] = prb->scm.pp[i];                                     //local coords
//                prb->qpt.loc.x[1] = prb->scm.pp[j];
//                prb->qpt.loc.x[2] = prb->scm.pp[k];
//
//                prb->qpt.loc.v = prb->scm.ww[i]*prb->scm.ww[j]*prb->scm.ww[k];          //quadrature weight
//
//                float3_emul(prb->qpt.loc.x,prb->msh.ele_h,prb->qpt.glb.x);              //global coords
//                float3_eadd(prb->qpt.glb.x, &prb->ele.vtx_glb[0], prb->qpt.glb.x);
//
//                prb->qpt.glb.v = prb->qpt.loc.v;                                        //copy weight
//
////                lst_add(&prb->lst2, prb->qpt.glb);                                      //store
//
//                vlm += prb->qpt.loc.v;                                                  //sum for volume
//            }
//        }
//    }
//    vlm *= prb->msh.ele_vlm;                                                            //scale for element volume
//
//    printf("quad_ele    %f %f\n",vlm,prb->vlm);
//
//    return vlm;
    
    return 0;
}
