//
//  scheme.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"

//gauss legendre quadrature schemes - define on [-1,+1] then shift to [0,1]
void scm_init(struct problem *prb)
{
    /*
     ========================
     init scheme
     ========================
     */
    
    switch (SCM_NP)
    {
        case 1:
            
            prb->scm.np = 1;
            
            prb->scm.pp[0] = 0;                                             //midpoint
            prb->scm.ww[0] = 2;                                             //weight on [-1,+1]
            
            break;
            
        case 2:
            
            prb->scm.np = 2;
            
            prb->scm.pp[0] = -1/sqrt(3e0);                                  //points
            prb->scm.pp[1] = +1/sqrt(3e0);
            
            prb->scm.ww[0] = 1;                                             //weights
            prb->scm.ww[1] = 1;
        
            break;
        
        case 3:
            
            prb->scm.np = 3;
        
            prb->scm.pp[0] = -sqrt(3e0/5e0);                                //points on [-1,+1]
            prb->scm.pp[1] = 0e0;
            prb->scm.pp[2] = +sqrt(3e0/5e0);
            
//            prb->scm.pp[0] = -1;                                            //points on [-1,+1] - just for plots
//            prb->scm.pp[1] = +0;
//            prb->scm.pp[2] = +1;
            
            prb->scm.ww[0] = 5e0/9e0;                                       //weights for [-1,+1]
            prb->scm.ww[1] = 8e0/9e0;
            prb->scm.ww[2] = 5e0/9e0;
            
            break;
            
        case 4:
        
            prb->scm.np = 4;
            
            prb->scm.pp[0] = -sqrt((3e0/7e0)+((2e0/7e0)*sqrt(6e0/5e0)));    //points on [-1,+1]
            prb->scm.pp[1] = -sqrt((3e0/7e0)-((2e0/7e0)*sqrt(6e0/5e0)));
            prb->scm.pp[2] = +sqrt((3e0/7e0)-((2e0/7e0)*sqrt(6e0/5e0)));
            prb->scm.pp[3] = +sqrt((3e0/7e0)+((2e0/7e0)*sqrt(6e0/5e0)));

            prb->scm.ww[0] = (18e0-sqrt(30e0))/36e0;                        //weights for [-1,+1]
            prb->scm.ww[1] = (18e0+sqrt(30e0))/36e0;
            prb->scm.ww[2] = (18e0+sqrt(30e0))/36e0;
            prb->scm.ww[3] = (18e0-sqrt(30e0))/36e0;
            
            break;
            
        case 5:
            
            prb->scm.np = 5;
            
            prb->scm.pp[0] = -sqrt(5+(2*sqrt(10e0/7e0)))/3e0;               //points on [-1,+1]
            prb->scm.pp[1] = -sqrt(5-(2*sqrt(10e0/7e0)))/3e0;
            prb->scm.pp[2] = 0e0;
            prb->scm.pp[3] = +sqrt(5-(2*sqrt(10e0/7e0)))/3e0;
            prb->scm.pp[4] = +sqrt(5+(2*sqrt(10e0/7e0)))/3e0;
            
            prb->scm.pp[0] = -1.0;                                            //points on [-1,+1] - just for plots
            prb->scm.pp[1] = -0.5;
            prb->scm.pp[2] = +0.0;
            prb->scm.pp[3] = +0.5;
            prb->scm.pp[4] = +1.0;
        
            prb->scm.ww[0] = (322e0-13e0*sqrt(70e0))/900e0;                 //weights for [-1,+1]
            prb->scm.ww[1] = (322e0+13e0*sqrt(70e0))/900e0;
            prb->scm.ww[2] = 128e0/225e0;
            prb->scm.ww[3] = (322e0+13e0*sqrt(70e0))/900e0;
            prb->scm.ww[4] = (322e0-13e0*sqrt(70e0))/900e0;
        
            break;

    }
    
    /*
     ========================
     adjust interval
     ========================
     */
    
    for(int i=0; i<prb->scm.np; i++)
    {
        prb->scm.pp[i] = 0.5*(1e0+prb->scm.pp[i]);                          //adjust for interval [0,1]
        prb->scm.ww[i] = 0.5*prb->scm.ww[i];
        
        printf("scm_init        %d %+f %f\n",i,prb->scm.pp[i],prb->scm.ww[i]);
    }
    
    return;
}
