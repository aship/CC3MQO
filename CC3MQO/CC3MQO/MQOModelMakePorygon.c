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
//    pMQOObject->mesh_num = iFaceNumber;
    // ここ、１つのメッシュにまとめます！
    pMQOObject->mesh_num = 1;
        
    // メッシュの数だけ、メモリを確保（用はメッシュ一個分）
	pMQOObject->mesh_arr = (MQOMesh*)malloc( sizeof(MQOMesh) * pMQOObject->mesh_num );
    
	memset( pMQOObject->mesh_arr, 0, sizeof(MQOMesh) * pMQOObject->mesh_num );
    
    // 先にメモリを確保しといた方がいいんじゃね？？
    int iVertexNumber = 0;
    int iMaterialIndex;
    
    for (int iLp = 0; iLp < iFaceNumber; iLp++) {
        
        // ここ要修正、、、マテリアルの index は統一されてる？？？バラバラ？？？
        // ここではとりあえずの値
        if (mqoObjectChunk->pMQOFace[iLp].iMaterialIndex < 0) {
            // mat_index = model->material_num;
            // よくわからんので、０にしとく
            iMaterialIndex = 0;
        }
        else {
            iMaterialIndex = mqoObjectChunk->pMQOFace[iLp].iMaterialIndex;
        }
        
        
        if (mqoObjectChunk->pMQOFace[iLp].iVertexNumber == 3) {
			iVertexNumber += 3;
		}
		else if (mqoObjectChunk->pMQOFace[iLp].iVertexNumber == 4) {
			//４頂点（四角）は３頂点（三角）ｘ２に分割
			//  0  3      0    0  3
			//   □   →　△　　▽
			//  1  2     1  2   2
			// ４頂点の平面データは
			// ３頂点の平面データｘ２個
			iVertexNumber += 3 * 2;
		}
    }
    
    // このメッシュに書き込む
    MQOMesh *pMQOMesh = &pMQOObject->mesh_arr[0];

    // 頂点数
    pMQOMesh->vertex_num = iVertexNumber;
    
    // マテリアル番号
    pMQOMesh->iMaterialIndex = iMaterialIndex;
    
    // まず領域確保
    // 頂点数 X 3(x, y, z)
    pMQOMesh->locations          = (float *)calloc(iVertexNumber * 3, sizeof(float));
    pMQOMesh->normals            = (float *)calloc(iVertexNumber * 3, sizeof(float));
    
    // 頂点数 X 2(u, v)
    pMQOMesh->texture_cordinates = (float *)calloc(iVertexNumber * 2, sizeof(float));
    
    
    int iVertexIndex = 0;
    int iUVIndex = 0;

    // メッシュを作ります
    for (int iLp = 0; iLp < iFaceNumber; iLp++) {
        
        // 面情報、、読み出し専用
        MQOFace mqoFace = mqoObjectChunk->pMQOFace[iLp];
        
        if (mqoFace.iVertexNumber == 3) {
            // 面が三角ポリゴン
            // location(3つの頂点ベクトル), normal(3頂点ごとの法線), テクスチャ(u,v 3組)をセット
            
            // 頂点ベクトル
            pMQOMesh->locations[iVertexIndex + 0] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].x;
            pMQOMesh->locations[iVertexIndex + 1] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].y;
            pMQOMesh->locations[iVertexIndex + 2] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].z;
            
            pMQOMesh->locations[iVertexIndex + 3] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]].x;
            pMQOMesh->locations[iVertexIndex + 4] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]].y;
            pMQOMesh->locations[iVertexIndex + 5] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]].z;
            
            pMQOMesh->locations[iVertexIndex + 6] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].x;
            pMQOMesh->locations[iVertexIndex + 7] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].y;
            pMQOMesh->locations[iVertexIndex + 8] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].z;            
            
            // 面法線ベクトル
            glPOINT3f glPOINT3fPlaneNormal;            
            
            // あし改造、、、この関数は時計回りで法線を出している
            // 面法線ベクトルを計算
            mqoSnormal(mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]],
                       mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]],
                       mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]],
                       &glPOINT3fPlaneNormal );              
            
            // ここで、面法線を使うか、頂点法線を使うかをスムージング各で判断しなければ、、、
            // 面法線と頂点法線の角度
            // 各頂点ごとに比較する！
            for (int iLp = 0; iLp < mqoFace.iVertexNumber; iLp++) {
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
                
                glPOINT3f glNormal;
                
                if (mqoObjectChunk->facet < dSAngle) {
                    // スムージング角　＜（頂点法線と面法線の角度）のときは面法線を法線とする
                    // つまり、頂点法線と面法制がかけ離れているときは、面法線を使う、、、
                    // これは、サイコロのときの扱いとしてはいいはず
                    glNormal = glPOINT3fPlaneNormal;
                }
                else {
                    // そうでないときは、頂点法線と面法線の方向が近いときは
                    // 頂点法線を法線にする
                    glNormal = glPOINT3fVertexNormal;
                }
                
                if (iLp == 2) {
                    pMQOMesh->normals[iVertexIndex + 0] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 1] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 2] = glNormal.z;
                }
                else if (iLp == 1) {
                    pMQOMesh->normals[iVertexIndex + 3] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 4] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 5] = glNormal.z;
                }
                else if (iLp == 0) {
                    pMQOMesh->normals[iVertexIndex + 6] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 7] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 8] = glNormal.z;
                }
            }
            
            // UV
            pMQOMesh->texture_cordinates[iUVIndex + 0] = mqoFace.uv[2].x;
            pMQOMesh->texture_cordinates[iUVIndex + 1] = mqoFace.uv[2].y;            
            pMQOMesh->texture_cordinates[iUVIndex + 2] = mqoFace.uv[1].x;
            pMQOMesh->texture_cordinates[iUVIndex + 3] = mqoFace.uv[1].y;                       
            pMQOMesh->texture_cordinates[iUVIndex + 4] = mqoFace.uv[0].x;
            pMQOMesh->texture_cordinates[iUVIndex + 5] = mqoFace.uv[0].y;
            
            // index を増やす
            iVertexIndex += 9;
            iUVIndex     += 6;
        }
        else if (mqoFace.iVertexNumber == 4) {
            // 面が四角ポリゴン -> 三角形２個に分割
            
            // location(4つの頂点ベクトル), normal(4頂点ごとの法線), テクスチャ(u,v 4組)をセット

            // 頂点ベクトル
            pMQOMesh->locations[iVertexIndex + 0]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].x;
            pMQOMesh->locations[iVertexIndex + 1]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].y;
            pMQOMesh->locations[iVertexIndex + 2]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].z;
            
            pMQOMesh->locations[iVertexIndex + 3]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].x;
            pMQOMesh->locations[iVertexIndex + 4]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].y;
            pMQOMesh->locations[iVertexIndex + 5]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].z;
            
            pMQOMesh->locations[iVertexIndex + 6]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]].x;
            pMQOMesh->locations[iVertexIndex + 7]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]].y;
            pMQOMesh->locations[iVertexIndex + 8]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]].z;
            
            /////////////////////
            
            pMQOMesh->locations[iVertexIndex + 9]  = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].x;
            pMQOMesh->locations[iVertexIndex + 10] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].y;
            pMQOMesh->locations[iVertexIndex + 11] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]].z;
            
            pMQOMesh->locations[iVertexIndex + 12] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[3]].x;
            pMQOMesh->locations[iVertexIndex + 13] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[3]].y;
            pMQOMesh->locations[iVertexIndex + 14] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[3]].z;
        
            pMQOMesh->locations[iVertexIndex + 15] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].x;
            pMQOMesh->locations[iVertexIndex + 16] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].y;
            pMQOMesh->locations[iVertexIndex + 17] = mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]].z;
            

            
            // 面法線ベクトル
            glPOINT3f glPOINT3fPlaneNormal;            
            

            // 面法線ベクトルを計算（４角だから、2つの三角形について計算）
            
            // 1個目の四角（０、２、１）
            // この関数は時計回りの法線を出す(時計回りだから順番逆）
            mqoSnormal(mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[1]],
                       mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]],
                       mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]],
                       &glPOINT3fPlaneNormal);
            
            // ここで、面法線を使うか、頂点法線を使うかをスムージング各で判断しなければ、、、
            // 面法線と頂点法線の角度
            // 各頂点ごとに比較する！
            for (int iLp = 0; iLp < 3; iLp++) {
                // 頂点法線
                glPOINT3f glPOINT3fVertexNormal = pglPOINT3fVertexNormals[ mqoFace.aiVertexNumber[iLp] ];
                
                double dSRadian;
                dSRadian = acos(glPOINT3fPlaneNormal.x * glPOINT3fVertexNormal.x +
                                glPOINT3fPlaneNormal.y * glPOINT3fVertexNormal.y +
                                glPOINT3fPlaneNormal.z * glPOINT3fVertexNormal.z);                
                
                // 結果がラジアンで来るので、度になおす
                double dSAngle = dSRadian / 2 / M_PI * 360;                
                
                // facet = スムージング角度(metaseqoia では ０〜１８０の値）    
                
                // スムージングの角度を最大45°としたときの面の表面のとある座標位置での
                // 面法線をN、頂点法線をVNとして考えてみます。
                
                // c1 = N.x * VN.x + N.y * VN.y + N.z * VN.z;
                // c2 = cos(45 * 3.1415926535 / 180.0);
                // 「c1 >= c2」のときは頂点法線を採用します。「c1 < c2」のときは面法線を採用します。
                // スムージング角度が大きいほどスムーズになり、スムージング角度が小さいとフラットなシェーディングになります。 
                
                glPOINT3f glNormal;
                
                if (mqoObjectChunk->facet < dSAngle) {
                    // スムージング角　＜（頂点法線と面法線の角度）のときは面法線を法線とする
                    // つまり、頂点法線と面法制がかけ離れているときは、面法線を使う、、、
                    // これは、サイコロのときの扱いとしてはいいはず
                    glNormal = glPOINT3fPlaneNormal;
                }
                else {
                    // そうでないときは、頂点法線と面法線の方向が近いときは
                    // 頂点法線を法線にする
                    glNormal = glPOINT3fVertexNormal;
                }
                
                if (iLp == 0) {
                    pMQOMesh->normals[iVertexIndex + 0] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 1] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 2] = glNormal.z;
                }
                else if (iLp == 2) {
                    pMQOMesh->normals[iVertexIndex + 3] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 4] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 5] = glNormal.z;
                }
                else if (iLp == 1) {
                    pMQOMesh->normals[iVertexIndex + 6] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 7] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 8] = glNormal.z;
                }
            }
            
            // 2個目の四角（０、3、2）
            // この関数は時計回りの法線を出す
            mqoSnormal(mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[2]],
                       mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[3]],
                       mqoObjectChunk->pglPOINT3fVertex[mqoFace.aiVertexNumber[0]],
                       &glPOINT3fPlaneNormal);
            
            // ここで、面法線を使うか、頂点法線を使うかをスムージング各で判断しなければ、、、
            // 面法線と頂点法線の角度
            // 各頂点ごとに比較する！
            for (int iLp = 0; iLp < 4; iLp++) {
                // 1の時はスキップ
                if (iLp == 1) {
                    continue;
                }
                
                // 頂点法線
                glPOINT3f glPOINT3fVertexNormal = pglPOINT3fVertexNormals[mqoFace.aiVertexNumber[iLp]];
                
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
                
                glPOINT3f glNormal;
                
                if (mqoObjectChunk->facet < dSAngle) {
                    // スムージング角　＜（頂点法線と面法線の角度）のときは面法線を法線とする
                    // つまり、頂点法線と面法制がかけ離れているときは、面法線を使う、、、
                    // これは、サイコロのときの扱いとしてはいいはず
                    glNormal = glPOINT3fPlaneNormal;
                }
                else {
                    // そうでないときは、頂点法線と面法線の方向が近いときは
                    // 頂点法線を法線にする
                    glNormal = glPOINT3fVertexNormal;
                }
                
                if (iLp == 0) {
                    pMQOMesh->normals[iVertexIndex + 9]  = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 10] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 11] = glNormal.z;
                }
                else if (iLp == 3) {
                    pMQOMesh->normals[iVertexIndex + 12] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 13] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 14] = glNormal.z;
                }
                else if (iLp == 2) {
                    pMQOMesh->normals[iVertexIndex + 15] = glNormal.x;
                    pMQOMesh->normals[iVertexIndex + 16] = glNormal.y;
                    pMQOMesh->normals[iVertexIndex + 17] = glNormal.z;
                }
            }

            // UV
            pMQOMesh->texture_cordinates[iUVIndex + 0] = mqoFace.uv[0].x;
            pMQOMesh->texture_cordinates[iUVIndex + 1] = mqoFace.uv[0].y;
            pMQOMesh->texture_cordinates[iUVIndex + 2] = mqoFace.uv[2].x;
            pMQOMesh->texture_cordinates[iUVIndex + 3] = mqoFace.uv[2].y;            
            pMQOMesh->texture_cordinates[iUVIndex + 4] = mqoFace.uv[1].x;
            pMQOMesh->texture_cordinates[iUVIndex + 5] = mqoFace.uv[1].y;
            
            pMQOMesh->texture_cordinates[iUVIndex + 6]  = mqoFace.uv[0].x;
            pMQOMesh->texture_cordinates[iUVIndex + 7]  = mqoFace.uv[0].y;
            pMQOMesh->texture_cordinates[iUVIndex + 8]  = mqoFace.uv[3].x;
            pMQOMesh->texture_cordinates[iUVIndex + 9]  = mqoFace.uv[3].y;
            pMQOMesh->texture_cordinates[iUVIndex + 10] = mqoFace.uv[2].x;
            pMQOMesh->texture_cordinates[iUVIndex + 11] = mqoFace.uv[2].y;
            
            // index を増やす
            iVertexIndex += 18;
            iUVIndex     += 12;
        }        
    }
}

