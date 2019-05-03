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
    
//    prb->msh.msh_ele_dim = msh_ele_dim;
    
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
    
    clock_t tic = clock();                                      //start timer
    
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
    
    clock_t toc = clock();                                      //stop timer
    
    /*
     ===================
     debug print results
     ===================
     */
    
    float vlm_num = prb->vlm[2]*prb->msh.ele_vlm;
    float vlm_ana = (M_PI*0.25*0.25)*(2*M_PI*0.75);
    float tmr_sec = (float)(toc - tic) * 1000.0 / CLOCKS_PER_SEC;
    
//    printf("prb_vlm         %4d,%d,%f,%f; \n",MSH_ELE_DIM, MSH_ELE_TOT , prb->vlm[2]*prb->msh.ele_vlm, (1e0/8e0)*M_PI*4e0/3e0);   //sphere
//    printf("prb_vlm         %6d,%8d,%e,%e,%e; \n",MSH_ELE_DIM, MSH_ELE_TOT , vlm_num, vlm_ana, (vlm_num/vlm_ana)-1);                 //torus (pi*r^2)*(2*pi*R)
    
    
    
    printf("%6d,%12d,%e,%e,%e,%e\n",MSH_ELE_DIM, MSH_ELE_TOT , vlm_num, vlm_ana, (vlm_num/vlm_ana)-1e0,tmr_sec);                 //torus (pi*r^2)*(2*pi*R)
    
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
    
    free(prb->lst1.pp);         //point lists
    free(prb->lst2.pp);
    free(prb->lst3.pp);
    free(prb->lst4.pp);
    
    free(prb);                  //problem
    
    
    
    printf("done\n");
    
    return 0;
}


////loop main
//int main(int argc, const char * argv[])
//{
//    int msh_ele_dd[4] = {1,2,3,4};
//
//    for(int i=0; i<4; i++)
//    {
//        calc_vlm(pow(2, msh_ele_dd[i]));
//    }
//
//    return 0;
//}

