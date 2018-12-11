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
    
    struct problem *prb = malloc(sizeof(struct problem));           //allocate memory
    
    scm_init(prb);                                                  //init scheme
    msh_init(prb);                                                  //init mesh
    
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

    
    printf("done\n");
    
    return 0;
}
