//
//  APObject.m
//  Vinci AP
//
//  Created by Bertrand Collard on 15/03/2016.
//  Copyright © 2016 Bertrand Collard. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "APObject.h"

static NSDateFormatter *dateFormatter = NULL;

@implementation APObject

@synthesize status = _status, invoiceId , numberAp, dateAp, numberPo,datePo, supplier,  siret, tvaCode,totalAmount,base64Image ;

- (NSString*) convertStatusToString {
    NSString *result = nil;
    
    switch([_status integerValue]) {
        case 0:
            result = @"Pending";
            break;
        case 1:
            result = @"Accepted";
            break;
        case 2:
            result = @"Refused";
            break;
            
        default:
            result = @"unknown";
    }
    
    return result;
}

__attribute__((constructor))
static void initialize_dateFormatter(){
    dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd'T'HH:mm:ss'Z'"];
}

- (NSDate*) getDateApRef{
    
    NSDate *date = [dateFormatter dateFromString:dateAp];
    return date;
}

- (void) setDateApRef: (NSDate*) value{
    dateAp = [dateFormatter stringFromDate:value];
}

- (NSDate*) getDatePoRef{
    NSDate *date = [dateFormatter dateFromString:datePo];
    return date;
}

- (void) setDatePoRef: (NSDate*) value{
    datePo = [dateFormatter stringFromDate:value];
}

@end