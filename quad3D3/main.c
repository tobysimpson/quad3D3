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
    geo_init(prb);
    
    lst_init(&prb->lst1);                                       //init list
    lst_init(&prb->lst2);                                       //init list
    lst_init(&prb->lst3);                                       //init list
    lst_init(&prb->lst4);                                       //init list
    
    /*
     ===================
     reset counters
     ===================
     */

    
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            for(int k=0; k<2; k++)
            {
                prb->ele.ctr.vtx_int[i][j][k] = 0;
            }
        }
    }
    
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
                prb->ele.pos[0] = i;                            //position in ele space (i,j,k) - this is wrong should be stored (z,y,x)
                prb->ele.pos[1] = j;
                prb->ele.pos[2] = k;
                
                ele_calc(prb);                                  //process the element
                
                prb->ele.idx += 1;                              //increment element counter
            }
        }
    }
    
//    printf("prb_vlm         %4d,%d,%f,%f; \n",MSH_ELE_DIM, MSH_ELE_TOT , prb->vlm[2]*prb->msh.ele_vlm, (1e0/8e0)*M_PI*4e0/3e0);                   //sphere
    printf("prb_vlm         %4d,%d,%f,%f; \n",MSH_ELE_DIM, MSH_ELE_TOT , prb->vlm[2]*prb->msh.ele_vlm, (1e0/8e0)*(M_PI*0.3*0.3)*(2*M_PI*0.7));    //torus
    
    printf("\n");
    
    /*
     ===================
     debug print counter
     ===================
     */

    int ctr_sum = 0;
    
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            printf("vtx_ctr %d %d %8d %6.3f %8d \n",i,j,prb->ele.ctr.vtx_int[i][j][0],(100e0*prb->ele.ctr.vtx_int[i][j][0])/(MSH_ELE_TOT),prb->ele.ctr.vtx_int[i][j][1]);
            
            ctr_sum += prb->ele.ctr.vtx_int[i][j][0];
        }
    }
    
    printf("ctr_sum     %8d\n",ctr_sum);
    
    printf("\n");
    
    /*
     ===================
     write files
     ===================
     */

    lst_write(&prb->lst1, "list1");
    lst_write(&prb->lst2, "list2");
    lst_write(&prb->lst3, "list3");
    lst_write(&prb->lst4, "list4");
    
    write_dof_raw(prb);

    /*
     ===================
     clean up
     ===================
     */
    
    free(prb);                  //problem
    
    printf("done\n");
    
    return 0;
}
