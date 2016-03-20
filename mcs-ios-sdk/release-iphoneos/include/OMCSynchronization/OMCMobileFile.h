//
//  OMCMobileFile.h
//  OMCSynchronization
//
//  Copyright (c) 2015 Oracle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OMCMobileResource.h"
#import "OMCSyncGlobals.h"

@class OMCMobileEndpoint;

/**
 * Class that represents an file or blob returned by a custom code API.
 */
@interface OMCMobileFile : OMCMobileResource

/**
 * Returns the raw file data.
 */
- (NSData *) getData;

/**
 * Returns the raw file data. ( Server copy )
 */
- (NSData *) getDataPersistentState;

/**
 * Returns the input stream of the data.
 */
- (NSInputStream *) getDataStream;

/**
 * Returns the file path of the raw file data.
 */
- (NSString *) getFilepath;

/**
 * Returns the file path of the raw file data. ( Server copy )
 */
- (NSString *) getFilepathPersistentState;

/**
 * Saves any changes to the file back to the service. Caches the changes locally if offline.
 @param data raw data for file
 @param contentType type of content in data for file
 @param successBlk block that will be called after request successfully finished with the mobile resource.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) saveResource:(NSData *) data
          contentType:(NSString *) contentType
            OnSuccess:(OMCFileSuccess) successBlk
              OnError:(OMCSyncErrorBlock) errorBlk;
;

/**
 * Saves any changes to the file back to the service.
 @param data raw data for file
 @param contentType type of content in data for file
 @param saveIfOffline If true will cache updates locally and sync them back to the service if the device is offline, if false will fail if the device is offline. ( This will override update policy )
 @param successBlk block that will be called after request successfully finished with the mobile resource.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) saveResource:(NSData *) data
          contentType:(NSString *) contentType
        saveIfOffline:(BOOL) saveIfOffline
            OnSuccess:(OMCFileSuccess) successBlk
              OnError:(OMCSyncErrorBlock) errorBlk;
;

/**
 * Deletes the file. Caches the delete locally if offline.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) deleteResourceOnError:(OMCSyncErrorBlock) errorBlk;

/**
 * Deletes the file.
 @param deleteIfOffline If true will cache the delete locally and sync back to the service if the device is offline, if false will fail if the device is offline.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) deleteResource:(BOOL) deleteIfOffline
                OnError:(OMCSyncErrorBlock) errorBlk;

@end
