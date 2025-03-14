# Refinements

These are miscellaneous tweaks and improvements that can be done to fix issues or improve gameplay.

## Camera Lag

Having the camera instantly follow the player feels stiff.  In the player controller you
can turn on camera lag to have the camera wait and catch up to the player movement
after a small lag. Smaller numbers make a more noticeable lag.

This is done in the `Lag` section of the `Spring Arm` component to which the
camera is attached.

## Use Flat Base

Since a capsule is used for the characters collision volume when they are on an edge of a 
platform they sink. This because of the curved bottom of the capsule.

Setting checked on Character Movement Walking: Advanced - use flat base for floor checks
prevents this problem.

## Camera Viewport

The game as is has a too large viewport and also its showing too much of the empty area
under the bottom of the screen. To fix these two issues, first move the spring arm up
by 50 pixels, above the player. Second change the camera ortho width to 900 (from 1024).

