//
//  MQOModelReadMaterial.m
//  CC3DemoMashUp
//
//  Created by Yasuo Ashina on 12/07/17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MQOModel.h"


/*=========================================================================
 【関数】mqoReadMaterial
 【用途】マテリアル情報の読み込み
 【引数】
 fp		ファイルポインタ
 M		マテリアル配列
 
 【戻値】読み込んだマテリアル数
 【仕様】mqoCreateModel(), mqoCreateSequence()のサブ関数．
 =========================================================================*/

int mqoReadMaterial(FILE *fp, MQOMaterial M[])
{
	float dif, amb, emi, spc;
	glCOLOR4f c;
	char buf[SIZE_STR];
	char *pStrEnd, *pStr;
	int len;
	int	i = 0;
    
	while (!feof(fp))
    {
		fgets(buf,SIZE_STR,fp);	// 行読み込み
        
		if (strstr(buf,"col("))
        {
            
            // 材質名読み飛ばし
            pStr = strstr(buf,"col(");
            
            
            sscanf( pStr,
                   "col(%f %f %f %f) dif (%f) amb(%f) emi(%f) spc(%f) power(%f)",
                   &c.r, &c.g, &c.b, &c.a, &dif, &amb, &emi, &spc, &M[i].power );
            
            // 頂点カラー
            M[i].col = c;
            
            // 拡散光
            M[i].dif[0] = dif * c.r;
            M[i].dif[1] = dif * c.g;
            M[i].dif[2] = dif * c.b;
            M[i].dif[3] = c.a;
            
            // 周囲光
            M[i].amb[0] = amb * c.r;
            M[i].amb[1] = amb * c.g;
            M[i].amb[2] = amb * c.b;
            M[i].amb[3] = c.a;
            
            // 自己照明
            M[i].emi[0] = emi * c.r;
            M[i].emi[1] = emi * c.g;
            M[i].emi[2] = emi * c.b;
            M[i].emi[3] = c.a;
            
            // 反射光
            M[i].spc[0] = spc * c.r;
            M[i].spc[1] = spc * c.g;
            M[i].spc[2] = spc * c.b;
            M[i].spc[3] = c.a;
            
            // tex：模様マッピング名
            if ( (pStr = strstr(buf,"tex(")) != NULL )
            {
                M[i].useTex = TRUE;
                
                pStrEnd = strstr(pStr,")")-1;
                len = pStrEnd - (pStr+5);
                strncpy(M[i].acTextureFileName,pStr+5,len);
                M[i].acTextureFileName[len] = (char)0;
            }
            else
            {
                M[i].useTex = FALSE;
                M[i].acTextureFileName[0] = (char)0;
            }
            
            // aplane：アルファマッピング名
            if ( (pStr = strstr(buf,"aplane(")) != NULL )
            {
                pStrEnd = strstr(pStr,")")-1;
                len = pStrEnd - (pStr+8);
                strncpy(M[i].alpFile,pStr+8,len);
                M[i].alpFile[len] = (char)0;
            }
            else
            {
                M[i].alpFile[0] = (char)0;
            }
            
            
            // bump：バンプマッピング名
            if ( (pStr = strstr(buf,"bump(")) != NULL )
            {
                pStrEnd = strstr(pStr,")")-1;
                len = pStrEnd - (pStr+6);
                strncpy(M[i].bmpFile,pStr+6,len);
                M[i].bmpFile[len] = (char)0;
            }
            else
            {
                M[i].bmpFile[0] = (char)0;
            }
            
            i++;
        }
        else if( strstr( buf, "}" ) )
        {
            break;
        }
        
    }
    
    return i;
}


