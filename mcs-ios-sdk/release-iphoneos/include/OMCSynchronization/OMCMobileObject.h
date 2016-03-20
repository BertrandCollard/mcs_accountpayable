//
//  OMCMobileObject.h
//  OMCSynchronization
//
//  Copyright (c) 2015 Oracle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OMCMobileResource.h"
#import "OMCSyncGlobals.h"

@class OMCMobileEndpoint;

/**
  * Class that represents an object returned by a custom code API. Callers can deliver from OMCMobileObject to provide strongly typed properties for the object.
 */
@interface OMCMobileObject : OMCMobileResource

/**
 * The underlying JSON representation of the object.
 */
- (id) jsonObject;

/**
 * Object's persistent state ( server copy ).
 */
- (id) jsonObjectPersistentState;

/**
 * Adds the property to json object. It will not save unit saveResource method invoked.
 * @param propertyName key cannot be nil
 * @param value value of the property
 */
-(void) addOrUpdateJsonProperty:(NSString *) propertyName
                  propertyValue:(NSString *) value;

/**
  * Saves any changes to the object back to the service. Caches the changes locally if offline.
 @param successBlk block that will be called after request successfully finished with the mobile resource.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) saveResourceOnSuccess:(OMCObjectSuccess) successBlk
                       OnError:(OMCSyncErrorBlock) errorBlk;;

/**
  * Saves any changes to the object back to the service.
@param saveIfOffline If true will cache updates locally and sync them back to the service if the device is offline, if false will fail if the device is offline. ( This will override update policy )
 @param successBlk block that will be called after request successfully finished with the mobile resource.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) saveResource:(BOOL) saveIfOffline
            OnSuccess:(OMCObjectSuccess) successBlk
              OnError:(OMCSyncErrorBlock) errorBlk;

/**
  * Deletes the object. Caches the delete locally if offline.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) deleteResourceOnError:(OMCSyncErrorBlock) errorBlk;

/**
  * Deletes the object.
@param deleteIfOffline If true will cache the delete locally and sync back to the service if the device is offline, if false will fail if the device is offline.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) deleteResource:(BOOL) deleteIfOffline
                OnError:(OMCSyncErrorBlock)errorBlk;

@end
