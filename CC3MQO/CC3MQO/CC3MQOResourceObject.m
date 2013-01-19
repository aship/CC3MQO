//
//  CC3MQOResourceObject.m
//  coco3333
//
//  Created by aship on 12/09/08.
//
//

#import "CC3MeshNode.h"

#import "CC3ParametricMeshes.h"
#import "CC3VertexArrays.h"

#import "CC3MQOResource.h"

@implementation CC3MQOResource (CC3MQOResourceObject)

- (void)buildObjects
{
    // Model -> Object の集合
    // Object -> Mesh の集合？？？
    
    CC3Node *cc3NodeModel = [ CC3Node nodeWithName:self.name ];
    
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
        for (int j = 0; j < mqoObject->mesh_num; j++) {
            MQOMesh *mqoMesh = &mqoObject->mesh_arr[j];
        
            CC3VertexArrayMesh *cc3VertexArrayMesh = [CC3VertexArrayMesh meshWithName:@"VertexArrayMesh"];
            cc3VertexArrayMesh.shouldInterleaveVertices = YES;
            
            // Prepare the vertex content and allocate space for vertices and indices.
            [cc3VertexArrayMesh ensureVertexContent];
            
            NSInteger intVertexNum = mqoMesh->vertex_num;
            
            cc3VertexArrayMesh.allocatedVertexCapacity      = intVertexNum;
            cc3VertexArrayMesh.allocatedVertexIndexCapacity = intVertexNum;
            
            [cc3VertexArrayMesh setExpectsVerticallyFlippedTextures:NO];
#if 1
            // 今回 vertex num が巨大な数字に、、、
            // ３頂点ポリゴン
       //     NSLog(@"intVertexNum %d", intVertexNum);
            [cc3VertexArrayMesh setDrawingMode:GL_TRIANGLES];
#else
            if (intVertexNum == 3) {
                // ３頂点ポリゴン
                [cc3VertexArrayMesh setDrawingMode:GL_TRIANGLES];
            }
            else if (intVertexNum == 4) {
                // 4頂点の四角ポリゴン
                // 三角形２個として描画
                [cc3VertexArrayMesh setDrawingMode:GL_TRIANGLE_STRIP];
            }
#endif
            
            for (int iLp = 0; iLp < intVertexNum; iLp++) {
                // location
                CC3Vector vectorLocation = CC3VectorMake(mqoMesh->locations[iLp*3], mqoMesh->locations[iLp*3+1], mqoMesh->locations[iLp*3+2]);
                [cc3VertexArrayMesh setVertexLocation:vectorLocation at:iLp];
                
                // texture_cordinates;
                ccTex2F tex2f = CC3TexCoordsMake(mqoMesh->texture_cordinates[iLp*2], 1 - mqoMesh->texture_cordinates[iLp*2+1]);
                [cc3VertexArrayMesh setVertexTexCoord2F:tex2f at:iLp];
                
                // set normal
                CC3Vector vectorNormal = CC3VectorMake(mqoMesh->normals[iLp*3], mqoMesh->normals[iLp*3+1], mqoMesh->normals[iLp*3+2]);
                [cc3VertexArrayMesh setVertexNormal:vectorNormal at:iLp];
                
                // set index
                [cc3VertexArrayMesh setVertexIndex:iLp at:iLp];
            }
            
            // よくわからんが、、BoundingBoxを計算して、インスタンス変数に保存
            // これはどこで使われる？？？ - (CC3BoundingBox)resouceBoundingBox で返される
            // 常に、最大、最小に更新している
            CC3BoundingBox meshBoundingBox = cc3VertexArrayMesh.boundingBox;
            CC3Vector cc3VectorMin = CC3VectorMinimize(self.boundingBox.minimum, meshBoundingBox.minimum);
            CC3Vector cc3VectorMax = CC3VectorMaximize(self.boundingBox.maximum, meshBoundingBox.maximum);
            self.boundingBox = CC3BoundingBoxFromMinMax(cc3VectorMin, cc3VectorMax);
            
            CC3MeshNode *cc3MeshNode = [CC3MeshNode nodeWithName:cc3VertexArrayMesh.name];
            
            
            @try {
                // CC3Mesh* mesh にセット
                cc3MeshNode.mesh = cc3VertexArrayMesh;
                
                // CC3Material* material にセット
                cc3MeshNode.material = [nsMutableArrayMaterials objectAtIndex:mqoMesh->iMaterialIndex];
                
                [cc3MeshNode createGLBuffers];
                [cc3MeshNode releaseRedundantData];
                
                [cc3NodeObject addChild:cc3MeshNode];
            }
            @catch(NSException *exception) {
            }
        
#if 0
            // CC3Mesh* mesh にセット
            cc3MeshNode.mesh = cc3VertexArrayMesh;
            
            if (mqoMesh->iMaterialIndex < [nsMutableArrayMaterials count]) {
                // CC3Material* material にセット
                cc3MeshNode.material = [nsMutableArrayMaterials objectAtIndex:mqoMesh->iMaterialIndex];
                
                [cc3MeshNode createGLBuffers];
                [cc3MeshNode releaseRedundantData];
                
                
                // ノードを CC3Resource の CCArray *nodes に追加
                [nodes addObject:cc3MeshNode];
            }
            else {
                
                NSLog(@"mqoMesh->iMaterialIndex %d", mqoMesh->iMaterialIndex);
                NSLog(@"[nsMutableArrayMaterials count] %d", [nsMutableArrayMaterials count]);
                NSLog(@"   ");
                // mqoMesh->iMaterialIndex -1
                // [nsMutableArrayMaterials count] 2
            }
#endif
            
        }
        
        [cc3NodeModel addChild:cc3NodeObject];
    }
    
	[cc3NodeModel createGLBuffers];
    [cc3NodeModel releaseRedundantData];
    
    // ノードを CC3Resource の CCArray *nodes に追加
    [nodes addObject:cc3NodeModel];
}

@end
