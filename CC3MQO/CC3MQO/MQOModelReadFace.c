//
//  MQOReadFace.m
//  CC3DemoMashUp
//
//  Created by Yasuo Ashina on 12/07/15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MQOModel.h"



/*=========================================================================
 【関数】mqoReadFace
 【用途】面情報の読み込み
 【引数】
 fp		ファイルポインタ
 F		面配列
 
 【戻値】なし
 【仕様】mqoReadObject()のサブ関数
 =========================================================================*/

#if 0
// 読み込みデータの例
// %d	頂点数	3～4
// V(%d ...)	頂点インデックス (頂点数分の数が存在）	0～頂点数-1
// M(%d)	材質インデックス	-1（ 未着色面）または0～材質数-1
// UV(%.5f %.5f ...)	ＵＶ値 （頂点数×２の数が存在）	

face 6 {
    4 V(0 2 3 1) M(0) UV(0.01047 0.01047 0.27225 0.01047 0.27225 0.53403 0.01047 0.53403)
    4 V(2 4 5 3) M(0) UV(0.29319 0.01047 0.48953 0.01047 0.48953 0.53403 0.29319 0.53403)
    4 V(4 6 7 5) M(0) UV(0.51047 0.01047 0.77225 0.01047 0.77225 0.53403 0.51047 0.53403)
    4 V(6 0 1 7) M(0) UV(0.79319 0.01047 0.98953 0.01047 0.98953 0.53403 0.79319 0.53403)
    4 V(6 4 2 0) M(0) UV(0.01047 0.55497 0.27225 0.55497 0.27225 0.75131 0.01047 0.75131)
    4 V(1 3 5 7) M(0) UV(0.29319 0.55497 0.55497 0.55497 0.55497 0.75131 0.29319 0.75131)
}
#endif

void mqoReadFace( FILE *fp, MQOFace mqoFace[] )
{
	char buf[SIZE_STR];
    
	char *pStr;
	
    int  i = 0;
    
	while (1)
    {
		fgets( buf, SIZE_STR, fp );
        
		if ( strstr( buf, "}" ) )
        {
            break;
        }
        
		// 面を構成する頂点数
		sscanf( buf, "%d", &mqoFace[i].iVertexNumber );
        
		// 頂点(V)の読み込み
		if ( ( pStr = strstr( buf, "V(" ) ) != NULL )
        {
			switch( mqoFace[i].iVertexNumber )
            {
				case 3:
                    // 頂点３個
                    sscanf( pStr,"V(%d %d %d)", 
                           &mqoFace[i].aiVertexNumber[0], 
                           &mqoFace[i].aiVertexNumber[1], 
                           &mqoFace[i].aiVertexNumber[2] );     

					break;
				case 4:
                    // 頂点４つ
                    sscanf( pStr,"V(%d %d %d %d)", 
                           &mqoFace[i].aiVertexNumber[0], 
                           &mqoFace[i].aiVertexNumber[1], 
                           &mqoFace[i].aiVertexNumber[2],
                           &mqoFace[i].aiVertexNumber[3] );
					break;
				default:
					break;
			}		
		}
        
		// マテリアル(M)の読み込み
        
        // この初期化意味ないでしょ？？？
		mqoFace[i].iMaterialIndex = 0;
        
		if( ( pStr = strstr( buf, "M(" ) ) != NULL )
        {
			sscanf( pStr, "M(%d)", &mqoFace[i].iMaterialIndex );
		}
		else
        {
            // マテリアルが設定されていない面(未着色面)
			mqoFace[i].iMaterialIndex = -1;
		}
        
		// UVマップ(UV)の読み込み
		if( ( pStr = strstr( buf, "UV(" ) ) != NULL )
        {
			switch( mqoFace[i].iVertexNumber )
            {
				case 3:
                    // 頂点数3
                    sscanf( pStr, "UV(%f %f %f %f %f %f)",
                           &mqoFace[i].uv[0].x, &mqoFace[i].uv[0].y,
                           &mqoFace[i].uv[1].x, &mqoFace[i].uv[1].y,
                           &mqoFace[i].uv[2].x, &mqoFace[i].uv[2].y
                           );
					break;
				case 4:	
                    // 頂点数4
					sscanf( pStr,"UV(%f %f %f %f %f %f %f %f)",
                            &mqoFace[i].uv[0].x, &mqoFace[i].uv[0].y,
                            &mqoFace[i].uv[1].x, &mqoFace[i].uv[1].y,
                            &mqoFace[i].uv[2].x, &mqoFace[i].uv[2].y,
                            &mqoFace[i].uv[3].x, &mqoFace[i].uv[3].y
                           );
                    break;
				default:
					break;
			}		
		}
        
		i++;
	}
    
  //  return 1;

}
