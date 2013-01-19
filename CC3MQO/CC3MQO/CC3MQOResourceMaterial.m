//
//  CC3MQOResourceMaterial.m
//  coco3333
//
//  Created by aship on 12/09/08.
//
//

#import "CC3MQOMaterial.h"

#import "CC3MQOResource.h"

@implementation CC3MQOResource (CC3MQOResourceMaterial)



- (void)buildMaterials
{
    // stringByDeletingLastPathComponent は、cd したように、一つ上のパスを表す
    // でも、 self.name はファイル名なので、、意味なし？？？
    NSString *pathDir = [self.name stringByDeletingLastPathComponent];
    
    for( int iLp = 0 ; iLp < mqoModelRef->iMaterialNumber ; iLp++ )
    {
        MQOMaterialRef mqoMaterialRef = &mqoModelRef->pMQOMaterial[iLp];
        
        // ファイル名以外の情報をセット
        CC3MQOMaterial *cc3MOQMaterial = [ CC3MQOMaterial materialWithName:[ NSString stringWithFormat:@"MAT-%d", iLp ]
                                           withMQOMaterial:mqoMaterialRef ];
        
        // bump map
        if (strlen( mqoMaterialRef->bmpFile ) > 0)
        {
            //  NSLog( @"###BUMP MAP" );
            
            NSString *bumpFile = [[NSString stringWithCString:mqoMaterialRef->bmpFile
                                                     encoding:NSShiftJISStringEncoding]
                                  stringByReplacingOccurrencesOfString:@"\\"
                                  withString:@"/"];
            [ cc3MOQMaterial addBumpTextureFromFile:
             [pathDir stringByAppendingPathComponent:
              [bumpFile lastPathComponent]]];
            
        }
        
        // texture map
        if (strlen( mqoMaterialRef->acTextureFileName ) > 0 )
        {
            
            //   NSLog( @"###TWEX MAP" );
            // 毎回ここ来てるっぽい
      //      NSLog( @"#####TWEX MAP %s", mqoMaterialRef->acTextureFileName);
       
            //                          ../../floor_tex/4/tex.jpg
            // NSHome Library/Caches/material/
            // に変換する必要あり！！！！
            //#####TWEX MAP ../../floor_tex/4/tex.jpg
            
            
#if 1
            // char -> NSString 変換
            NSString *strTexFilename = [ NSString stringWithCString:mqoMaterialRef->acTextureFileName
                                                           encoding:NSShiftJISStringEncoding ];
            
            NSString *strFullPath;
            
            
            NSRange searchResult = [strTexFilename rangeOfString:@"../"];
            if(searchResult.location == NSNotFound) {
                // みつからない場合の処理
    
                strFullPath = [strCurrentPath stringByAppendingPathComponent:strTexFilename];
            } else {
                // みつかった場合の処理
                NSMutableString *mStr = [strTexFilename mutableCopy];
                
                // これ、../../ と２回続くことを前提とし多コード、、、あとで直す
                
                NSString *strTmpCurrentPath = [strCurrentPath copy];
                strTmpCurrentPath = [strTmpCurrentPath stringByDeletingLastPathComponent];
                strTmpCurrentPath = [strTmpCurrentPath stringByDeletingLastPathComponent];
                
                // 1 - 6 文字目削除
                [mStr deleteCharactersInRange:NSMakeRange(0,6)];
                
                strFullPath = [strTmpCurrentPath stringByAppendingPathComponent:mStr];
            }
            
            
 //           NSLog( @"#####TWEX MAP strFullPath %@", strFullPath);
            [cc3MOQMaterial addTextureFromFile:strFullPath];
            
            
#else
            // char -> NSString 変換
            NSString *strTexFilename = [ NSString stringWithCString:mqoMaterialRef->acTextureFileName
                                                           encoding:NSShiftJISStringEncoding ];

            NSString *strFullPath;
            
            
            NSRange searchResult = [strTexFilename rangeOfString:@"../"];
            if(searchResult.location == NSNotFound){
                // みつからない場合の処理
                
                //strFullPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches"];
                //strFullPath = [strFullPath stringByAppendingPathComponent:@"material/bg/1"];
                //strFullPath = [strFullPath stringByAppendingPathComponent:strTexFilename];
                
                strFullPath = [strCurrentPath stringByAppendingPathComponent:strTexFilename];
                
               // strFullPath = strTexFilename;
            }else{
                // みつかった場合の処理
                NSMutableString *mStr = [strTexFilename mutableCopy];
                
                // 1 - 6 文字目削除
                [mStr deleteCharactersInRange:NSMakeRange(0,6)];
                
                
                strFullPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches"];
                strFullPath = [strFullPath stringByAppendingPathComponent:@"material"];
                strFullPath = [strFullPath stringByAppendingPathComponent:mStr];
                
            }

         //   NSString *strFullPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches"];
         //   strFullPath = [strFullPath stringByAppendingPathComponent:@"material/floor_tex/4/tex.jpg"];
         //   strFullPath = [strFullPath stringByAppendingPathComponent:strTexFilename];
            
            NSLog( @"#####TWEX MAP strFullPath %@", strFullPath);
            [cc3MOQMaterial addTextureFromFile:strFullPath];
#endif
            
            
#if 0
            NSString *nsStringFileName = [ NSString stringWithCString:mqoMaterialRef->acTextureFileName
                                                     encoding:NSShiftJISStringEncoding ];
            
            // 文字の置き換え
            NSString *nsStringTexFile = [ nsStringFileName stringByReplacingOccurrencesOfString:@"\\"
                                                                     withString:@"/" ];
            
            // stringByAppendingPathComponent はパスを追加、、、
            [ cc3MOQMaterial addTextureFromFile:[ pathDir stringByAppendingPathComponent:[nsStringTexFile lastPathComponent]]];
#endif
        }
        
        // alpha map
        if (strlen(mqoMaterialRef->alpFile) > 0)
        {
            
            //   NSLog( @"###Alpha MAP" );
            
            
            
            NSString *alpFile = [[NSString stringWithCString:mqoMaterialRef->alpFile
                                                    encoding:NSShiftJISStringEncoding]
                                 stringByReplacingOccurrencesOfString:@"\\"
                                 withString:@"/"];
            [ cc3MOQMaterial addAlphaTextureFromFile:
             [pathDir stringByAppendingPathComponent:
              [alpFile lastPathComponent]]];
        }
        
        
        //  CC3MQOMaterial : CC3Material を追加
        [ nsMutableArrayMaterials addObject:cc3MOQMaterial ];
        
    }
    
    
    /*  shinyWhite  *
     * Allocates and initializes an autoreleased unnamed instance with an automatically
     * generated unique tag value. The tag value is generated using a call to nextTag.
     *
     * The returned instance will have both diffuseColor and specularColor set to
     * { 1.0, 1.0, 1.0, 1.0 } and a shininess of 75.0.
     */
    // なにこれ？
    
    // 未設定材質を追加
    [ nsMutableArrayMaterials addObject:[ CC3Material shinyWhite ] ];
    
    
}





@end



