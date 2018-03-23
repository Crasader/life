//
//  iOSFuncs.h
//  Lippi
//
//  Created by ff on 16/12/7.
//
//

#ifndef iOSFuncs_h
#define iOSFuncs_h

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SCNetworkReachability.h>
@interface iOSFuncs : NSObject

- (BOOL)connectedToNetwork;

@end

#endif /* iOSFuncs_h */
