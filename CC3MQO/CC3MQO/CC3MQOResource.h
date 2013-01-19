//
//  CC3MQOResource.h
//  cc3MQO
//
//  Created by T.Takabayashi on 11/08/07.
//  Copyright 2011 T.Takabayashi. All rights reserved.
//

#import "MQOModel.h"

#import "CC3Resource.h"

@interface CC3MQOResource : CC3Resource
{    
    MQOModelRef mqoModelRef;

    NSMutableArray *nsMutableArrayMaterials;
    
    // ../../問題を解決するためのカレントパス
    NSString *strCurrentPath;
}

@property (nonatomic, assign) CC3BoundingBox boundingBox;

@end
