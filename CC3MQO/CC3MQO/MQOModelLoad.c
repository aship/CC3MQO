//
//  MQOModelLoad.m
//  CC3DemoMashUp
//
//  Created by Yasuo Ashina on 12/07/15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MQOModel.h"



// 関数宣言
#ifdef __cplusplus
extern "C" {
#endif
    
    
    
    void mqoReadScene(FILE *fp, MQOModel *model, double scale);
    
    int mqoReadMaterial(FILE *fp, MQOMaterial M[]);
    int mqoReadObject(FILE *fp, MQOObjectChunk *obj);
    
    
    glPOINT3f *mqoCreateVertexNormals(MQOObjectChunk *obj);
 
    
    void mqoMakePolygon( MQOModel *mqoModel,
                         MQOObjectChunk *obj_chunk,
                         glPOINT3f N[], 
                         double scale, 
                         unsigned char alpha);
    
     
    
#ifdef __cplusplus
}
#endif







/*=========================================================================
 【関数】mqoLoadFile
 【用途】メタセコイアファイル(*.mqo)からデータを読み込む
 【引数】
 mqoobj		MQOオブジェクト
 filename	ファイルのパス
 scale		拡大率
 alpha		アルファ
 
 【戻値】成功：1 ／ 失敗：0
 =========================================================================*/

int mqoLoadFile(MQOModel *mqoModel, MQOObjectChunk *mqoObjectChunk, const char *filename )
{
    
    // ファイル読み込み
    FILE *fp = fopen(filename,"rb");
    
	if( fp == NULL )
    {
        return 0;
    }
    
    int iResult = 1;
    
    // オブジェクト数
  int	iObjectNumber = 0;			
    
	while (!feof(fp))
    {
        
        // 文字列読み込みバッファ
        char buf[SIZE_STR];
        
		fgets(buf, SIZE_STR, fp);
        
		// Scene
		if (strstr(buf, "Scene"))
        {
            mqoReadScene(fp, mqoModel, 1.0 );
		}
        
		// Material
		if (strstr(buf, "Material"))
        {
            // マテリアル数を読み取り
			sscanf(buf, "Material %d", &mqoModel->iMaterialNumber );
            
			mqoModel->pMQOMaterial = (MQOMaterial *) calloc(mqoModel->iMaterialNumber , sizeof(MQOMaterial));
            
            if (mqoReadMaterial(fp, mqoModel->pMQOMaterial ) != mqoModel->iMaterialNumber )
            {
                iResult = 0;
                break;
            }
		}
        
		// Object
		if (strstr(buf,"Object"))
        {
            if (iObjectNumber < MAX_OBJECT)
            {
                sscanf( buf,"Object %s", mqoObjectChunk[iObjectNumber].objname );
                
                if( mqoReadObject( fp, &mqoObjectChunk[iObjectNumber] ) != 1 )
                {
                    iResult = 0;
                    break;
                }
            }
            else 
            {
                printf("MQOファイル読み込み：　最大オブジェクト数を超えました[%d]\n", iObjectNumber);
            }
            
            iObjectNumber++;
		}
	}
	
    
    // オブジェクト数保存
    mqoModel->iObjectNumber = iObjectNumber;    

    
    fclose(fp);    
    
	return iResult;
}