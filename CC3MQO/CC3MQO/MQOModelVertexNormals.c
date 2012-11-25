//
//  MQOModelVertexNormals.m
//  CC3DemoMashUp
//
//  Created by Yasuo Ashina on 12/07/16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MQOModel.h"


// 関数宣言
#ifdef __cplusplus
extern "C" {
#endif
    
    void mqoSnormal(glPOINT3f A, glPOINT3f B, glPOINT3f C, glPOINT3f *normal);
    
#ifdef __cplusplus
}
#endif


/*=========================================================================
 【関数】mqoCreateVertexNormal
 【用途】頂点法線の計算
 【引数】
 obj			オブジェクト情報
 
 【戻値】法線配列
 【仕様】４頂点の面は三角形に分割して計算
 戻り値は必ず呼び出し元で解放（free）すること！
 =========================================================================*/

// 全ての頂点（立方体だったら８個？）に対し、その点を共有する周りの面から、頂点法線の平均を出す？
glPOINT3f *mqoCreateVertexNormals( MQOObjectChunk *mqoObjectChunk )
{


    // 戻り値用：各頂点に対する、頂点法線を全部保存
    glPOINT3f *pglPOINT3fResult;
    
	pglPOINT3fResult = (glPOINT3f*)calloc( mqoObjectChunk->iVertexNumber, sizeof(glPOINT3f) );
	
    
    // 面の法線を頂点に足し込み
	for( int iLp = 0 ; iLp < mqoObjectChunk->iFaceNumber ; iLp++ )
    {
        
        MQOFace   *mqoFace    = mqoObjectChunk->pMQOFace;
        glPOINT3f *glPOINT3fV = mqoObjectChunk->pglPOINT3fVertex;        
        
        
        // 三角形の場合
		if ( mqoObjectChunk->pMQOFace[iLp].iVertexNumber == 3 )
        {
            // 面法線ベクトル
            glPOINT3f fnormal;            
            
			mqoSnormal( glPOINT3fV[ mqoFace[iLp].aiVertexNumber[0] ],
                        glPOINT3fV[ mqoFace[iLp].aiVertexNumber[1] ],
                        glPOINT3fV[ mqoFace[iLp].aiVertexNumber[2] ],
                        &fnormal );
            
			for( int iLp2 = 0; iLp2 < 3; iLp2++ )
            {
                // ３つの頂点にそれぞれ足す
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].x += fnormal.x;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].y += fnormal.y;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].z += fnormal.z;
			}
		}

        // これ、四角形のばあい、、、分割したら、不公平なんじゃない？？？？
        // 一回計算して、、同じ値を４つに代入するように変更
        
        
        // 四角形の場合、0,1,2 の三角形の面法線を計算してそれを４点に使う
		if ( mqoObjectChunk->pMQOFace[iLp].iVertexNumber == 4 )
        {
            
#if 1
            // 面法線ベクトル
            glPOINT3f fnormal;
            
			mqoSnormal( glPOINT3fV[ mqoFace[iLp].aiVertexNumber[0] ],
                       glPOINT3fV[ mqoFace[iLp].aiVertexNumber[1] ],
                       glPOINT3fV[ mqoFace[iLp].aiVertexNumber[2] ],
                       &fnormal );            
            
            
            for( int iLp2 = 0; iLp2 < 4 ; iLp2++ )
            {                
                pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].x += fnormal.x;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].y += fnormal.y;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].z += fnormal.z;
			}            

#else       
            // 面法線ベクトル
            glPOINT3f fnormal;
            
			mqoSnormal( glPOINT3fV[ mqoFace[iLp].aiVertexNumber[0] ],
                        glPOINT3fV[ mqoFace[iLp].aiVertexNumber[1] ],
                        glPOINT3fV[ mqoFace[iLp].aiVertexNumber[2] ],
                        &fnormal );
            
			for( int iLp2 = 0; iLp2 < 4 ; iLp2++ )
            {
				if ( iLp2 == 3 )
                {
                    continue;
				}
                    
                pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].x += fnormal.x;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].y += fnormal.y;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].z += fnormal.z;
			}
            
			mqoSnormal( glPOINT3fV[ mqoFace[iLp].aiVertexNumber[0] ],
                        glPOINT3fV[ mqoFace[iLp].aiVertexNumber[2] ],
                        glPOINT3fV[ mqoFace[iLp].aiVertexNumber[3] ],
                        &fnormal );
			
            for ( int iLp2 = 0 ; iLp2 < 4 ; iLp2++ )
            {
				if ( iLp2 == 1 )
                {
                    continue;
				}
                 
                pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].x += fnormal.x;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].y += fnormal.y;
				pglPOINT3fResult[ mqoFace[iLp].aiVertexNumber[iLp2] ].z += fnormal.z;
			}
#endif
            
            
            
		}
        

	}
    
    
	// 正規化
	for( int iLp = 0 ; iLp < mqoObjectChunk->iVertexNumber ; iLp++ )
    {
        
        // この判定いらないでしょ、、、どうせ０、０、０だったら、
        // 下の割り算されないし、、、０、０、０のままで問題なし
	//	if ( pglPOINT3fResult[iLp].x == 0 && pglPOINT3fResult[iLp].y == 0 && pglPOINT3fResult[iLp].z == 0 )
     //   {
			// 面に使われてない点
		//	continue;
		//}
	
        double dLength = sqrt( pglPOINT3fResult[iLp].x * pglPOINT3fResult[iLp].x +
                               pglPOINT3fResult[iLp].y * pglPOINT3fResult[iLp].y +
                               pglPOINT3fResult[iLp].z * pglPOINT3fResult[iLp].z );
		
        if ( dLength != 0 )
        {
			pglPOINT3fResult[iLp].x = pglPOINT3fResult[iLp].x / dLength;
			pglPOINT3fResult[iLp].y = pglPOINT3fResult[iLp].y / dLength;
			pglPOINT3fResult[iLp].z = pglPOINT3fResult[iLp].z / dLength;
		}
	}
    
	return pglPOINT3fResult;
}




