//
//  OMCAuthorization.h
//  OMCCore
//
//  Copyright (c) 2015, Oracle Corp. All rights reserved. 
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "OMCCore.h"
@class OMCMobileBackend;
@class OMAuthView;
@class OMCUser;

///---------------------------
/// @name Enumerated types.
///---------------------------

/**
 Authentication type to use.  Depending on the version of
 the product, only certain types are supported.
 */
typedef NS_ENUM(int, OMCAuthenticationType) {
    /**
     * OAuth 2.0 authentication.
     */
    OMCAuthenticationTypeOAuth = 0,
    
    /**
     * HTTP Basic Authentication.
     */
    OMCAuthenticationTypeHTTPBasic = 1,
    
    /**
     * Single sign-on Authentication.
     */
    OMCAuthenticationTypeSSO = 2,
    
    /**
     * Facebook Authentication.
     */
    OMCAuthenticationTypeFacebook = 3,

};

/**
 Authentication mode.  This distinguishes having been authenticated online
 by the server from offline mode, in which the local cached credentials
 are used.
 */
typedef NS_ENUM(int, OMCAuthenticationMode) {
    /**
     * Not authenticated.
     */
    OMCAuthenticationModeNotAuthenticated = 0,
    
    /**
     * Online authentication.
     */
    OMCAuthenticationModeOnline = 1,
    
    /**
     * Offline authentication.
     */
    OMCAuthenticationModeOffline = 2,
};

/**
 Module responsible for handling OAuth 2.0, Basic Authentication, and in the future
 other authentication modes.  API allows for authentication, validity check,
 refresh (if applicable) and logout.  Optionally, the underlying toolkit supplies
 a login dialog to get the user ID and  password.
 
 In the documentation below, the term "authentication" refers to what is appropriate
 for the authentication type in use.  For OAuth, authentication acquires an OAuth
 token, whereas for Basic Authentication, the authentication simply validates that
 the user/password combination is correct.
 
 Note an OMCAuthorization instance is capable of holding only one valid set of
 authentication credentials at a time, so subsequent calls to acquire a token
 will overwrite any previous token that has not been released.  The methods
 in this class are not thread-safe, so the caller must use appropriate
 synchronization methods when the potential for concurrent invocation exists.
 For methods that use asynchronous callbacks, the lifetime of the concurrency
 includes completion of the user callback.
 
 Note to Swift developers: for full visibility to all the API's in this class,
 add the following headers to your Objective-C bridging header:
 OMCUser.h from the Core library (this module), plus OMAuthView.h and
 OMDefinitions.h from the IDM Security Library.  The latter two may be found in
 the "PublicHeaders" folder under the thirdParty distributions folder of the release.
 */
@interface OMCAuthorization : NSObject

#pragma mark - Constants

///---------------------------
/// @name External definitions
///---------------------------

/**
 The default authentication type if none is configured from the Mobile Backend.
 */
extern OMCAuthenticationType OMCAuthorizationDefaultAuthenticationType;


/**
 The authorization's `authenticationType` property name.
 */
extern NSString* const OMCAuthorizationAuthenticationTypePropertyName;

/**
 The authorization's `offlineAuthenticationEnabled` Boolean property name.
 Note the default value is YES for this if omitted.
 */
extern NSString* const OMCAuthorizationOfflineAuthenticationEnabledPropertyName;


/**
 The sub-dictionary name `OAuth` for OAuth properties.  Placed under the
 "authorization" dictionary.
 */
extern NSString* const OMCAuthorizationOAuthPropDictName;

/**
 The sub-dictionary name `Basic` for Basic properties.  Placed under the
 "authorization" dictionary.
 */
extern NSString* const OMCAuthorizationBasicPropDictName;

/**
 The sub-dictionary name `Facebook` for Facebook properties.  Placed under the
 "authorization" dictionary.
 */
extern NSString* const OMCAuthorizationFacebookPropDictName;

/**
 The authorization's `clientID` property name (OAuth only).
 */
extern NSString* const OMCAuthorizationClientIDPropertyName;

/**
 The authorization's `clientSecret` property name (OAuth only).
 */
extern NSString* const OMCAuthorizationClientSecretPropertyName;

/**
 The authorization's `tokenEndpoint` property name (OAuth only).
 */
extern NSString* const OMCAuthorizationTokenEndpointPropertyName;

/**
 The authorization's `logCompleteToken` property name (OAuth only).
 */
extern NSString* const OMCAuthorizationLogCompleteTokenPropertyName;

/**
 The authorization's `identityDomain` property name.
 */
extern NSString* const OMCAuthorizationIdentityDomainPropertyName;

/**
 The authorization's `logCompleteToken` default value (`YES`).
 */
extern BOOL const OMCAuthorizationLogCompleteTokenDefault;

/**
 The authorization's `mobileBackendID` property name (Basic Auth only).
 */
extern NSString* const OMCAuthorizationMobileBackendIDPropertyName;

/**
 The authorization's token string for anonymous authentication (Basic Auth only).
 */
extern NSString* const OMCAuthorizationAnonymousAccessKeyPropertyName;

/**
 The authorization's `facebookAppID` property name (Facebook Auth only).
 */
extern NSString* const OMCAuthorizationFacebookAppIdKeyPropertyName;

/**
 The name of the HTTP OAuth header, i.e. `"Authorization"`.
 */
extern NSString* const OMCAuthorizationOAuthHTTPHeaderName;

/**
 The name of the Basic Auth header, i.e. `"Authorization"`.
 */
extern NSString* const OMCAuthorizationBasicAuthHTTPHeaderName;

/**
 The name of the Basic Auth Mobile Backend ID header,
 i.e. `"Oracle-Mobile-Backend-ID"`.
 */
extern NSString* const OMCAuthorizationBasicBackendHTTPHeaderName;

/**
 The value of the `code` of the `NSError` generated when
 an issue occurs requesting an anonymous OAuth token.
 See the values in the `userInfo` dictionary.
 */
extern NSInteger const OMCAuthorizationAnonymousOAuthTokenRequestError;

/**
 The name of the mobile/social idenitiy provider header,
 i.e. `oracle-mobile-identity-provider'
 */
extern NSString* const OMCAuthorizationMobileIdentityProviderHeaderName;

/**
 The name pf the mobile/social token header,
 i.e. `oracle-mobile-social-token`
 */
extern NSString* const OMCAuthorizationMobileSocialTokenHeaderName;


#pragma mark - Blocks

/**
 * Completion handler for authorization.  If no error, the token may
 * be used.
 */
typedef void (^OMCAuthorizationAuthCompletionBlock)(NSError* error);

/**
 * Completion handler for logout.  If no error, the logout was successful.
 */
typedef void (^OMCAuthorizationLogoutCompletionBlock)(NSError* error);

/**
 Completion handler for async validity check/token refresh.
 */
typedef void (^OMCAuthorizationTokenValidityBlock)(BOOL valid, NSError* error);

/**
 Completion handler used for all the self-registration methods that return a user.
 */
typedef void (^OMCUserRegistrationCompletionBlockWithUser)(NSError* error, OMCUser* user);

/**
 Completion handler used for all the self-registration methods that return an HTTP status only.
 */
typedef void (^OMCUserRegistrationCompletionBlock)(NSError* error);

#pragma mark - Properties

/**
 The string representing the authentication token.  This could be an
 OAuth token. For basic Authentication, only the name of the logged in
 user is returned so as to not reveal the password.
 */
@property(readonly, strong, nonatomic) NSString *accessToken;

/**
 * The user name of the authenticated user.  Returns `nil`
 * if user is not authenticated or if client credentials
 * OAUth Type (anonymous) was used.
 */
@property(readonly, strong, nonatomic) NSString *userName;

/**
 Whether authorization has been performed.  This could be acquisition
 of an OAuth token, or validation of a user/password combination.
 */
@property(readonly, nonatomic) BOOL authorized;

/**
 Property indicating current authentication type from
 enumeration OMCAuthenticationType.  Note if the server
 instance supports multiple authentication types, the user
 may switch the authentication type in use at runtime.
 */
@property(readwrite, nonatomic) OMCAuthenticationType authenticationType;

/**
 Property indicating the current authentication mode.  See enumeration
 for possible values.
 */
@property(readonly, nonatomic) OMCAuthenticationMode authenticationMode;

/**
 Property indicating whether the user may authenticate offline.  If this
 mode is enabled, the user may authenticate against their cached credentials
 when the server-side is not reachable through the network.
 
 Note the user must have previously authenticated online and then have not
 logged out to keep the credentials available for offline authentication.
 
 The default setting for this property is YES.
 */
@property(readwrite, nonatomic) BOOL offlineAuthenticationEnabled;


#pragma mark - Authentication

/**
 Returns whether a given authentication type is configured for this
 object.  Based on the OMCMobileBackend settings, one or more authentication
 types may be configured.

 @param type The authentication type to be checked for.
 */
-(BOOL) isAuthenticationConfiguredForType: (OMCAuthenticationType) type;

/**
 Performs the handshake to do the authentication with the supplied user
 name and password.  Any required credentials beyond user and password
 are gotten from the Mobile Backend.  This method does not require a UI for use.
 
 This API may be used for social login (e.g. Facebook) but the user name and
 password will be ignored.  Equivalently, athenticateSocial may be invoked
 for this case.
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.
 @param userName The user name of the person acquiring the token.
 @param password The password of the person acquiring the token.
 @param completionBlock The handler invoked after the handshake has completed.
 */
-(void) authenticate:(NSString *)userName
            password:(NSString *)password
     completionBlock: (OMCAuthorizationAuthCompletionBlock) completionBlock;

/**
 Perform the handshake to do the authentication for the anonymous user.  No
 user or password is required.  For OAuth, The client ID and secret are gotten
 from the Mobile Backend.  This method does not require a UI for use.
 
 Anonymous authentication is available from social login mode, altohugh
 currently only Basic Authentication is supported.
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.
 @param completionBlock the handler invoked after the handshake has completed
 */
-(void) authenticateAnonymous: (OMCAuthorizationAuthCompletionBlock) completionBlock;


/**
 Gets the authentication token or logs in the user synchronously with
 the supplied user name and password.  Any additional required credentials
 are gotten from the Mobile Backend.  This method does not require a UI
 for use and should not be invoked from a UI thread.  This method is
 primarily intended for testing and its use is not recommended in production code.
 
 This API may be used for social login (e.g. Facebook) but the user name and
 password will be ignored.  Equivalently, athenticateSocial may be invoked
 for this case.
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.
 @param userName The user name of the person acquiring the token.
 @param password The password of the person acquiring the token.
 @return An error if one occurred, else `nil`.
 */
-(NSError *) authenticate:(NSString *)userName
                 password:(NSString *)password;

/**
 Does the authentication synchronously for the anonymous login.
 No user or password is required.  Any additional required credentials are
 gotten from the Mobile Backend.  This method does not require a UI for use
 and should not be  invoked from a UI thread.  This method is primarily intended
 for testing and its use is not recommended in production code.
 
 Anonymous authentication is available from social login mode, altohugh
 currently only Basic Authentication is supported.
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.
 @return An error if one occurred, else `nil`.
 */
-(NSError *) authenticateAnonymous;

/**
 Acquire the authentication credentials for a mobile social site asynchronously.
 Currently the only social app supported is Facebook.  The user config onfig file
 must be set up for Facebook-style authentication.
 @param completionBlock the handler invoked after the handshake has completed
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.
 */
-(void) authenticateSocial: (OMCAuthorizationAuthCompletionBlock) completionBlock;

/**
 Acquire the authentication credentials for a mobile social site synchronously.
 Currently the only social app supported is Facebook.  The user config onfig file
 must be set up for Facebook-style authentication.  This method is primarily
 intended for testing and its use is not recommended in production code.
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.

*/
-(NSError*) authenticateSocial;

/**
 Acquire the authentication credentials using federated single sign-on.  The
 The user config file must be set up for SSO-style authentication.
 
 @param presentingViewController - a view contoller upon which the SSO web browser
 view will be presented.
 @param clearCookies - if set, clears all cookies related to authentication for this app
 @param completionBlock - the handler invoked after the token acquisition attempt
 has completed.  Note this incldues a boolean value to indicate whether the user has
 cancelled from the web browser.
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.
 */
-(void) authenticateSSO: (UIViewController*) presentingViewController
           clearCookies: (BOOL) clearCookies
        completionBlock:(OMCAuthorizationAuthCompletionBlock) completionBlock;

/**
 Acquire the authentication credentials using federated single sign-on synchronously.
 The user config file must be set up for SSO-style authentication.  This method is
 primarily intended for testing and its use is not recommended in production code.
 
 @param presentingViewController - a view contoller upon which the SSO web browser
 view will be presented.
 @param clearCookies - if set, clears all cookies related to authentication for this app
 
 Note: This method and all the other "authenticate" methods are not thread-safe,
 so the caller must use appropriate synchronization methods when the potential for
 concurrent invocation exists. The lifetime of the concurrency includes completion
 of the asynchronous user callback.
 */
-(NSError*) authenticateSSO: (UIViewController*) presentingViewController
               clearCookies: (BOOL) clearCookies;


/**
 Sets the proper HTTP Authorization headers for an authorization-enabled request on the
 specified request.  The exact headers depend on the authentication type in use.
 
 @param request The request object to set the header on.
 @see setHTTPHeadersOnRequest:completionHandler:
 @see -[OMCMobileBackend setHTTPHeadersOnRequest:]
 */
-(void) setHTTPHeadersOnRequest: (NSMutableURLRequest *) request;

/**
 Adds the proper HTTP Authorization headers for an authorization-enabled request to the
 specified request.  The exact headers depend on the authentication type in use.
 
 @param request The request object to add the header to.
 @warning This method is deprecated. Use `setHTTPHeadersOnRequest:`.
 @see setHTTPHeadersOnRequest:
 @see -[OMCMobileBackend addHTTPHeadersToRequest:]
 */
-(void) addHTTPHeadersToRequest: (NSMutableURLRequest *) request
               __deprecated_msg("use 'setHTTPHeadersOnRequest:' instead");

/**
 Sets the proper HTTP Authorization headers for an authorization-enabled request on the
 specified request.  The exact headers depend on the authentication type in use.
 
 @param request The request object to set the header on.
 @param completionHandler The completion block invoked once the receiver's
 HTTP headers have been set on the URL request or an error occurs.
 @see setHTTPHeadersOnRequest:
 @see -[OMCMobileBackend setHTTPHeadersOnRequest:completionHandler:]
 */
-(void) setHTTPHeadersOnRequest: (NSMutableURLRequest *) request
              completionHandler: (OMCErrorCompletionBlock) completionHandler;

/**
 Adds the proper HTTP Authorization headers for an authorization-enabled request to the
 specified request.  The exact headers depend on the authentication type in use.
 
 @param request The request object to add the header to.
 @param completionHandler The completion block invoked once the receiver's
 HTTP headers have been added to the URL request or an error occurs.
 @warning This method is deprecated. Use `setHTTPHeadersOnRequest:completionHandler:`.
 @see setHTTPHeadersOnRequest:completionHandler:
 @see -[OMCMobileBackend addHTTPHeadersToRequest:completionHandler:]
 */
-(void) addHTTPHeadersToRequest: (NSMutableURLRequest *) request
              completionHandler: (OMCErrorCompletionBlock) completionHandler
               __deprecated_msg("use 'setHTTPHeadersOnRequest:completionHandler:' instead");



/**
 Returns whether the current token is valid (non-expired) and
 optionally refreshes the token if expired if the authentication
 protocol supports refresh.
 
 Note: in the current implementation, OAuth token refresh is not supported by the
 server, so the the "refresh" will actually be a new token acquisition,
 provided the credentials have been cached.
 
 For Facebook authentication, token refresh is unconditional, that is, a new
 token will be acquired regardless of expiry and the "refresh" parameter value
 is ignored.  Note these are long-lived tokens (60 days), so this method should
 only be called occasionally, or in repsonse to receiving an HTTP 401.
 

 For certain authentication types, such as Basic Authentication, where refresh
 is not supported, the "token" is taken to mean the "current login session", so
 a return of YES indicates the user is currently logged in.  Also for Basic
 Authentication, a return of NO indicates the refresh must be accomplished
 manually by invoking the authorization API, as the "refresh" parameter is ignored.
 @param refresh Whether to refresh the token (ignored for OAuth).
 @return Whether the token was valid.
 */
-(BOOL) validateToken: (BOOL) refresh;

/**
 Returns asynchronously whether the current token is valid
 (non-expired) and refreshes the token if expired.
 
 For Basic Authentication, the "token" is taken to mean the "current login
 session", so if the token is not valid, the authentication sequence must
 be invoked by the caller as no refresh protocol is available for Basic Authentication.
 
 Note: In the current implementation, OAuth token refresh is not supported by the
 server, so the the "refresh" will actually be a new token acquisition,
 provided the credentials have been cached.
 
 For Facebook authentication, token refresh is unconditional, that is, a new
 token will be acquired regardless of expiry.  Note these are long-lived tokens
 (60 days), so this method should only be called occasionally, or in repsonse
 to receiving an HTTP 401.
 
 For Facebook authentication, token refresh is unconditional, that is, a new
 token will be acquired regardless of expiry.  Note these are long-lived tokens
 (60 days), so this method should only be called occasionally, or in repsonse
 to receiving an HTTP 401.
 @param completionBlock the handler block called to report the results.
 If valid is YES, the token was valid or has successfully been refreshed;
 otherwise, the entire authentication sequence should be reinitiated.
 
 */
-(void) refreshTokenAsync: (OMCAuthorizationTokenValidityBlock) completionBlock;

/**
 Logs the user out and revokes the authentication credentials (if applicable and supported).
 Clears any local cached credentials.
 
 @param completionBlock The handler invoked after the logout attempt has completed.
 */
-(void) logout: (OMCAuthorizationLogoutCompletionBlock) completionBlock;

/**
 Synchronously logs the user out and revokes the authentication credentials
 (if applicable and supported).
 Clears any local cached credentials.
 @return An error if one occurred, else `nil`.
 */
- (NSError*)logout;

/**
 Logs the user out and optionally revokes the authentication credentials (if applicable and supported).
 Clears any local cached credentials.
 
 @param clearCredentials whether or not to clear the cached credentials
 @param completionBlock the handler invoked after the logout attempt has completed
 */
-(void) logoutClearCredentials: (BOOL) clearCredentials
               completionBlock: (OMCAuthorizationLogoutCompletionBlock) completionBlock;

/**
 Synchronously logs the user out and revokes the authentication credentials
 (if applicable and supported).
 Clears any local cached credentials.

 @param clearCredentials whether or not to clear the cached credentials
 @return An error if one occurred, else `nil`.
 */
- (NSError*)logoutClearCredentials: (BOOL) clearCredentials;

#pragma mark - User

/**
 Gets the properties for the currently authorized user.  This is an asynchronous
 method where the success status is returned in the completion block.
 @param completionBlock The completion handler called upon return from the server
 */
-(void) getCurrentUser: (OMCUserRegistrationCompletionBlockWithUser) completionBlock;

/**
 Updates the currently authorized user's UMS properties.  This is an asynchronous
 method where the success status is returned in the completion block.
 @param user The user object to set as the current user attributes.
 @param completionBlock The completion handler called upon return from the server.
 */
-(void) updateCurrentUser: (OMCUser*) user
       completionCallback: (OMCUserRegistrationCompletionBlock) completionBlock;

@end
