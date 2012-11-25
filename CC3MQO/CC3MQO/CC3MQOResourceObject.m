//
//  CC3MQOResourceObject.m
//  coco3333
//
//  Created by aship on 12/09/08.
//
//

#import "CC3MeshNode.h"
#import "CC3VertexArrayMesh.h"
#import "CC3MQOResource.h"

@implementation CC3MQOResource (CC3MQOResourceObject)

- (void)buildObjects
{
    // Model -> Object の集合
    // Object -> Mesh の集合？？？
    
    CC3Node *cc3NodeModel = [CC3Node nodeWithName:self.name];
    
    for (int i = 0; i < mqoModelRef->iObjectNumber; i++) {
        
        MQOObject *mqoObject = &mqoModelRef->aMQOObject[i];
        
        CC3Node *cc3NodeObject = [CC3Node nodeWithName:[NSString stringWithCString:mqoObject->objname
                                                                          encoding:NSShiftJISStringEncoding]];
        
        if (mqoObject->isVisible == 0) {
            cc3NodeObject.visible = NO;
        }
        else {
            cc3NodeObject.visible = YES;
        }
        
        // 一つの三角ポリこんを一つのメッシュにして、たくさんのメッシュを生成するか
        // 全部のポリゴンを一つのメッシュにまとめるか
        for (int j = 0; j < mqoObject->mesh_num; j++)
        {
            MQOMesh *mqoMesh = &mqoObject->mesh_arr[j];
        
            CC3VertexLocations *cc3VertexLocations = [ CC3VertexLocations vertexArrayWithName:@"locations" ];
            // 頂点数
            cc3VertexLocations.vertexCount = mqoMesh->vertex_num;
            // 頂点情報
            cc3VertexLocations.vertices     = mqoMesh->locations;
            
            if (mqoMesh->vertex_num == 3) {
                // ３頂点の三角ポリゴン
                cc3VertexLocations.drawingMode  = GL_TRIANGLES;
            }
            else if (mqoMesh->vertex_num == 4) {
                // 4頂点の四角ポリゴン 三角形２個として描画
                cc3VertexLocations.drawingMode = GL_TRIANGLE_STRIP;
            }
            
            CC3VertexNormals *cc3VertexNormals = [ CC3VertexNormals vertexArrayWithName:@"normals" ];
            
            // 法線の数
            cc3VertexNormals.vertexCount = mqoMesh->vertex_num;
            
            // 法線情報
            cc3VertexNormals.vertices    = mqoMesh->normals;
            
            // テクスチャ
            CC3VertexTextureCoordinates *cc3VertexTextureCoordinates = [ CC3VertexTextureCoordinates vertexArrayWithName:@"texture" ];
            cc3VertexTextureCoordinates.vertexCount = mqoMesh->vertex_num;
            cc3VertexTextureCoordinates.vertices    = mqoMesh->texture_cordinates;
            
            // 頂点+法線+テクスチャをまとめる
            CC3VertexArrayMesh *cc3VertexArrayMesh = [CC3VertexArrayMesh meshWithName:[NSString stringWithFormat:@"%@-MESH-%d",
                                                                                       cc3NodeObject.name, i ] ];
            cc3VertexArrayMesh.vertexLocations          = cc3VertexLocations;
            cc3VertexArrayMesh.vertexNormals            = cc3VertexNormals;
            cc3VertexArrayMesh.vertexTextureCoordinates = cc3VertexTextureCoordinates;
        
            // よくわからんが、、BoundingBoxを計算して、インスタンス変数に保存
            // これはどこで使われる？？？ - (CC3BoundingBox)resouceBoundingBox で返される
            // 常に、最大、最小に更新している
            CC3BoundingBox meshBoundingBox = cc3VertexArrayMesh.boundingBox;
            CC3Vector cc3VectorMin = CC3VectorMinimize( self.boundingBox.minimum, meshBoundingBox.minimum );
            CC3Vector cc3VectorMax = CC3VectorMaximize( self.boundingBox.maximum, meshBoundingBox.maximum );
            
            self.boundingBox = CC3BoundingBoxFromMinMax( cc3VectorMin, cc3VectorMax );

            CC3MeshNode *cc3MeshNode = [CC3MeshNode nodeWithName:cc3VertexArrayMesh.name];
            
            // CC3Mesh* mesh にセット
            cc3MeshNode.mesh = cc3VertexArrayMesh;

            // CC3Material* material にセット
            cc3MeshNode.material = [mArrMaterial objectAtIndex:mqoMesh->iMaterialIndex];
#if 0
            CC3Texture* texture;
            CCArray* textureOverlays;
            ccColor4F ambientColor;
            ccColor4F diffuseColor;
            ccColor4F specularColor;
            ccColor4F emissionColor;
            GLfloat shininess;
            GLenum alphaTestFunction;
            GLfloat alphaTestReference;
            ccBlendFunc blendFunc;
            BOOL shouldUseLighting : 1;
#endif
            
       //     cc3MeshNode.material.isOpaque = YES;
       //     cc3MeshNode.material.shouldUseLighting = YES;
       //     cc3MeshNode.material.shininess = 10;
            
            
            [cc3NodeObject addChild:cc3MeshNode];
        }
        
        [ cc3NodeModel addChild:cc3NodeObject ];
    }
    
	[cc3NodeModel createGLBuffers];
    [cc3NodeModel releaseRedundantData];
    
    // ノードを CC3Resource の CCArray *nodes に追加
    [nodes addObject:cc3NodeModel];
}

@end
