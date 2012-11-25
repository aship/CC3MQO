//
//  CC3MQOResource.m
//  cc3MQO
//
//  Created by T.Takabayashi on 11/08/07.
//  Copyright 2011 T.Takabayashi. All rights reserved.
//

#import "CC3MQOResource.h"

@interface CC3MQOResource(Private)

- (void)buildMaterials;
- (void)buildObjects;

@end

@implementation CC3MQOResource

- (id)init
{
	if ( (self = [super init]) )
    {
		wasLoaded = NO;
        mArrMaterial = [[NSMutableArray array] retain];
	}
	return self;
}

- (BOOL)loadFromFile: (NSString*) aFilePath
{

//	NSLog( @"CC3MQOResouce.m の loadFromFile" );
    
    
    if (wasLoaded)
    {
		LogError(@"%@ has already been loaded from MQO file '%@'", self, aFilePath);
		return wasLoaded;
	}
    
    // Ensure the path is absolute, converting it if needed.
	NSString *nsStringAbsoluteFilePath = CC3EnsureAbsoluteFilePath(aFilePath);
    
   // LogTrace(@"Loading MQO file path '%@'", aFilePath);
//	LogTrace(@"Loading MQO file abs path '%@'", absFilePath);
    
    
    // カレントパスを保存
    strCurrentPath = [nsStringAbsoluteFilePath stringByDeletingLastPathComponent];
    
#ifdef DEBUG_PRINT
    NSLog(@"Loading MQO file path '%@'", aFilePath);
	NSLog(@"Loading MQO file abs path '%@'", nsStringAbsoluteFilePath);
	NSLog(@"current path '%@'", strCurrentPath);
#endif

    
	//self.name = aFilePath;
    // hoge.mqo ファイル名
    self.name = [ nsStringAbsoluteFilePath lastPathComponent ];    
    
    // ここで、ファイルから読み込み
    mqoModelRef = MQOModelRefCreate( [ nsStringAbsoluteFilePath UTF8String ] );
    
    if (mqoModelRef == NULL) {
        wasLoaded = NO;
        LogError( @"Could not load MQO file '%@'", nsStringAbsoluteFilePath );
        return NO;
    }
    
    wasLoaded = YES;
    
    [self buildMaterials];
    [self buildObjects];

    MQOModelRefRelease(mqoModelRef);
    mqoModelRef = NULL;
    return wasLoaded;
}

- (void)dealloc
{
    if (mqoModelRef != NULL) {
        MQOModelRefRelease( mqoModelRef );
        mqoModelRef = NULL;
    }
    
    [mArrMaterial release];
    [super dealloc];
}

@end
