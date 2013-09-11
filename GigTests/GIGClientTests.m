//
//  GIGClientTests.m
//  Gig
//
//  Created by guille on 11/09/13.
//  Copyright (c) 2013 Guillermo Gonzalez. All rights reserved.
//

@interface GIGClientTests : SenTestCase

@property (strong, nonatomic) GIGClient *client;

@end

@implementation GIGClientTests

- (void)setUp {
    [super setUp];

    self.client = [[GIGClient alloc] initWithAccount:nil defaultParameters:nil];
    self.client.serviceType = SLServiceTypeTwitter;
}

- (void)tearDown {
    self.client = nil;
    [super tearDown];
}

- (void)testConstants {
    STAssertEqualObjects(GIGMentionsTimeline, @"mentions_timeline", nil);
    STAssertEqualObjects(GIGUserTimeline, @"user_timeline", nil);
    STAssertEqualObjects(GIGHomeTimeline, @"home_timeline", nil);
    STAssertEqualObjects(GIGRetweetsOfMe, @"retweets_of_me", nil);

    STAssertEqualObjects(GIGCountKey, @"count", nil);
    STAssertEqualObjects(GIGSinceIDKey, @"since_id", nil);
    STAssertEqualObjects(GIGMaxIDKey, @"max_id", nil);
    STAssertEqualObjects(GIGTrimUserKey, @"trim_user", nil);
    STAssertEqualObjects(GIGContributorDetailsKey, @"contributor_details", nil);
    STAssertEqualObjects(GIGIncludeEntitiesKey, @"include_entities", nil);
}

- (void)testFetchTimeline {
    id requestHandler = [self addRequestHandlerForURL:[NSURL URLWithString:@"https://api.twitter.com/1.1/statuses/home_timeline.json?count=2"]
                                     responseFilename:@"home_timeline.json"];

    NSArray * __block blockTweets = nil;
    NSError * __block blockError = nil;
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);

    OVCRequestOperation *operation = [self.client fetchTimeline:GIGHomeTimeline parameters:@{
            GIGCountKey : @2
    } completion:^(NSArray *tweets, NSError *error) {
        blockTweets = tweets;
        blockError = error;
        dispatch_semaphore_signal(semaphore);
    }];

    BOOL timeout = [self waitForSemaphore:semaphore timeout:5];
    STAssertFalse(timeout, @"Timeout waiting for processing queue");

    STAssertNotNil(operation, nil);
    STAssertNil(blockError, nil);

    STAssertEquals(blockTweets.count, (NSUInteger)2, nil);
    STAssertTrue([blockTweets[0] isKindOfClass:GIGTweet.class], nil);

    [OHHTTPStubs removeRequestHandler:requestHandler];
}

- (id)addRequestHandlerForURL:(NSURL *)url responseFilename:(NSString *)filename {
    return [OHHTTPStubs addRequestHandler:^OHHTTPStubsResponse *(NSURLRequest *request, BOOL onlyCheck) {
        if (![request.URL isEqual:url]) return nil;

        NSURL *fileURL = [[NSBundle bundleForClass:self.class] URLForResource:filename.stringByDeletingPathExtension withExtension:filename.pathExtension];
        return [OHHTTPStubsResponse responseWithFileURL:fileURL contentType:@"application/json" responseTime:0];
    }];
}

- (BOOL)waitForSemaphore:(dispatch_semaphore_t)semaphore timeout:(NSTimeInterval)timeout {
    NSDate *date = [NSDate dateWithTimeIntervalSinceNow:timeout];

    while (dispatch_semaphore_wait(semaphore, DISPATCH_TIME_NOW)) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:date];

        if ([date timeIntervalSinceNow] < 0.0) {
            return YES;
        }
    }

    return NO;
}

@end