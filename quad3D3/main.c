//
//  main.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//3D quadrature using basis, gauss and vertex logic
//works and converges in linear fashion - needs extra logic for non adjacent vertices
//needs surface area


int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    /*
     ===================
     initialise
     ===================
     */
    
    struct problem *prb = malloc(sizeof(struct problem));       //allocate memory
    
    prb->vlm[0] = 0;                                            //init volume
    prb->vlm[1] = 0;
    prb->vlm[2] = 0;
    
    scm_init(prb);                                              //init scheme
    msh_init(prb);                                              //init mesh
    
    lst_init(&prb->lst1);                                       //init list
    lst_init(&prb->lst2);                                       //init list
    lst_init(&prb->lst3);                                       //init list
    lst_init(&prb->lst4);                                       //init list

    prb->ele.ctr.ele_ext =  0;                                  //reset counters
    prb->ele.ctr.ele_int =  0;
    prb->ele.ctr.fac_ext =  0;
    prb->ele.ctr.fac_int =  0;
    prb->ele.ctr.fac_oth =  0;
    
    prb->ele.ctr.fac_ext_v1  = 0;
    prb->ele.ctr.fac_ext_v2  = 0;
    prb->ele.ctr.fac_ext_v3  = 0;
    prb->ele.ctr.fac_ext_v4  = 0;
    
    prb->ele.ctr.fac_int_v1  = 0;
    prb->ele.ctr.fac_int_v2  = 0;
    prb->ele.ctr.fac_int_v3  = 0;
    prb->ele.ctr.fac_int_v4  = 0;
    
    /*
     ===================
     loop elements
     ===================
     */
    
    prb->ele.idx = 0;                                           //init counter
    
    //loop
    for(int k=0; k<MSH_ELE_DIM_2; k++)
    {
        for(int j=0; j<MSH_ELE_DIM_1; j++)
        {
            for(int i=0; i<MSH_ELE_DIM_0; i++)
            {
                prb->ele.pos[0] = i;                            //position in ele space (i,j,k)
                prb->ele.pos[1] = j;
                prb->ele.pos[2] = k;
                
                ele_calc(prb);                                  //process the element
                
                prb->ele.idx += 1;                              //increment element counter
            }
        }
    }
    
    printf("prb_vlm         %4d,%d,%f,%f,%f; \n",MSH_ELE_DIM, MSH_ELE_TOT , prb->vlm[2]*prb->msh.ele_vlm, M_PI*4e0/3e0, (M_PI*0.3*0.3)*(2*M_PI*0.7));   //sphere
//    printf("prb_vlm         %4d,%d,%f,%f; \n",MSH_ELE_DIM, MSH_ELE_TOT , prb->vlm[2]*prb->msh.ele_vlm, (M_PI*0.3*0.3)*(2*M_PI*0.7));
    
    printf("ele_ext         %8d %6.3f\n",prb->ele.ctr.ele_ext,(100e0*prb->ele.ctr.ele_ext)/(MSH_ELE_TOT));       //display counters
    printf("ele_int         %8d %6.3f\n",prb->ele.ctr.ele_int,(100e0*prb->ele.ctr.ele_int)/(MSH_ELE_TOT));
    printf("fac_ext         %8d %6.3f\n",prb->ele.ctr.fac_ext,(100e0*prb->ele.ctr.fac_ext)/(MSH_ELE_TOT));
    printf("fac_int         %8d %6.3f\n",prb->ele.ctr.fac_int,(100e0*prb->ele.ctr.fac_int)/(MSH_ELE_TOT));
    printf("fac_oth         %8d %6.3f\n",prb->ele.ctr.fac_oth,(100e0*prb->ele.ctr.fac_oth)/(MSH_ELE_TOT));
    
    printf("fac_ext_v1      %8d\n",prb->ele.ctr.fac_ext_v1);
    printf("fac_ext_v2      %8d\n",prb->ele.ctr.fac_ext_v2);
    printf("fac_ext_v3      %8d\n",prb->ele.ctr.fac_ext_v3);
    printf("fac_ext_v4      %8d\n",prb->ele.ctr.fac_ext_v4);
    
    printf("fac_ext_sum     %8d\n",prb->ele.ctr.fac_ext_v1+prb->ele.ctr.fac_ext_v2+prb->ele.ctr.fac_ext_v3+prb->ele.ctr.fac_ext_v4);
    
    printf("fac_int_v1      %8d\n",prb->ele.ctr.fac_int_v1);
    printf("fac_int_v2      %8d\n",prb->ele.ctr.fac_int_v2);
    printf("fac_int_v3      %8d\n",prb->ele.ctr.fac_int_v3);
    printf("fac_int_v4      %8d\n",prb->ele.ctr.fac_int_v4);
    
    printf("fac_int_sum     %8d\n",prb->ele.ctr.fac_int_v1+prb->ele.ctr.fac_int_v2+prb->ele.ctr.fac_int_v3+prb->ele.ctr.fac_int_v4);
    
    printf("fac_sum         %8d\n",prb->ele.ctr.ele_ext+prb->ele.ctr.ele_int+prb->ele.ctr.fac_ext+prb->ele.ctr.fac_int+prb->ele.ctr.fac_oth);
    
    /*
     ===================
     debug
     ===================
     */

//    lst_write(&prb->lst1, "list1");
//    lst_write(&prb->lst2, "list2");
//    lst_write(&prb->lst3, "list3");
//    lst_write(&prb->lst4, "list4");

    /*
     ===================
     clean up
     ===================
     */
    
    free(prb);                  //problem
    
    printf("done\n");
    
    return 0;
}
