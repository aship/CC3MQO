//
//  MQOModel.c
//  cc3MQO
//
//  Created by T.Takabayashi on 11/08/04.
//  Copyright 2011 T.Takabayashi.
//

#include "MQOModel.h"


// 関数宣言
#ifdef __cplusplus
extern "C" {
#endif
    
    
    int mqoLoadFile(MQOModel *mqoModel, MQOObjectChunk *mqoObjectChunk, const char *filename );
    
    
    
    void mqoReadScene(FILE *fp, MQOModel *model, double scale);

    int mqoReadMaterial(FILE *fp, MQOMaterial M[]);
    int mqoReadObject(FILE *fp, MQOObjectChunk *obj);
    int mqoReadVertex(FILE *fp, glPOINT3f V[]);
    int	mqoReadBVertex(FILE *fp,glPOINT3f V[]);
     
    glPOINT3f *mqoCreateVertexNormals(MQOObjectChunk *obj);
    void mqoSnormal(glPOINT3f A, glPOINT3f B, glPOINT3f C, glPOINT3f *normal);
    
    void mqoMakePolygon( MQOObject *pMQOObject,
                        MQOObjectChunk *mqoObjectChunk,
                        glPOINT3f *pglPOINT3fVertexNormals ); 

    void mqoMakeVertexArray(MQOMesh *mesh, 
                            MQOFace face_arr[], 
                            int face_num, 
                            glPOINT3f vertex_arr[],
                            glPOINT3f normals_arr[], 
                            double facet, 
                            double scale, 
                            unsigned char alpha);

    void mqoTransformCoordinates (glPOINT3f *v);
    

    
#ifdef __cplusplus
}
#endif




// MQOModel 作成
MQOModelRef MQOModelRefCreate( const char *filename ) 
{
    
	MQOModelRef mqoModelRef;
    
	// 領域確保と初期化
	mqoModelRef = (MQOModelRef)malloc(sizeof(MQOModel));
    
    if( mqoModelRef == NULL )
    {
        return NULL;
    }
    
	memset( mqoModelRef, 0, sizeof(MQOModel) );
      

    
	MQOObjectChunk mqoObjectChunk[ MAX_OBJECT ];
    
    // メモリを全部０にする
    memset( mqoObjectChunk, 0, sizeof( mqoObjectChunk ) );
        

    
    int iResult;

    // ファイル読み込み
    // ファイルポインタが無効のとき、その他想定外エラーのとき、0 が返ってくる
    iResult = mqoLoadFile( mqoModelRef, mqoObjectChunk, filename );    
    
    
    
    ///////////////////////
    
    
    for( int iLp = 0 ; iLp < mqoModelRef->iObjectNumber ; iLp++ )
    {
        // 各頂点の頂点法線
        glPOINT3f *pglPOINT3fVertexNormals;
        
        pglPOINT3fVertexNormals = mqoCreateVertexNormals( &mqoObjectChunk[iLp] );
        
        // MQOObject を取得    
        MQOObject *pMQOObject = &mqoModelRef->aMQOObject[ iLp ];
        
        
        
        // 面法線は、この関数の内部で計算、処理している
        mqoMakePolygon( pMQOObject,
                       &mqoObjectChunk[iLp],
                       pglPOINT3fVertexNormals );
                //       1.0,
                  //     (unsigned char)255 );
        
        free( pglPOINT3fVertexNormals );
    }
    
    
    
	// オブジェクトのデータの開放
	for( int iLp = 0 ; iLp < mqoModelRef->iObjectNumber ; iLp++ )
    {
		free( mqoObjectChunk[ iLp ].pglPOINT3fVertex );
		free( mqoObjectChunk[ iLp ].pMQOFace );
	}
    


    
    if( iResult == 0 )
    {
        MQOModelRefRelease( mqoModelRef );
        mqoModelRef = NULL;
    }
    
	return mqoModelRef;
}

/*=========================================================================
 【関数】MQOModelRefRelease
 【用途】MQOモデルを削除する
 【引数】
 model	MQOモデル
 
 【戻値】なし
 =========================================================================*/
void MQOModelRefRelease(MQOModelRef model)
{
    
    if (model == NULL)
    {
        return;
    }
    
	int o, m;
	
    MQOObject *obj;
	
    MQOMesh *mesh;
    
    for (o = 0; o < model->iObjectNumber; o++)
    {
        obj = &(model->aMQOObject[o]);
        for ( m = 0; m < obj->mesh_num; m++ )
        {
            mesh = &obj->mesh_arr[m];
            if ( mesh->vertex_num <= 0 ) continue;
            
            // 頂点配列の削除
            if (mesh->locations != NULL) {
                free(mesh->locations);
                mesh->locations = NULL;
            }
            if (mesh->normals != NULL) {
                free(mesh->normals);
                mesh->normals = NULL;
            }
            if (mesh->texture_cordinates != NULL) {
                free(mesh->texture_cordinates);
                mesh->texture_cordinates = NULL;
            }
            
            
        }
        if ( obj->mesh_arr != NULL ) {
            free(obj->mesh_arr);
            obj->mesh_arr = NULL;
        }
        obj->mesh_num = 0;
    }
    
	// マテリアルの開放
	free( model->pMQOMaterial );

    free( model );
}


