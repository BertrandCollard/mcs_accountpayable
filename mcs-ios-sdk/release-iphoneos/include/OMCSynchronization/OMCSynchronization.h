//
//  OMCSynchronization.h
//  OMCSynchronization
//
//  Copyright (c) 2015, Oracle Corp. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OMCCore/OMCServiceProxy.h"
#import "OMCSyncPolicy.h"
#import "OMCSyncGlobals.h"
#import "OMCMobileEndpoint.h"

@class OMCSyncSettings;

/**
 The main class for the Synchronization Service implementation. Contains methods to make requests and manipulate the cache.
 */
@interface OMCSynchronization : OMCServiceProxy


/**
 Initializes an `OMCSynchronization` object with the settings from 'synchronization' node of OMC.plist
 * This method OR initializeWithMobileObjectEntities: method, should be the first method called on the OMCSynchronizaiton class
 */
- (void) initialize;

/**
 Initializes an `OMCSynchronization` object with the settings from 'synchronization' node of OMC.plist and creates database entities for your business model or mobile objects using provided custom classes.
 @param customEntities (optional) Array of custom classes that derived from OMCMobileObject class, use class properties as entity attributes for queries.
    Example array: [NSArray arrayWithObjects:[MyClass1 class], [MyClass2 class], ... , nil]
 */
- (void) initializeWithMobileObjectEntities:(NSArray *) customEntities;


/**
 Returns an object that provides access to an endpoint in a custom code API.
 @param mobileClass (optional) The custom mobile object (entity) class, that the custom code API exposes.
 ( custom mobile object class must have been registered through initializeWithMobileObjectEntities: method. Or pass nil. )
 @param apiName The name of the custom code API.
 @param endpointPath The endpoint in the custom code API.
 @return An MobileEndpoint object for accessing custom code.
 */
- (OMCMobileEndpoint *) openEndpoint:(Class) mobileClass
                             apiName:(NSString *) apiName
                        endpointPath:(NSString *) endpointPath;

/**
 * Request for any method, could go to server or cache based on policy settings.
 @param uri The URI to send the request to.
 @param method RequestMethod value.
 @param policy (optional) SyncPolicy object which has all policy settings.
 @param headers (optional) NSDictionary for extra headers to support request.
 @param data (optional) NSData for http body data, can set `nil` for GET method.
 @param success (optional) The block will be invoked on success of the request.
 @param error (optional) The block will be invoked on error of the request.
 */
- (void) requestWithUri:(NSString *) uri
                 method:(SyncRequestMethod) method
             syncPolicy:(OMCSyncPolicy *) policy
                headers:(NSDictionary *) headers
                   data:(NSData *) data
              onSuccess:(OMCSyncSuccessBlock) success
                onError:(OMCSyncErrorBlock) error;


/**
 * Synchronize all pinned resources.
 @param background boolean to specify for background mode sync.
 */
-(void) synchronizePinnedResources:(BOOL) background;

/**
 * Event that is raised after every offline resource is synchronized with the service.
 @param synchronizedResource The block, invoked for each resource finished synchronized, pass nil to be removed from callbacks.
 */
- (void) offlineResourceSynchronized:(OMCSyncResourceChanged) synchronizedResource;

/**
 * Event that is raised every time a cached resource is updated either from new data from the service
 * or from a online or offline-write by the application or on a delete.
 @param changedResource The block, invoked for each resource changed in cache, pass nil to be removed from callbacks.
 */
- (void) cachedResourceChanged:(OMCSyncResourceChanged) changedResource;

/**
 * Evict ( delete ) the resource from the local cache.
 @param uri The URI of the resource.
 @param error (optional) The block will be invoked on error of the request.
 */
- (void) evictResource:(NSString *) uri
               onError:(OMCSyncErrorBlock) error;

/**
 * Returns the number of cache hits.
 * @return The number of cache hits.
 */
- (int) cacheHitCount;

/**
 * Returns the number of cache misses.
 * @return The number of cache misses.
 */
- (int) cacheMissCount;

/**
* Purge entire store with all files.
*/
- (void) purge;

/**
 * Sets device to offline mode. Useful for testing
 * If the device is actually offline, this setting will be ignored.
 @param isOffline boolean to set offline mode
 */
- (void) setOfflineMode:(BOOL) isOffline;

/**
 * Gets device network status, that is currently being used by Synchronization.
 @return SyncNetworkStatus enum
 */
- (SyncNetworkStatus) getNetworkStatus;

@end


extern NSString* const OMCSynchronizationVersion;
