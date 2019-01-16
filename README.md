# Monocular Augmentation
This project deals with the augmentation of monocular images while attempting to maintain realistic occlusion and minimize manual intervention. The implementation uses [Godard et. al. Unsupervised Monocular Depth Estimation With Left Right Consistency](https://ieeexplore.ieee.org/abstract/document/8100182) to estimate a dense depth map for an input image.
Each texel is then rendered at the depth given by the depth map.

<p align="left">
<img src="https://github.com/acvictor/Monocular-Augmentation-Depth-Map/blob/master/images/1.jpg" alt="NMPC" width="256" height="192" border="50" /></a> 
<img src="https://github.com/acvictor/Monocular-Augmentation-Depth-Map/blob/master/images/1_disp.png" alt="NMPC" width="256" height="192" border="50" /></a>
<img src="https://github.com/acvictor/Monocular-Augmentation-Depth-Map/blob/master/images/6.png" alt="NMPC" width="316" height="192" border="50" /></a>
</p>

## Compilation and Execution Instructions
To compile and execute the code run
```
make run
```

## Data
 * D.txt is a 768x1024 array of depth values
 * 1.jpg is the original image
 * result.png is the segmented image

## Usage 
 * Use the arrow keys to move camera up, down, left and right
 * Use the '+' and '-' keys to move camera in and out
	* '+' zooms in
	* '-' zooms out
 * Use left mouse button to rotate camera
 * Use 'b' to toggle bounding boxes
 * Use ESC to quit.

## Attribution
Used https://github.com/mrharicot/monodepth to construct the depth map

