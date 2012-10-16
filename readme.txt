*******CS418 MP2**************
*******FLIGHT SIMULATOR*******
*******AUTHOR: YI CHENG*******
******************************

The binary file -- fly -- was compiled on a 64-bit ubuntu. 

Implementation:
1.A matrix library NEWMAT10 is adopted http://www.robertnz.net/nm10.htm#refer
2.Three orthogonal vectors are used to keep track of the orientation of the plane: lookat, ref, and up. roll is rotating about lookat,  pitch is rotating about ref.

Feature:
	1. lighting fixed
	2. enabled lighting on the plane
Advanced Feature: Mode Selection
			-Spectator Mode
			-Pilot Mode
Key: 
	'1' for PILOT Mode
	'2' for SPECTATOR Mode
	'w' 's' for pitch up/down
	'a' 'd' for roll left/right
	'q' 'e' for speed up/down

To compile: make clear && make 
To run: ./fly
To quit: press ESC
