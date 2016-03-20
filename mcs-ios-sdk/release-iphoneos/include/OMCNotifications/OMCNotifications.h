//
//  OMCNotifications.h
//  OMCNotifications
//
//  Copyright (c) 2015, Oracle Corp. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OMCCore/OMCServiceProxy.h>

/**
 The format of the completion block called on success.
 @param response The HTTP response object.
 */
typedef void(^OMC_Notifications_SuccessBlock)(NSHTTPURLResponse *response);

/**
 The format of the completion block called on error.
 @param error The standard error object.
 */
typedef void(^OMC_Notifications_ErrorBlock)(NSError* error);

/**
 * This class is used to both register for and send push notifications.
 */
@interface OMCNotifications : OMCServiceProxy

/**
 Registers the device token.
 @param deviceToken Device token of iOS device.
 @param successCallback (optional) The block to be called on success.
 @param errorCallback (optional) The block to be called on error.
 */
- (void) registerForNotifications:(NSData*) deviceToken
                        onSuccess:(OMC_Notifications_SuccessBlock) successCallback
                          onError:(OMC_Notifications_ErrorBlock) errorCallback;

/**
 De-registers the device token.
 @param deviceToken Device token of iOS device.
 @param successCallback (optional) The block to be called on success.
 @param errorCallback (optional) The block to be called on error.
 */
- (void) deregisterForNotifications:(NSData*) deviceToken
                          onSuccess:(OMC_Notifications_SuccessBlock) successCallback
                            onError:(OMC_Notifications_ErrorBlock) errorCallback;

@end


/**
 The Notifications library's current version.
 */
extern NSString* const OMCNotificationsVersion;
