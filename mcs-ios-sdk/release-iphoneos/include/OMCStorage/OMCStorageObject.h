//
//  OMCStorageObject.h
//  OMCStorage
//
//  Copyright (c) 2015, Oracle Corp. All rights reserved. 
//

#import <Foundation/Foundation.h>

/**
 This interface manages the contents and metadata of a StorageObject.
 */

@interface OMCStorageObject : NSObject

/**
 Identification of the `OMCStorageObject`
 should be unique across the OMCStorageCollection.
 */
@property(readwrite, nonatomic) NSString* ID;


/**
 Display Name of the `OMCStorageObject`.
 */
@property(readwrite, nonatomic) NSString* displayName;

/**
 Content-Type of the payload in the `OMCStorageObject`.
 */
@property(readwrite, nonatomic) NSString* contentType;

/**
 The property `createdBy`.
 */
@property(readonly, nonatomic) NSString* createdBy;

/**
 The property `modifiedBy`. Might be NULL if the object is never modified.
 */
@property(readonly, nonatomic) NSString* modifiedBy;

/**
 The property `createdOn` in the date format "yyyy-MM-dd'T'HH:mm:ss'Z'".
 */
@property(readonly, nonatomic) NSDate* createdOn;

/**
 The property `modifiedOn` in the date format "yyyy-MM-dd'T'HH:mm:ss'Z'".
 Might be NULL if the object is never modified.
 */
@property(readonly, nonatomic) NSDate* modifiedOn;

/**
 The property `canonicalLink` is the canonical link for the current Storage Object.
 The link is a relative url.
 */
@property(readonly, nonatomic) NSString* canonicalLink;


/**
 Initializes an `OMCStorageObject` object with NSData format payload.
 @param objectID The ID of the object in the MOS service.
 @param data The payload of the current object in the format of NSData.
 @param contentType The content-Type of the payload.
 */
-(instancetype) initPayload: (NSString*) objectID
                   withData: (NSData*) data
             andContentType: (NSString*) contentType;

/**
 Initializes an `OMCStorageObject` object with NSInputStream format payload.
 @param objectID The ID of the object in the MOS service.
 @param inputStream The payload of the current object in the format of NSInputStream.
 @param contentType The content-Type of the payload.
 */
-(instancetype) initPayload: (NSString*) objectID
                 withStream: (NSInputStream*) inputStream
             andContentType: (NSString*) contentType;


/**
 Returns the payload as NSData.
 */
-(NSData*) getPayloadData;

/**
 Returns the payload as NSInputStream.
 */
-(NSInputStream*) getPayloadStream;

/**
 Sets the payload and content-Type of the `OMCStorageObject`.
 Returns the updated `OMCStorageObject`.

 @param data is the new payload
 @param contentType The content-Type of the payload.
 */
-(OMCStorageObject*) setPayloadFromData: (NSData*) data
                        withContentType: (NSString*) contentType;
/**
 Sets the payload and content-Type of the `OMCStorageObject`.
 Returns the updated `OMCStorageObject`.
 
 @param inputStream The new payload.
 @param contentType The content-Type of the payload.
 */

-(OMCStorageObject*) setPayloadFromStream: (NSInputStream*) inputStream
                          withContentType: (NSString*) contentType;


@end
