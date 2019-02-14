//
//  main.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"


int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    /*
     ===================
     initialise
     ===================
     */
    
    struct problem *prb = malloc(sizeof(struct problem));       //allocate memory
    
    prb->vlm = 0;                                               //init volume
    
    scm_init(prb);                                              //init scheme
    msh_init(prb);                                              //init mesh
    
    lst_init(&prb->lst1);                                       //init list
    lst_init(&prb->lst2);                                       //init list
    lst_init(&prb->lst3);                                       //init list
    lst_init(&prb->lst4);                                       //init list
    
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
    
    prb->vlm = prb->vlm*prb->msh.ele_vlm;                       //adjust for ele size
    
    printf("prb_vlm         %f %f\n",prb->vlm, M_PI*4e0/3e0);
    
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
