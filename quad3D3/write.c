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

