//
//  ATSDragToReorderTableViewController.h
//  Reordering
//
//  Created by Daniel Shusta on 11/28/10.
//  Copyright 2010 Acacia Tree Software. All rights reserved.
//
//  Permission is given to use this source code file, free of charge, in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//
//  THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
//	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
//	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
//	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
//	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


/*
 
	Interface Overview:
 
		ATSDragToReorderTableViewController is a UITableViewController subclass 
		that incorporates a press-and-drag-to-reorder functionality into the 
		tableView.
		
		Because it subclasses from UITableViewController, you can use existing
		classes that subclass from UITableViewController and subclass from
		ATSDragToReorderTableViewController instead. You only need to make a few
		changes (listed below). 
 
		ATSDragToReorderTableViewControllerDelegate notifies upon change in 
		dragging state. This could be useful if the destination or source of the
		reorder	could change the content of the cell.
 
		Requires iOS 4.0 or greater.
 
 
	Steps for use:
 
		0. If you aren't already, link against the QuartzCore framework.
		In Xcode 3.2, right-click the group tree (sidebar thingy),
		Add -> Existing Frameworks… -> QuartzCore.framework
		In Xcode 4.0, go to the project file -> Build Phases and add to 
		Link Binary With Libraries.
 
		1. Subclass from this instead of UITableViewController
 
		2. UITableViewDataSource (almost certainly your subclass) should
 		implement -tableView:moveRowAtIndexPath:toIndexPath: 
 
		3. Subclass should call [super viewDidUnload] from -viewDidUnload.
 
 
	Other recommendations:
		
		It is recommended that the tableView's dataSource -setReorderingEnabled:
		to NO if there is only one row.
 
		Rotation while dragging a cell is screwy and it's not clear to me how to
		handle that situation. For now, recommendation is to not allow rotations
		while dragging, maybe not at all.

 
	Assumptions made by this code:
 
		Subclass doesn't conform to UIGestureRecognizerDelegate. If it does,
		you're going to have to figure out how to orchastrate it all. It's
		likely not that difficult.
		
		self.tableView is of type UITableViewStylePlain. It will technically 
		work with UITableViewStyleGrouped but it'll look ugly.
 
		This code totally ignores self.tableView.delegate's 
		-tableView:targetIndexPathForMoveFromRowAtIndexPath:toProposedIndexPath:
		I have no idea what I would do with that.
 
 */


#import <UIKit/UIKit.h>
#import <QuartzCore/CADisplayLink.h>
#import <QuartzCore/CALayer.h>


@protocol ATSDragToReorderTableViewControllerDelegate;

@interface ATSDragToReorderTableViewController : UITableViewController <UIGestureRecognizerDelegate>  {
	NSObject <ATSDragToReorderTableViewControllerDelegate> *dragDelegate;	

@private
	// Use setter/getter, not even subclasses should adjust this directly.
	BOOL reorderingEnabled;
	
	UIPanGestureRecognizer *dragGesture;
	UILongPressGestureRecognizer *longPressGesture;	
	
	CADisplayLink *timerToAutoscroll;
	CGFloat distanceThresholdToAutoscroll;
	
	CGFloat initialYOffsetOfDraggedCellCenter;
	
	UITableViewCell *draggedCell;
	NSIndexPath *indexPathBelowDraggedCell;
	
	/*
	 *	For restoring the cell to its former color, only used in add/remove draggability code.
	 */	
	CGFloat formerSelectedBackgroundViewColorAlpha;
	
	id resignActiveObserver;
}


// default is YES. Removes or adds gesture recognizers to self.tableView.
@property (assign, getter=isReorderingEnabled) BOOL reorderingEnabled;

- (BOOL)isDraggingCell;

/*
 *	Optional delegate
 */
@property (assign) NSObject <ATSDragToReorderTableViewControllerDelegate> *dragDelegate;

@end


/*
 *	Delegate can observe beginning and ending of dragging. 
 */
@protocol ATSDragToReorderTableViewControllerDelegate

@optional

- (void)dragTableViewController:(ATSDragToReorderTableViewController *)dragTableViewController didBeginDraggingAtRow:(NSIndexPath *)dragRow;
- (void)dragTableViewController:(ATSDragToReorderTableViewController *)dragTableViewController willEndDraggingToRow:(NSIndexPath *)destinationRow;

@end


