//
//  MQOModelReadScene.m
//  CC3DemoMashUp
//
//  Created by Yasuo Ashina on 12/07/17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MQOModel.h"


#if 1

void mqoReadScene(FILE *fp, MQOModel *model, double scale)
{
    char buf[SIZE_STR];
    
    glPOINT3f pos;
	
    while (!feof(fp))
    {
		fgets(buf, SIZE_STR, fp);	// 行読み込み        
		// pos
		if (strstr(buf,"pos "))
        {
			sscanf(buf," pos %f %f %f", &pos.x, &pos.y, &pos.z);
                        
      //      model->camera_pos.x = scale * pos.x;
      //      model->camera_pos.y = scale * pos.y;
      //      model->camera_pos.z = scale * pos.z;
		}        
		// lookat
        else if (strstr(buf,"lookat "))
        {
			sscanf(buf," lookat %f %f %f", &pos.x, &pos.y, &pos.z);
            
     //       model->camera_lookat.x = scale * pos.x;
     //       model->camera_lookat.y = scale * pos.y;
     //       model->camera_lookat.z = scale * pos.z;
		}
		// head
		else if (strstr(buf,"head "))
        {
	//		sscanf(buf," head %f", &model->camera_head);
		}
		// pich
		else if (strstr(buf,"pich "))
        {
	//		sscanf(buf," pich %f", &model->camera_pich);
		}
		// head
		else if (strstr(buf,"head "))
        {
	//		sscanf(buf," head %f", &model->camera_head);
		}
        // ortho
		else if (strstr(buf,"ortho "))
        {
	//		sscanf(buf," ortho %d", &model->ortho);
		}
        // zoom2
		else if (strstr(buf,"zoom2 "))
        {
	//		sscanf(buf," zoom2 %f", &model->camera_zoom2);
		}
        // amb
		else if (strstr(buf,"amb "))
        {
	//		sscanf(buf," amb %f %f %f", &model->amb[0], &model->amb[1], &model->amb[2]);
		}
        else if (strstr(buf,"}"))
        {
            break;
        }
        
	}
    
}

#endif






