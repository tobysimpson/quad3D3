//
//  path.c
//  quad3D3
//
//  Created by Toby Simpson on 27.02.19.
//  Copyright © 2019 Toby Simpson. All rights reserved.
//

#include "master.h"

//functions to calculate path-connectedness

//test if both int and ext verts are path connected
int ele_pth_test(struct problem *prb)
{
    int pth_int_num = 0;                                                        //length of internal path
    
    //internal
    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)                                    //loop all verts of ele
    {
        if(prb->ele.vtx_sdf[vtx_idx]<0)                                         //start from internal vertex
        {
            pth_int_num = pth_calc(prb, vtx_idx);                               //get length path
            
            break;
        }
    }
//    return pth_int_num == prb->ele.vtx_int;                                 //internal verts are path connected
    
    //external
    int pth_ext_num = 0;                                                        //length of external path

    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)                                    //loop all verts of ele
    {
        if(!(prb->ele.vtx_sdf[vtx_idx]<0))                                      //start from external vertex
        {
            pth_ext_num = pth_calc(prb, vtx_idx);                               //get length path

            break;
        }
    }
////    printf("ele_pth_test    %d %d | %d %d | %d \n",prb->ele.vtx_int_tot, 8-prb->ele.vtx_int_tot, pth_int_num, pth_ext_num, (pth_int_num + pth_ext_num) == 8);
//
    return (pth_int_num + pth_ext_num) == 8;                                    //are all verts accounted for?
}


//start a path recursion from a given vertex - return its length
int pth_calc(struct problem *prb, int vtx_idx)
{
    int pth_vtx[8];                                                             //path verts
    
    int pth_num = 1;                                                            //num verts in path
    pth_vtx[0] = vtx_idx;                                                       //add first vertex
    
    pth_rec(prb, vtx_idx, pth_vtx, &pth_num);                                   //call recursion
    
//    printf("pth_num %d \n",pth_num);
    
    return pth_num;
}

//recursion for path connectedness
void pth_rec(struct problem *prb, int vtx_idx1, int *pth_vtx, int *pth_num)
{
    for(int dim_idx=0; dim_idx<3; dim_idx++)                                    //loop dims
    {
        int vtx_idx2 = vtx_idx1;                                                //init neighbour
        
        vtx_idx2 ^= 1 << dim_idx;                                               //flip bit for edge traversal
        
//        printf("pth_rec %d %d %d %d\n",pth_num,dim_idx,vtx_idx1,vtx_idx2);
        
        int bflag = 0;                                                          //found flag
        
        //check list
        for(int i=0; i<*pth_num; i++)                                           //loop path vertex list
        {
            if(pth_vtx[i]==vtx_idx2)
            {
                bflag = 1;                                                      //flag if exists
                
                break;
            }
        }
        
        if(!bflag)                                                              //not on list
        {
            if((prb->ele.vtx_sdf[vtx_idx1]<0)==(prb->ele.vtx_sdf[vtx_idx2]<0))  //test same sign of sdf
            {
                pth_vtx[*pth_num] = vtx_idx2;                                   //update list
                
                *pth_num +=1;                                                   //increment list length
                
                pth_rec(prb, vtx_idx2, pth_vtx, pth_num);                       //call recursion
            }
        }
    }
    
    return;
}

