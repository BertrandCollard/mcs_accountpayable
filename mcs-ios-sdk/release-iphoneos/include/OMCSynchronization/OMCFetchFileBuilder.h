//
//  OMCFetchFileBuilder.h
//  OMCSynchronization
//
//  Copyright (c) 2015 Oracle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OMCSyncGlobals.h"
#import "OMCSyncPolicy.h"

@class OMCMobileEndpoint;
@class OMCMobileFile;

/**
 * The callback on fetch success
 @param mobileFile The Mobile File object fetched.
 */
typedef void(^OMCFetchFileSuccess) (OMCMobileFile * mobileFile);

/**
 * Fetch file builder to get OMCMobileFile
 */
@interface OMCFetchFileBuilder : NSObject


/**
 * Gets currently set SyncPolicy
 @return OMCSyncPolicy object, if not set will return default SyncPolicy.
 */
- (OMCSyncPolicy *) getSyncPolicy;

/**
 * Sets SyncPolicy
 @param syncPolicy will set the passed policy as current SyncPolicy
 */
- (void) setSyncPolicy:(OMCSyncPolicy *) syncPolicy;

/**
 * Sets Fetch policy in SyncPolicy as FETCH_POLICY_FETCH_FROM_SERVICE_IF_ONLINE, other policies in SyncPolicy will remain same.
 */
- (void) setSyncPolicyFetchFromServer;

/**
 * Sets extra request headers.
 * No need of Headers for authorization, content-type, as they will be added by default.
 * @param headers Extra request headers
 */
- (void) setRequestHeaders:(NSDictionary *) headers;

/**
 * Execute the Get request, based on Policy set, it will goto Server to Local cache or both.
 @param successBlk block that will be called after request successfully finished with the mobile resource.
 @param errorBlk block that will be called after request finished with error.
 */
- (void) executeFetchOnSuccess:(OMCFetchFileSuccess) successBlk
                       OnError:(OMCSyncErrorBlock) errorBlk;

@end
