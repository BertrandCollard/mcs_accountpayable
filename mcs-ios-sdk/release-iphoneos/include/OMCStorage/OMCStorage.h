//
//  OMCStorage.h
//  OMCStorage
//
//  Copyright (c) 2015, Oracle Corp. All rights reserved. 
//

#import <Foundation/Foundation.h>
#import "OMCCore/OMCServiceProxy.h"
#import "OMCStorageCollection.h"

/**
 This is the main class for Storage API operations.
 Please also include OMCSynchronization library and SyncStore.mom with your app
 */
@interface OMCStorage : OMCServiceProxy

/**
 Fetch an `OMCStorageCollection` from the Storage Service.
 The `OMCStorageCollection` is specified with the collectionID.
 
 @param collectionID The ID of the `OMCStorageCollection` to be fetched.
 @return The target `OMCStorageCollection` object.
 */
-(OMCStorageCollection*) getCollection: (NSString*) collectionID;



/**
 Fetch the `OMCStorageCollection` which is owned by a different user specified with `userId` here.
 The `OMCStorageCollection` should be a user-isolated collection.
 The current user could fetch the collection only if he or she has one of the following permissions
 READ_ALL, READ_WRITE_ALL.
 
 @param collectionID The ID of the `OMCStorageCollection`.
 @param userId The ID of the user who owns the collection.
 */
-(OMCStorageCollection*) getCollection:(NSString*) collectionID
                             forUserId:(NSString*) userId;

@end

/**
 The Storage library's current version.
 */
extern NSString* const OMCStorageVersion;
