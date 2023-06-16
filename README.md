# Visibility-Graphs

This program outputs the visibility tree of a point on a 2D plane given obstacles in the form of disjoint line segments.

**Note: Intersecting line segments may result in wrong output 

##Controls: 

First you must right click to position the observer at the mouse's current coordinates

Then you must left click to add points at desired locations, the most recent pair of unconnected points 
will be automatically connected to form a linesegment

After Adding all desired obstacles, you must right click to see the visibility tree of the observor

##Implementation:

Time Complexity O(nlogn) where n is the num of points based on an algorithm

A rotating sweep line centered at the observor, is used to rank line segments in the order of closest intersection to observor to farthest. 

C++ stl set is used to order the line segments due to its similarity to a self balancing binary tree

Tree Modification events occur when the sweep line rotates to one of the input points. 

