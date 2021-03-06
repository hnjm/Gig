// GIGClient.h
//
// Copyright (c) 2013 Guillermo Gonzalez
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Overcoat/Overcoat.h>

@class GIGTweet;
@class GIGUserIDCollection;

extern NSString * const GIGCountKey;
extern NSString * const GIGSinceIDKey;
extern NSString * const GIGMaxIDKey;
extern NSString * const GIGTrimUserKey;
extern NSString * const GIGIncludeEntitiesKey;
extern NSString * const GIGIncludeUserEntitiesKey;
extern NSString * const GIGUserIDKey;
extern NSString * const GIGScreenNameKey;
extern NSString * const GIGExcludeRepliesKey;
extern NSString * const GIGIncludeRetweetsKey;
extern NSString * const GIGInReplyToStatusIDKey;
extern NSString * const GIGLatitudeKey;
extern NSString * const GIGLongitudeKey;
extern NSString * const GIGPlaceIDKey;
extern NSString * const GIGDisplayCoordinatesKey;

// Twitter API client.
@interface GIGClient : OVCSocialClient

// Initializes a `GIGClient`with the specified account.
//
// account - The user account that will be used to sign each request.
- (id)initWithAccount:(ACAccount *)account;

@end

// The different timelines that can be found on Twitter.
//
// GIGTimelineMentions     - Mentions for the authenticating user.
// GIGTimelineUser         - Tweets posted by a specific user.
// GIGTimelineHome         - Tweets and retweets posted by the authenticating user and the users they follow.
// GIGTimelineRetweetsOfMe - Tweets authored by the authenticating user that have been retweeted by others.
typedef NS_ENUM(NSInteger, GIGTimeline) {
    GIGTimelineMentions,
    GIGTimelineUser,
    GIGTimelineHome,
    GIGTimelineRetweetsOfMe
};

@interface GIGClient (Timelines)

// Fetches a collection of tweets, ordered with the most recent first.
//
// timeline   - The type of timeline. See `GIGTimeline`.
// parameters - The parameters for the request. See https://dev.twitter.com/docs/api/1.1.
// completion - A block to be executed when the operation finishes.
//
// See https://dev.twitter.com/docs/working-with-timelines
- (OVCRequestOperation *)fetchTimeline:(GIGTimeline)timeline
                            parameters:(NSDictionary *)parameters
                            completion:(void (^)(NSArray *tweets, NSError *error))completion;

@end

@interface GIGClient (Tweets)

// Fetches a collection of the 100 most recent retweets of the specified tweet.
//
// statusID   - The ID of the desired tweet.
// parameters - The parameters for the request.
// completion - A block to be executed when the operation finishes.
//
// See https://dev.twitter.com/docs/api/1.1/get/statuses/retweets/%3Aid
- (OVCRequestOperation *)fetchRetweetsForStatus:(NSNumber *)statusID
                                     parameters:(NSDictionary *)parameters
                                     completion:(void (^)(NSArray *tweets, NSError *error))completion;

// Fetches a single tweet.
//
// statusID   - The ID of the desired tweet.
// parameters - The parameters for the request.
// completion - A block to be executed when the operation finishes.
//
// See https://dev.twitter.com/docs/api/1.1/get/statuses/show/%3Aid
- (OVCRequestOperation *)fetchStatus:(NSNumber *)statusID
                          parameters:(NSDictionary *)parameters
                          completion:(void (^)(GIGTweet *tweet, NSError *error))completion;

// Deletes the specified tweet. The authenticating user must be the author of the specified status.
//
// statusID   - The ID of the desired tweet.
// parameters - The parameters for the request.
// completion - A block to be executed when the operation finishes.
//
// See https://dev.twitter.com/docs/api/1.1/get/statuses/show/%3Aid
- (OVCRequestOperation *)removeStatus:(NSNumber *)statusID
                            parameters:(NSDictionary *)parameters
                            completion:(void (^)(GIGTweet *tweet, NSError *error))completion;

// Updates the authenticating user's current status, also known as tweeting.
//
// text       - The text of your status update, typically up to 140 characters.
// parameters - The parameters for the request.
// completion - A block to be executed when the operation finishes.
//
// See https://dev.twitter.com/docs/api/1.1/post/statuses/update
- (OVCRequestOperation *)updateStatusWithText:(NSString *)text
                                   parameters:(NSDictionary *)parameters
                                   completion:(void (^)(GIGTweet *tweet, NSError *error))completion;

// Retweets a tweet. Returns the original tweet with retweet details embedded.
//
// statusID   - The ID of the desired tweet.
// parameters - The parameters for the request.
// completion - A block to be executed when the operation finishes.
//
// See https://dev.twitter.com/docs/api/1.1/post/statuses/retweet/%3Aid
- (OVCRequestOperation *)retweetStatus:(NSNumber *)statusID
                            parameters:(NSDictionary *)parameters
                            completion:(void (^)(GIGTweet *tweet, NSError *error))completion;

// Updates the authenticating user's current status and attaches media for upload. In other words,
// it creates a Tweet with a picture attached.
//
// text       - The text of your status update, typically up to 140 characters.
// media      -
// parameters - The parameters for the request.
// completion - A block to be executed when the operation finishes.
//
// See https://dev.twitter.com/docs/api/1.1/post/statuses/update
- (OVCRequestOperation *)updateStatusWithText:(NSString *)text
                                        media:(NSData *)media
                                   parameters:(NSDictionary *)parameters
                                   completion:(void (^)(GIGTweet *tweet, NSError *error))completion;

// Returns a collection of up to 100 user IDs belonging to users who have retweeted the specified tweet.
//
// statusID   - The ID of the desired tweet.
// parameters - The parameters for the request.
// completion - A block to be executed when the operation finishes.
//
// https://dev.twitter.com/docs/api/1.1/get/statuses/retweeters/ids
- (OVCRequestOperation *)fetchRetweetersForStatus:(NSNumber *)statusID
                                       parameters:(NSDictionary *)parameters
                                       completion:(void (^)(GIGUserIDCollection *collection, NSError *error))completion;

@end
