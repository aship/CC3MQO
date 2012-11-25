//
//  CC3MQOMaterial.m
//  cc3MQO
//
//  Created by T.Takabayashi on 11/08/07.
//  Copyright 2011 T.Takabayashi. All rights reserved.
//

#import "CC3MQOMaterial.h"


@implementation CC3MQOMaterial

+ (id)materialWithName:(NSString *)aName
       withMQOMaterial:(MQOMaterialRef)mqoMaterial
{
    CC3MQOMaterial *material = [CC3MQOMaterial materialWithName:aName];

    if (!material)
    {
        return nil;
    }
    
    material.color = ccc3(mqoMaterial->col.r * 255, 
                          mqoMaterial->col.g * 255, 
                          mqoMaterial->col.b * 255);
    
    material.ambientColor = CCC4FMake(mqoMaterial->amb[0], 
                                      mqoMaterial->amb[1], 
                                      mqoMaterial->amb[2], 
                                      mqoMaterial->amb[3]);
    
    material.diffuseColor = CCC4FMake(mqoMaterial->dif[0], 
                                      mqoMaterial->dif[1], 
                                      mqoMaterial->dif[2], 
                                      mqoMaterial->dif[3]);
    
    material.emissionColor = CCC4FMake(mqoMaterial->emi[0], 
                                       mqoMaterial->emi[1], 
                                       mqoMaterial->emi[2], 
                                       mqoMaterial->emi[3]);
    
    material.specularColor = CCC4FMake(mqoMaterial->spc[0], 
                                       mqoMaterial->spc[1], 
                                       mqoMaterial->spc[2], 
                                       mqoMaterial->spc[3]);
    
  
    
    return material;
}

// ファイルからテクスチャをセット
-(void) addTextureFromFile:(NSString*)aFilepath
{
    CC3Texture *cc3Texture = [CC3Texture textureFromFile:aFilepath];

    [ self addTexture:cc3Texture ];
    
    CCLOG(@"%@", aFilepath);
    
  //  NSLog(@"%@", aFilepath);
    
    // あしp改造
  //  NSLog( @"AHSIP COMVEJFIJEI" );
  //  self.isOpaque = NO;
    
   // self.shouldIncludeInDeepCopy = YES;
    
    // パキラの透過PNGで崩れるのを直す
    self.shouldDrawLowAlpha = NO;
 //   self.shouldDrawLowAlpha = YES;
    
    
   // self.alphaTestFunction = GL_NOTEQUAL;
    
    
    
    
 //   self.sourceBlend = GL_SRC_ALPHA;
//   self.destinationBlend = GL_ONE_MINUS_SRC_ALPHA;
    
    
  //  NSLog( @"COLORBOX material.isOpaque %@", NSStringFromBoolean( self.isOpaque ) );
  //  NSLog( @"COLORBOX material.textureCount %d", self.textureCount );
  //  NSLog( @"COLORBOX material.hasBumpMap %@", NSStringFromBoolean( self.hasBumpMap ) );
  //  NSLog( @"COLORBOX material.shininess %f", self.shininess );
    
  //  self.shininess = kCC3MaximumMaterialShininess;
    
    
    
    
    
    
    // Indicates whether the RGB components of each pixel of the encapsulated texture
    // have had the corresponding alpha component applied already.
    // Returns YES if this instance contains a CCTexture2D instance, and that texture
    // instance indicates that it contains pre-mulitiplied alpha.
    if( cc3Texture.hasPremultipliedAlpha )
    {
        self.isOpaque = NO;
        self.sourceBlend = GL_ONE;
    }
}

-(void) addAlphaTextureFromFile:(NSString *)aFilepath {
//    return;
    CC3Texture * aTexture = [CC3Texture textureFromFile:aFilepath];

    // アルファマップを作成
    UIImage *img = [UIImage imageWithContentsOfFile:aFilepath];
    
    if (img == nil) {
        CCLOG(@"Could not load image:%@", aFilepath);
        return;
    }
    
    CGColorSpaceRef graySpace = CGColorSpaceCreateDeviceGray();
    CGContextRef alphaContext = CGBitmapContextCreate(nil, 
                                                      img.size.width, 
                                                      img.size.height, 
                                                      8, 
                                                      0, 
                                                      graySpace, 
                                                      kCGImageAlphaNone);
    CGColorSpaceRelease(graySpace);
    CGContextDrawImage(alphaContext, 
                       CGRectMake(0, 0, img.size.width, img.size.height), 
                       [img CGImage]);
    
    CGImageRef maskImg = CGBitmapContextCreateImage(alphaContext);
    
    CGImageRef alpImageRef = CGImageCreateWithMask([img CGImage], maskImg);
    
    if (alpImageRef == nil) {
        CCLOG(@"mask not found:%@", aFilepath);
    } else {
        CCTexture2D *alpTex = [[CCTextureCache sharedTextureCache] addCGImage:alpImageRef 
                                                                       forKey:[aFilepath stringByAppendingString:@".alp"]];
        
        aTexture.texture = alpTex;
        

        CC3ConfigurableTextureUnit *texunit = [CC3ConfigurableTextureUnit textureUnit];
//        texunit.textureEnvironmentMode = GL_COMBINE;
//        
//        texunit.combineRGBFunction = GL_REPLACE;
//        texunit.rgbSource0 = GL_PREVIOUS;
        texture.textureUnit = texunit;
        
        [self addTexture:aTexture];
        
        self.isOpaque = NO;
        self.sourceBlend = GL_ONE;
    }
    CGImageRelease(alpImageRef);
    CGImageRelease(maskImg);
    CGContextRelease(alphaContext);
    
}

-(void) addBumpTextureFromFile:(NSString *)aFilepath {
    CC3Texture * aTexture = [CC3Texture textureFromFile:aFilepath];
    aTexture.textureUnit = [CC3BumpMapTextureUnit textureUnit];
    [self addTexture:aTexture];
}


@end
