//
//  OMCSyncGlobals.h
//  OMCSynchronization
//
//  Copyright (c) 2015, Oracle Corp. All rights reserved. 
//

#import <Foundation/Foundation.h>

@class OMCMobileFile;
@class OMCMobileResource;

/**
 * The callback on sucessfully fetching the resource.
 @param mobileResource This could be OMCMobileObject, or custom mobile object,
 OMCMobileObjectCollection or OMCMobileFile.
 */
typedef void(^OMCMobileResourceSuccess) (id mobileResource);

/**
 * The callback on sucessfully fetching the object.
 @param mobileObject This could be OMCMobileObject, or custom mobile object.
 */
typedef void(^OMCObjectSuccess) (id mobileObject);

/**
 The callback on successfully fetching the mobile file.
 @param mobileFile The mobile file retrieved.
 */
typedef void(^OMCFileSuccess) (OMCMobileFile * mobileFile);

/**
 The callback used when an error ocurred fetching data.
 @param error The standard error object.
 */
typedef void(^OMCSyncErrorBlock) (NSError* error);

/**
 The callback used when data weas successfully fetched.
 @param data The data fetched.
 @param response The HTTP response object.
 */
typedef void(^OMCSyncSuccessBlock) (NSData* data, NSHTTPURLResponse* response);


/**
 The callback used when resource synchronized or refreshed in cache.
 @param uri Uri of the changed resource
 @param resource MobileResource that is changed in cache ( mobileResource could be one of OMCMobileObject/OMCMobileObjectCollection/OMCMobileFile or customClass's object )
 */
typedef void(^OMCSyncResourceChanged) (NSString* uri, id mobileResource);


/**
 * An enumeration of the different resource types.
 *
 * Default is file (ResourceKindFile).
 */
typedef NS_ENUM(int, SyncResourceKind) {
    /** Denotes a file resource. */
    ResourceKindFile = 0,

    /** Denotes an object resource. */
    ResourceKindObject = 1,

     /** Denotes a collection resource. */
    ResourceKindCollection = 2,
    
    /** Denotes an unknown type. */
    ResourceKindUnknown = -1,
};

/**
 * An enumeration of the different HTTP request method types.
 *
 * Default is "Get".
 */
typedef NS_ENUM(int, SyncRequestMethod) {
    /** HTTP method GET */
    RequestMethodGet = 0,
    
    /** HTTP method PUT */
    RequestMethodPut = 1,
    
    /** HTTP method POST */
    RequestMethodPost = 2,
    
    /** HTTP method DELETE */
    RequestMethodDelete = 3,
    
    /** HTTP method PATCH */
    RequestMethodPatch = 4,
};

/**
 * An enumeration of the different Sync network status.
 *
 * Default is "SyncOnline".
 */
typedef NS_ENUM(int, SyncNetworkStatus) {
   
    /** Online state */
    SyncOnline = 0,
    
    /** Offline state */
    SyncOffline = 1,
    
    /** Offline state for test */
    SyncOfflineTest = 2,
};


/**
 * This interface defines global constants to be used by the client.
 */
@interface OMCSyncGlobals : NSObject

@end
