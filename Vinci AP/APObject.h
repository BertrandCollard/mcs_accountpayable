//
//  APObject.h
//  Vinci AP
//
//  Created by Bertrand Collard on 15/03/2016.
//  Copyright © 2016 Bertrand Collard. All rights reserved.
//

#ifndef APObject_h
#define APObject_h

#import <Foundation/Foundation.h>
#import "OMCMobileObject.h"

typedef enum {
    Pending = 0,
    Accepted = 1,
    Refused = 2
} APStatus;


@interface APObject : OMCMobileObject
// Properties
@property (nonatomic, readonly) NSNumber* invoiceId;
@property (nonatomic, readonly) NSString* numberAp;
@property (nonatomic, readonly) NSString* dateAp;
@property (nonatomic, readonly) NSString* numberPo;
@property (nonatomic, readonly) NSString* datePo;
@property (nonatomic, readonly) NSString* supplier;
@property (nonatomic, readonly) NSNumber* siret;
@property (nonatomic, readonly) NSString* tvaCode;
@property (nonatomic, readonly) NSNumber* totalAmount;

@property (nonatomic, readonly) NSString* base64Image;
@property (nonatomic, retain) NSNumber* status;

- (NSString*) convertStatusToString;

- (NSDate*) getDateApRef;
- (void) setDateApRef: (NSDate*) value;

- (NSDate*) getDatePoRef;
- (void) setDatePoRef: (NSDate*) value;

@end


#endif /* APObject_h */
