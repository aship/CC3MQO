//
//  MQOModelReadVertex.m
//  CC3DemoMashUp
//
//  Created by Yasuo Ashina on 12/07/16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MQOModel.h"




/*=========================================================================
 【関数】mqoReadVertex
 【用途】頂点情報の読み込み
 【引数】
 fp		現在オープンしているメタセコイアファイルのファイルポインタ
 V		頂点を格納する配列
 
 【戻値】なし
 【仕様】mqoReadObject()のサブ関数
 =========================================================================*/

int mqoReadVertex(FILE *fp, glPOINT3f glPOINT3fVertex[])
{
	char buf[SIZE_STR];
    
	int i = 0;
    
    int iResult = 0;
	
    while (1)
    {
		fgets(buf,SIZE_STR,fp);
        
		if (strstr(buf,"}"))
        {
            iResult = 1;
            break;
        }
        
		sscanf( buf, "%f %f %f", &glPOINT3fVertex[i].x, 
                                 &glPOINT3fVertex[i].y,
                                 &glPOINT3fVertex[i].z );
        
		
        i++;
	}
    
    return iResult;

}


/*=========================================================================
 【関数】mqoReadBVertex
 【用途】バイナリ形式の頂点情報を読み込む
 【引数】
 fp		現在オープンしているメタセコイアファイルのファイルポインタ
 V		頂点を格納する配列
 
 【戻値】頂点数
 【仕様】mqoReadObject()のサブ関数
 =========================================================================*/

int mqoReadBVertex(FILE *fp, glPOINT3f V[])
{
	int n_vertex,i;
	float *wf;
	int size;
	char cw[256];
	char *pStr;
    
	fgets(cw,sizeof(cw),fp);
    
	if ( (pStr = strstr(cw,"Vector")) != NULL )
    {
		sscanf(pStr,"Vector %d [%d]",&n_vertex,&size);	// 頂点数、データサイズを読み込む
	}
	else
    {
		return -1;
	}
    
	//MQOファイルのバイナリ頂点データはintel形式（リトルエディアン）
	wf = (float *)malloc(size);
	
    fread(wf,size,1,fp);
	
    for ( i = 0; i < n_vertex; i++ )
    {
		V[i].x = wf[i*3+0];
		V[i].y = wf[i*3+1];
		V[i].z = wf[i*3+2];                
    }

	free(wf);
    
	// "}"まで読み飛ばし
	{
		char buf[SIZE_STR];
		while (1) {
			fgets(buf,SIZE_STR,fp);
			if (strstr(buf,"}")) break;
		}
	}
    
	return n_vertex;
}

