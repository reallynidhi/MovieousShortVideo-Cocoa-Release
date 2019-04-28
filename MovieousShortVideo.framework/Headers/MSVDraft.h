//
//  MSVDraft.h
//  MovieousShortVideo
//
//  Created by Chris Wang on 2018/10/5.
//  Copyright © 2018 Movieous Team. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import "MSVTypeDefines.h"
#import "MSVMainTrackClip.h"
#import "MSVClip.h"
#import "MSVLUTFilterEditorEffect.h"
#import "MSVBasicEditorEffect.h"
#import "MSVTimeEditorEffect.h"
#import "MSVMixTrackClip.h"
#import "MSVImageGenerator.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Video drafts, callers can generate drafts themselves or edit them arbitrarily with drafts，Then use MSVEditor to generate a preview of the draft in real time, or you can use MSVExporter to export the draft.
 */
@interface MSVDraft : NSObject
<
NSCopying
>

/**
 * Initialize a draft using the audio and video file address, and the initial draft will contain an address that the MSVMainTrackClip points to.
 *
 * @param URL Audio and video file address.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return Initialized successful draft object
 */
+ (instancetype)draftWithAVURL:(NSURL *)URL error:(NSError **)outError;

/**
 * Create a draft object in using the image file address, and the initial draft will contain an address that the MSVMainTrackClip points to.
 *
 * @param URL Image file address.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If the creation is successful, the draft object is returned, otherwise returns nil
 */
+ (instancetype)draftWithImageURL:(NSURL *)URL error:(NSError **)outError;

/**
 * Initialize the draft object.
 *
 * @param mainTrackClipType The Main track segment type.
 * @param URL The Main track segment address.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If the initialization is successful, the draft object is returned, otherwise returns nil.
 */
- (instancetype)initWithMainTrackClipType:(MSVClipType)mainTrackClipType URL:(NSURL *)URL error:(NSError **)outError;

/**
 * Indicate the background color for video display, if video has not cover the whole space whose size is assigned by the videoSize property.
 */
@property (nonatomic, strong, readonly) UIColor *backgroundColor;

/**
 * Update the background color for video display.
 *
 * @param backgroundColor The new background color.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If operation succeeded, return YES, otherwise, return NO.
 */
- (BOOL)updateBackgroundColor:(UIColor *)backgroundColor error:(NSError **)outError;

/**
 * The main track clip array, the main track clip is arranged in the order of the duration specified by its durationAtMainTrack to form the main track of the video, and the length of the draft is determined by the length of the main track.
 */
@property (nonatomic, strong, readonly) NSArray<MSVMainTrackClip *> *mainTrackClips;

/**
 * Update the main track clip set. Non-Readonly parameters can be adjusted directly in related objects. Other operations (add, delete, replace, etc.) need to be refreshed using this interface.
 *
 * @param mainTrackClips The new main track clip set.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return Valid operation returns YES, invalid operation returns NO.
 */
- (BOOL)updateMainTrackClips:(NSArray<MSVMainTrackClip *> *)mainTrackClips error:(NSError **)outError;

/**
 * The mix track clip array, the visual part of the video is generated by mixing the video in mainTrackClips and mixTrackClips according to the location of these clips.
 */
@property (nonatomic, strong, readonly) NSArray<MSVMixTrackClip *> *mixTrackClips;

/**
 * Update the mix track clips. Non-Readonly parameters can be adjusted directly in related objects. Other operations (add, delete, replace, etc.) need to be refreshed using this interface.
 *
 * @param mixTrackClips The new mixTrackClips.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return Valid operation returns YES, invalid operation returns NO.
 */
- (BOOL)updateMixTrackClips:(NSArray<MSVMixTrackClip *> *)mixTrackClips error:(NSError **)outError;

/**
 * Basic effects array，so far we support MSVExternalFilterEditorEffect, MSVLUTFilterEditorEffect, MSVImageEffect, all of these effects will be used on the final video.
 */
@property (nonatomic, strong, readonly) NSArray<id<MSVBasicEditorEffect>> *basicEffects;

/**
 * Update the basic effect sets.
 *
 * @param basicEffects the new effect sets.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If operation succeeded, return YES, otherwise, return NO.
 */
- (BOOL)updateBasicEffects:(NSArray<id<MSVBasicEditorEffect>> *)basicEffects error:(NSError **)outError;

/**
 * Time effects array，so far we support MSVRepeatEditorEffect, MSVSpeedEditorEffect
 */
@property (nonatomic, strong, readonly) NSArray<id<MSVTimeEditorEffect>> *timeEffects;

/**
 * Update effect sets.
 *
 * @param timeEffects the new time effect sets.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If operation succeeded, return YES, otherwise, return NO.
 */
- (BOOL)updateTimeEffects:(NSArray<id<MSVTimeEditorEffect>> *)timeEffects error:(NSError **)outError;

/**
 * The original total duration of the draft (regardless of time effects, regardless of timeRange).
 */
@property (nonatomic, assign, readonly) NSTimeInterval originalDuration;

/**
 * Draft duration in total (take time effects into account, regardless of timeRange).
 */
@property (nonatomic, assign, readonly) NSTimeInterval duration;

/**
 * Whether to reverse video part of the draft, audio part will never be reversed.
 */
@property (nonatomic, assign) BOOL reverseVideo;

/**
 * The size of the video.
 */
@property (nonatomic, assign, readonly) CGSize videoSize;

/**
 * Set the size of the video, if the size of the window does not match the size of the videoSize when previewing, Scaling depends on method specified by editor.previewScalingMode.
 *
 * @param videoSize The new size of the video.
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If operation succeeded, return YES, otherwise, return NO.
 */
- (BOOL)setVideoSize:(CGSize)videoSize error:(NSError **)outError;

/**
 * Valid time area, other parts will be ignored.
 *
 * @warning The time interval here is the interval after the time effect is applied, if timeRange was setup by callers, after the main track was relevantly operated like added, deleted, or time effects, the timeRange should be adjusted to ensure that the correct time range is applied.
 */
@property (nonatomic, assign) MovieousTimeRange timeRange;

/**
 * Get the image generator object used for generating snapshots.
 *
 * @return The instantiated MSVImageGenerator object.
 */
@property (nonatomic, strong) MSVImageGenerator *imageGenerator;

/**
 * The operation of updating the volume is performed in real time. When you need to modify the volume of many clips in batches, please call this method to improve the efficiency of the operation,then make the relevant volume modification, and finally call the -commitVolumeChange method to submit the changes.If you do not follow this paradigm, an update operation will be triggered each time when you modify volume, which will result in performance problems.
 */
- (void)beginVolumeChangeTransaction;

/**
 * Submit volume update.
 *
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If operation succeeded, return YES, otherwise, return NO.
 */
- (BOOL)commitVolumeChangeWithError:(NSError **)outError;

/**
 * In addition to the volume updating, you can call this method to start a transaction when you need to manipulate the related properties in batches, and then call the -commitChangeWithError: method to commit all changes after all the operations are completed.
 */
- (void)beginChangeTransaction;

/**
 * Cancel current batch update transaction.
 */
- (void)cancelChangeTransaction;

/**
 * Submit an update to the general properties.
 *
 * @param outError If an error occurs, return the error that occurred.
 *
 * @return If operation succeeded, return YES, otherwise, return NO.
 */
- (BOOL)commitChangeWithError:(NSError **)outError;

/**
 * Tool method, applying time effects to the original time point, mapping to the time point after applying the time effect.
 *
 * @param originalTime Time point when no time effects are applied.
 *
 * @return Time point after applying time effects.
 */
- (NSTimeInterval)getEffectedTimeFromOriginalTime:(NSTimeInterval)originalTime;

/**
 * Tool method, which maps the time point when the time effect has been applied to the time point when the time effect is removed.
 *
 * @param effectedTime Time point when the time effect has been applied.
 *
 * @return Time point after removing the time effect.
 */
- (NSTimeInterval)getOriginalTimeFromEffectedTime:(NSTimeInterval)effectedTime;

/**
 * Tool method, applying time effects to the original time interval, mapping to the time interval after applying the time effect.
 *
 * @param originalTimeRange Time interval when no time effects are applied.
 *
 * @return Time interval after applying time effects.
 */
- (MovieousTimeRange)getEffectedRangeTimeFromOriginalTimeRange:(MovieousTimeRange)originalTimeRange;

/**
 * Tool method that maps the time interval in which the time effect has been applied to the time interval in which the time effect is removed.
 *
 * @param effectedTimeRange Time interval after applying time effects.
 *
 * @return Time interval after removing the time effect.
 */
- (MovieousTimeRange)getOriginalTimeRangeFromEffectedTimeRange:(MovieousTimeRange)effectedTimeRange;

@end

NS_ASSUME_NONNULL_END
