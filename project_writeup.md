
---

**PID Controller Project**

The goals / steps of this project are the following:

* Perform a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a classifier Linear SVM classifier
* Optionally, you can also apply a color transform and append binned color features, as well as histograms of color, to your HOG feature vector. 
* Note: for those first two steps don't forget to normalize your features and randomize a selection for training and testing.
* Implement a sliding-window technique and use your trained classifier to search for vehicles in images.
* Run your pipeline on a video stream (start with the test_video.mp4 and later implement on full project_video.mp4) and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.

[//]: # (Image References)
[video1]: ./project_video.mp4

## [Rubric](https://review.udacity.com/#!/rubrics/824/view) Points
###Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
###Writeup / README

###Reflection

####1. Describe the effect each of the P, I, D components.

The PID components of the PID controller refer to the Proportional, Integral and Diffenerential errors used in determining how best to control an agent.

The goal when using a PID Controller is to minimize the cross track error (which is the error between the agent's current trajectory and the predetermined reference trajectory).

The Proportional error informs the agent how far off it is from the desired trajectory. This is the foundational component for the PID controller.

The Diffenerential error informs the agent of how close it is to the desired trajectory. Without adding the effect of the differential error, the agent over shoots while trying to maintain the desired trajectory which results in a lot of zig zag motion which is not desired for a self driving car.

Finally, the Integral error gives a measure of well the agent's trajectory matches the desired trajectory over time. This helps correct for other factors (such as incorrect steering alignment for the self driving car) which may have a negative impact on the agent maintainig its desired trajectory.



####2. Describe how the final parameters were chosen.

The final parameters were chosen usning the Twiddle algorithm.

This algorithm helps explore a range of parameter values to determine the optimal set of parameters that combine to provide the least amount of error specified by some error function.

I used it because it was quite simple to program.

However, some approaches like SGD sound interesting since it is an algorithm used for minimizing the loss of a specified loss/cost function

Here's a [link to my video result](./project_video.mp4)
