//
//  MQOModelNormal.m
//  CC3DemoMashUp
//
//  Created by Yasuo Ashina on 12/07/15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MQOModel.h"






/*=========================================================================
 【関数】mqoSnormal
 【用途】法線ベクトルを求める
 【引数】
 A		3次元座標上の点A
 B		3次元座標上の点B
 C		3次元座標上の点C
 *normal	ベクトルBAとベクトルBCの法線ベクトル（右ねじ方向）
 
 【戻値】なし
 【仕様】メタセコイアにおいて面を構成する頂点の番号は，表示面から見て
 時計回りに記述してある．つまり，頂点A,B,C があったとき，
 求めるべき法線はBAとBCの外積によって求められる
 =========================================================================*/

void mqoSnormal(glPOINT3f A, glPOINT3f B, glPOINT3f C, glPOINT3f *normal)
{
	double norm;
	glPOINT3f vec0,vec1;
    
	// ベクトルBA
	vec0.x = A.x - B.x; 
	vec0.y = A.y - B.y;
	vec0.z = A.z - B.z;
    
	// ベクトルBC
	vec1.x = C.x - B.x;
	vec1.y = C.y - B.y;
	vec1.z = C.z - B.z;
    
	// 法線ベクトル
	normal->x = vec0.y * vec1.z - vec0.z * vec1.y;
	normal->y = vec0.z * vec1.x - vec0.x * vec1.z;
	normal->z = vec0.x * vec1.y - vec0.y * vec1.x;
    
	// 正規化
	norm = normal->x * normal->x + normal->y * normal->y + normal->z * normal->z;
	norm = sqrt ( norm );
    
	normal->x /= norm;
	normal->y /= norm;
	normal->z /= norm;
}


