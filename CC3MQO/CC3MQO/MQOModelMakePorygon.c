//
//  MQOModelPorygon.m
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
    
    
    void mqoMakeVertexArray(MQOMesh *mesh, 
                            MQOFace face_arr[], 
                            int face_num, 
                            glPOINT3f vertex_arr[],
                            glPOINT3f normals_arr[], 
                            double facet, 
                            double scale, 
                            unsigned char alpha);
        
        void mqoSnormal(glPOINT3f A, glPOINT3f B, glPOINT3f C, glPOINT3f *normal);
        
    
#ifdef __cplusplus
}
#endif



// ポリゴンの生成
// aglPOINT3fVertexNormals 各頂点の頂点法線
// スムージング角によって面法線か頂点法線を選ぶ
void mqoMakePolygon( MQOObject *pMQOObject,
                     MQOObjectChunk *mqoObjectChunk,
					 glPOINT3f *pglPOINT3fVertexNormals )
{
        
    // Object名を設定
	strcpy( pMQOObject->objname, mqoObjectChunk->objname );
	
    pMQOObject->isVisible = mqoObjectChunk->visible;
	
    pMQOObject->iShadingFlat = (mqoObjectChunk->iShading == 0);
	

	
    // 面の数
    int	iFaceNumber = mqoObjectChunk->iFaceNumber;
    
    // メッシュの数　＝　面の数とする
    pMQOObject->mesh_num = iFaceNumber;

    // メッシュの数だけ、メモリを確保
	pMQOObject->mesh_arr = (MQOMesh*)malloc( sizeof(MQOMesh) * pMQOObject->mesh_num );
    
	memset( pMQOObject->mesh_arr, 0, sizeof(MQOMesh) * pMQOObject->mesh_num );
    
    
    // メッシュを作ります
    for( int iLp = 0; iLp < pMQOObject->mesh_num ; iLp++ )
    {    
    
        // このメッシュに書き込む
        MQOMesh *pMQOMesh = &pMQOObject->mesh_arr[ iLp ];    
    
        // 面情報、、読み出し専用
        MQOFace mqoFace = mqoObjectChunk->pMQOFace[ iLp ];
                                                   
        
        
        if ( mqoFace.iVertexNumber == 3 )
        {
            // 面が三角ポリゴン
            //省略
            // location(3つの頂点ベクトル), normal(3頂点ごとの法線), テクスチャ(u,v 3組)をセット
            
            // まず領域確保            
            // x, y, z が3頂点分
            pMQOMesh->locations          = (float*)calloc( 3 * 3, sizeof(float) );
            pMQOMesh->normals            = (float*)calloc( 3 * 3, sizeof(float) );
            
            // u, v が3頂点分
            pMQOMesh->texture_cordinates = (float*)calloc( 2 * 3, sizeof(float) );            
            

            // 頂点ベクトル
            pMQOMesh->locations[0] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ].x;
            pMQOMesh->locations[1] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ].y;
            pMQOMesh->locations[2] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ].z;
            pMQOMesh->locations[3] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[1] ].x;
            pMQOMesh->locations[4] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[1] ].y;
            pMQOMesh->locations[5] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[1] ].z;
            pMQOMesh->locations[6] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ].x;
            pMQOMesh->locations[7] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ].y;
            pMQOMesh->locations[8] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ].z;            

            
            // 面法線ベクトル
            glPOINT3f glPOINT3fPlaneNormal;            
            
            

            // あし改造、、、この関数は時計回りで法線を出している
            // 面法線ベクトルを計算
            mqoSnormal( mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ],
                       mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[1] ],
                       mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ],
                       &glPOINT3fPlaneNormal );              
            

#if 1
            // ここで、面法線を使うか、頂点法線を使うかをスムージング各で判断しなければ、、、
            // 面法線と頂点法線の角度
            // 各頂点ごとに比較する！
            for( int iLp = 0 ; iLp < mqoFace.iVertexNumber ; iLp++ )
            {
                // 頂点法線
                glPOINT3f glPOINT3fVertexNormal = pglPOINT3fVertexNormals[ mqoFace.aiVertexNumber[iLp] ];
                
                
                double dSRadian;
                dSRadian = acos(glPOINT3fPlaneNormal.x * glPOINT3fVertexNormal.x +
                          glPOINT3fPlaneNormal.y * glPOINT3fVertexNormal.y +
                          glPOINT3fPlaneNormal.z * glPOINT3fVertexNormal.z );                
                
                // 結果がラジアンで来るので、度になおす
                double dSAngle = dSRadian / 2 / M_PI * 360; 
                
                // facet = スムージング角度    
                
                // スムージングの角度を最大45°としたときの面の表面のとある座標位置での
                // 面法線をN、頂点法線をVNとして考えてみます。
                
                // c1 = N.x * VN.x + N.y * VN.y + N.z * VN.z;
                // c2 = cos(45 * 3.1415926535 / 180.0);
                // 「c1 >= c2」のときは頂点法線を採用します。「c1 < c2」のときは面法線を採用します。
                // スムージング角度が大きいほどスムーズになり、スムージング角度が小さいとフラットなシェーディングになります。 
                
                if ( mqoObjectChunk->facet < dSAngle )
                {
                    // スムージング角　＜（頂点法線と面法線の角度）のときは面法線を法線とする
                    // つまり、頂点法線と面法制がかけ離れているときは、面法線を使う、、、
                    // これは、サイコロのときの扱いとしてはいいはず。。。。
                    
                    // メッシュにその法線をぶちこむ(3頂点分
                    pMQOMesh->normals[iLp    ] = glPOINT3fPlaneNormal.x;
                    pMQOMesh->normals[iLp + 1] = glPOINT3fPlaneNormal.y;
                    pMQOMesh->normals[iLp + 2] = glPOINT3fPlaneNormal.z; 
                }
                else
                {
                    // そうでないときは、頂点法線と面法線の方向が近いときは
                    // 頂点法線を法線にする
                    
                    if( iLp == 2 )
                    {
                        pMQOMesh->normals[0] = glPOINT3fVertexNormal.x;
                        pMQOMesh->normals[1] = glPOINT3fVertexNormal.y;
                        pMQOMesh->normals[2] = glPOINT3fVertexNormal.z;                        
                        
                    }
                    else if( iLp == 1 )
                    {
                        pMQOMesh->normals[3] = glPOINT3fVertexNormal.x;
                        pMQOMesh->normals[4] = glPOINT3fVertexNormal.y;
                        pMQOMesh->normals[5] = glPOINT3fVertexNormal.z;                          
                    }
                    else if( iLp == 0 )
                    {
                        pMQOMesh->normals[6] = glPOINT3fVertexNormal.x;
                        pMQOMesh->normals[7] = glPOINT3fVertexNormal.y;
                        pMQOMesh->normals[8] = glPOINT3fVertexNormal.z;                          
                    }                    
                    
                    
                   
                    
                }                
            }
            
#else
            
            // メッシュにその法線をぶちこむ(3頂点分
            pMQOMesh->normals[0] = glPOINT3fPlaneNormal.x;
            pMQOMesh->normals[1] = glPOINT3fPlaneNormal.y;
            pMQOMesh->normals[2] = glPOINT3fPlaneNormal.z;            
            pMQOMesh->normals[3] = glPOINT3fPlaneNormal.x;
            pMQOMesh->normals[4] = glPOINT3fPlaneNormal.y;
            pMQOMesh->normals[5] = glPOINT3fPlaneNormal.z; 
            pMQOMesh->normals[6] = glPOINT3fPlaneNormal.x;
            pMQOMesh->normals[7] = glPOINT3fPlaneNormal.y;
            pMQOMesh->normals[8] = glPOINT3fPlaneNormal.z;            
            

#endif       
            
            // あし改造
            // UV
            pMQOMesh->texture_cordinates[0] = mqoFace.uv[2].x;
            pMQOMesh->texture_cordinates[1] = mqoFace.uv[2].y;            
            pMQOMesh->texture_cordinates[2] = mqoFace.uv[1].x;
            pMQOMesh->texture_cordinates[3] = mqoFace.uv[1].y;                       
            pMQOMesh->texture_cordinates[4] = mqoFace.uv[0].x;
            pMQOMesh->texture_cordinates[5] = mqoFace.uv[0].y; 

            
            // その他の情報
            
            // 頂点数
            pMQOMesh->vertex_num = mqoFace.iVertexNumber;
            
            // マテリアル番号
            pMQOMesh->iMaterialIndex = mqoFace.iMaterialIndex;
            
            
        }
        else if ( mqoFace.iVertexNumber == 4 )  
        {
            // 面が四角ポリゴン
            
            // location(4つの頂点ベクトル), normal(4頂点ごとの法線), テクスチャ(u,v 4組)をセット
            
            // まず領域確保
            
            // x, y, z が４頂点分
            pMQOMesh->locations          = (float*)calloc( 3 * 4, sizeof(float) );
            pMQOMesh->normals            = (float*)calloc( 3 * 4, sizeof(float) );
            
            // u, v が４頂点分
            pMQOMesh->texture_cordinates = (float*)calloc( 2 * 4, sizeof(float) );
            

            // 頂点ベクトル
            pMQOMesh->locations[0]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[3] ].x;
            pMQOMesh->locations[1]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[3] ].y;
            pMQOMesh->locations[2]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[3] ].z;
            pMQOMesh->locations[3]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ].x;
            pMQOMesh->locations[4]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ].y;
            pMQOMesh->locations[5]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ].z;
            pMQOMesh->locations[6]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ].x;
            pMQOMesh->locations[7]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ].y;
            pMQOMesh->locations[8]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ].z;
            pMQOMesh->locations[9]  = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[1] ].x;
            pMQOMesh->locations[10] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[1] ].y;
            pMQOMesh->locations[11] = mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[1] ].z;            
            

            
            // 面法線ベクトル
            glPOINT3f glPOINT3fPlaneNormal;            
            
            

            // あし改造
            // 面法線ベクトルを計算（４角だから、適当な３点で計算）
            // この関数は時計回りの法線を出す
            mqoSnormal( mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[2] ],
                        mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[3] ],
                        mqoObjectChunk->pglPOINT3fVertex[ mqoFace.aiVertexNumber[0] ],
                        &glPOINT3fPlaneNormal );              
                         
            // ここで、面法線を使うか、頂点法線を使うかをスムージング各で判断しなければ、、、
#if 1
            
            // ここで、面法線を使うか、頂点法線を使うかをスムージング各で判断しなければ、、、
            // 面法線と頂点法線の角度
            // 各頂点ごとに比較する！
            for( int iLp = 0 ; iLp < mqoFace.iVertexNumber ; iLp++ )
            {
                // 頂点法線
                glPOINT3f glPOINT3fVertexNormal = pglPOINT3fVertexNormals[ mqoFace.aiVertexNumber[iLp] ];
                
                
                double dSRadian;
                dSRadian = acos(glPOINT3fPlaneNormal.x * glPOINT3fVertexNormal.x +
                          glPOINT3fPlaneNormal.y * glPOINT3fVertexNormal.y +
                          glPOINT3fPlaneNormal.z * glPOINT3fVertexNormal.z );                
                
                // 結果がラジアンで来るので、度になおす
                double dSAngle = dSRadian / 2 / M_PI * 360;                
                
                // facet = スムージング角度(metaseqoia では ０〜１８０の値）    
                
                // スムージングの角度を最大45°としたときの面の表面のとある座標位置での
                // 面法線をN、頂点法線をVNとして考えてみます。
                
                // c1 = N.x * VN.x + N.y * VN.y + N.z * VN.z;
                // c2 = cos(45 * 3.1415926535 / 180.0);
                // 「c1 >= c2」のときは頂点法線を採用します。「c1 < c2」のときは面法線を採用します。
                // スムージング角度が大きいほどスムーズになり、スムージング角度が小さいとフラットなシェーディングになります。 
                
             //   printf( "##############facet < dSAngle : %f  <  %f  \n",  mqoObjectChunk->facet, dSAngle );
                
                
                if ( mqoObjectChunk->facet < dSAngle )
                {
                    // スムージング角　＜（頂点法線と面法線の角度）のときは面法線を法線とする
                    // つまり、頂点法線と面法制がかけ離れているときは、面法線を使う、、、
                    // これは、サイコロのときの扱いとしてはいいはず。。。。
                    
                    // メッシュにその法線をぶちこむ(4頂点分
                    pMQOMesh->normals[iLp    ] = glPOINT3fPlaneNormal.x;
                    pMQOMesh->normals[iLp + 1] = glPOINT3fPlaneNormal.y;
                    pMQOMesh->normals[iLp + 2] = glPOINT3fPlaneNormal.z;     
                }
                else
                {
                    // そうでないときは、頂点法線と面法線の方向が近いときは
                    // 頂点法線を法線にする
                    
                   
                    
                    if( iLp == 3 )
                    {
                        pMQOMesh->normals[0] = glPOINT3fVertexNormal.x;
                        pMQOMesh->normals[1] = glPOINT3fVertexNormal.y;
                        pMQOMesh->normals[2] = glPOINT3fVertexNormal.z;                        
                        
                    }
                    else if( iLp == 2 )
                    {
                        pMQOMesh->normals[3] = glPOINT3fVertexNormal.x;
                        pMQOMesh->normals[4] = glPOINT3fVertexNormal.y;
                        pMQOMesh->normals[5] = glPOINT3fVertexNormal.z;                          
                    }
                    else if( iLp == 0 )
                    {
                        pMQOMesh->normals[6] = glPOINT3fVertexNormal.x;
                        pMQOMesh->normals[7] = glPOINT3fVertexNormal.y;
                        pMQOMesh->normals[8] = glPOINT3fVertexNormal.z;                          
                    }
                    else if( iLp == 1 )
                    {
                        pMQOMesh->normals[9]  = glPOINT3fVertexNormal.x;
                        pMQOMesh->normals[10] = glPOINT3fVertexNormal.y;
                        pMQOMesh->normals[11] = glPOINT3fVertexNormal.z;                          
                    }  
                    
                    
                }                
            }

            
#else
            // メッシュにその法線をぶちこむ(4頂点分
            pMQOMesh->normals[0] = glPOINT3fPlaneNormal.x;
            pMQOMesh->normals[1] = glPOINT3fPlaneNormal.y;
            pMQOMesh->normals[2] = glPOINT3fPlaneNormal.z;            
            pMQOMesh->normals[3] = glPOINT3fPlaneNormal.x;
            pMQOMesh->normals[4] = glPOINT3fPlaneNormal.y;
            pMQOMesh->normals[5] = glPOINT3fPlaneNormal.z; 
            pMQOMesh->normals[6] = glPOINT3fPlaneNormal.x;
            pMQOMesh->normals[7] = glPOINT3fPlaneNormal.y;
            pMQOMesh->normals[8] = glPOINT3fPlaneNormal.z; 
            pMQOMesh->normals[9] = glPOINT3fPlaneNormal.x;
            pMQOMesh->normals[10] = glPOINT3fPlaneNormal.y;
            pMQOMesh->normals[11] = glPOINT3fPlaneNormal.z; 
#endif
        

            // UV
            pMQOMesh->texture_cordinates[0] = mqoFace.uv[3].x;
            pMQOMesh->texture_cordinates[1] = mqoFace.uv[3].y;            
            pMQOMesh->texture_cordinates[2] = mqoFace.uv[2].x;
            pMQOMesh->texture_cordinates[3] = mqoFace.uv[2].y;            
            pMQOMesh->texture_cordinates[4] = mqoFace.uv[0].x;
            pMQOMesh->texture_cordinates[5] = mqoFace.uv[0].y;            
            pMQOMesh->texture_cordinates[6] = mqoFace.uv[1].x;
            pMQOMesh->texture_cordinates[7] = mqoFace.uv[1].y;              

            
            // その他の情報
            
            // 頂点数
            pMQOMesh->vertex_num = mqoFace.iVertexNumber;
            
            // マテリアル番号
            pMQOMesh->iMaterialIndex = mqoFace.iMaterialIndex;
            

        }
        
    }
}

