//
//  MQOModelReadObject.m
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
    
    
 
    int mqoReadVertex(  FILE *fp, glPOINT3f V[] );
    int	mqoReadBVertex( FILE *fp, glPOINT3f V[] );
    
    void mqoReadFace( FILE *fp, MQOFace F[] );
   
    
    
#ifdef __cplusplus
}
#endif



/*=========================================================================
 【関数】mqoReadObject
 【用途】オブジェクト情報の読み込み
 【引数】
 fp		ファイルポインタ
 obj		オブジェクト情報
 
 【戻値】なし
 【仕様】この関数で１個のオブジェクト情報が読み込まれる．
 =========================================================================*/

int mqoReadObject(FILE *fp, MQOObjectChunk *mqoObjectChunk )
{
	char buf[SIZE_STR];
    
    int res = 0;
	
    while (!feof(fp))
    {
		fgets(buf,SIZE_STR,fp);
	
        
		if( strstr( buf, "visible " ) )
        {
            // visible
            
			sscanf(buf," visible %d", &mqoObjectChunk->visible);
		}
		else if(strstr(buf,"shading "))
        {
            // shading
            
			sscanf(buf," shading %d", &mqoObjectChunk->iShading);
		}
		else if (strstr(buf,"facet "))
        {
            // facet
			
            sscanf( buf, " facet %f", &mqoObjectChunk->facet );
		}
		else if( strstr( buf, "vertex " ) )
        {
            // vertex
            
			sscanf(buf," vertex %d", &mqoObjectChunk->iVertexNumber );
            
			mqoObjectChunk->pglPOINT3fVertex = (glPOINT3f*) calloc( mqoObjectChunk->iVertexNumber, sizeof(glPOINT3f) );
			
            mqoReadVertex( fp, mqoObjectChunk->pglPOINT3fVertex );
		}
		else if (strstr(buf,"BVertex"))
        {
            // BVertex
            
			sscanf( buf, " BVertex %d", &mqoObjectChunk->iVertexNumber );
			mqoObjectChunk->pglPOINT3fVertex = (glPOINT3f*)calloc( mqoObjectChunk->iVertexNumber, sizeof(glPOINT3f) );
			mqoReadBVertex( fp,mqoObjectChunk->pglPOINT3fVertex );
		}
		else if( strstr ( buf, "face " ) )
        {
            // face
            
			sscanf( buf, " face %d", &mqoObjectChunk->iFaceNumber );
            
			mqoObjectChunk->pMQOFace = (MQOFace*)calloc( mqoObjectChunk->iFaceNumber, sizeof(MQOFace) );
			
            mqoReadFace( fp, mqoObjectChunk->pMQOFace );
		}
		else if (strstr(buf,"}"))
        { 
            res = 1;
            break;
        }
        
	}
    
    return res;
}

