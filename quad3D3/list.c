//
//  list.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//init point list
void lst_init(struct point_list *lst)
{
    lst->pp_num = 0;
    lst->pp_alloc = 128;
    lst->pp = malloc(lst->pp_alloc*sizeof(struct point));
}


//add point to list
void lst_add_pt(struct point_list *lst, const float x[3], const float v)
{
    struct point pt = {{x[2],x[1],x[0]},v};                                         //init point
    
    if(lst->pp_num == (lst->pp_alloc-1))                                            //needs to be resized
    {
        lst->pp_alloc = 2*lst->pp_alloc;                                            //increase size
        
        lst->pp = realloc(lst->pp, lst->pp_alloc*sizeof(struct point));             //reallocate larger array
    }
    lst->pp[lst->pp_num] = pt;                                                      //assign
    
    lst->pp_num += 1;                                                               //increment
    
    return;
}

//add element verts to list
void lst_add_ele(struct point_list *lst, struct problem *prb)
{
    for(int vtx_idx=0; vtx_idx<8; vtx_idx++)
    {
        lst_add_pt(lst, prb->ele.vtx_glb[vtx_idx], prb->ele.vtx_sdf[vtx_idx]<0);    //add to list
    }
    return;
}


//write for paraview
void lst_write(struct point_list *lst, char file_name[100])
{
    FILE *file1;
    
    char file_path[150];
    
    sprintf(file_path,"%s/%s.txt",FILE_ROOT,file_name);
    
    file1 = fopen(file_path,"w");
    
    fprintf(file1, "x,y,z,v\n");                                                                                //write header
    
    for(int k=0; k<lst->pp_num; k++)
    {
        fprintf(file1, "%+e,%+e,%+e,%+e\n",lst->pp[k].x[0],lst->pp[k].x[1],lst->pp[k].x[2],lst->pp[k].v);       //write data
    }
    fclose(file1);
}
