//
//  list.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"


//write dof
void write_dof_raw(struct problem *prb)
{
    FILE *file1;
    
    char file_name[100];
    
    sprintf(file_name,"%s/%s",FILE_ROOT,"dof1.raw");
    
    file1 = fopen(file_name,"w");
    
    fwrite(&prb->geo.dof_sdf,sizeof(float), MSH_DOF_TOT, file1);     //write binary data

    fclose(file1);
}



////write dof
//void write_dof_raw(struct problem *prb)
//{
//    FILE *file1;
//
//    char file_name[100];
//
//    sprintf(file_name,"%s/%s",FILE_ROOT,"dof1.raw");
//
//    file1 = fopen(file_name,"w");
//
//
//    for(int k=0; k<(MSH_ELE_DIM_2+1); k++)                                      //loop space
//    {
//        for(int j=0; j<(MSH_ELE_DIM_1+1); j++)
//        {
//            for(int i=0; i<(MSH_ELE_DIM_0+1); i++)
//            {
////                float x[3];
////
////                x[0] = prb->msh.xmin[0] + i*prb->msh.ele_h[0];                //gen coords
////                x[1] = prb->msh.xmin[1] + j*prb->msh.ele_h[1];
////                x[2] = prb->msh.xmin[2] + k*prb->msh.ele_h[2];
//
//                fwrite(&prb->geo.dof_sdf[i][j][k],sizeof(float), 1, file1);     //write binary data
//            }
//        }
//    }
//    fclose(file1);
//}

