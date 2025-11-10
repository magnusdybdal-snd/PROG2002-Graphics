# PROG20002 Graphics Programming

### Evaluation

This is a PASS. I have a couple of comments:

  - There are more efficient ways to draw the board (1 board tile + synthetic texturing, instead of 64 individual tiles)
  - There are more efficient ways to render the pieces. Try to think about how to reduce the number of draw calls for the pieces
  - I see you have enabled address sanitation in the top-level CMakeLists.txt. Please create a CMake option to enable/disable sanitation and set it to 'OFF' by default. This caused problems with renderdoc and wasn't trivial to find why.
  - I have deleted some leftover Makefile files in the source tree.

### Group_03 members
- Magnus Dybdal
- Marius Eilertsen
## Instructions
```bash
git clone --recursive git@git.gvk.idi.ntnu.no:course/prog2002/autumn_2025/workspace/Group_03.git
cd Group_03
mkdir build; cd build
cmake ../; make assignment; ./bin/assignment
```
### Controls
`T` - toggle textures
`H` / `L` - rotate camera
`O` / `P` - zoom
`ESC` - quit
`ENTER` - select/place chess piece
`ARROW KEYS` - move tile selector
## Issues completed
- [x] #1 Creating a Simple Executable using CMake
- [x] #2 Creating a Simple OpenGL Application (Part I)
- [x] #3 Creating a Simple OpenGL Application (Part II)
- [x] #4 Create a GLFWApplication class
- [x] #5 Add GeometricTools to the Framework
- [x] #6 Create an interactive chessboard
- [x] #7 Framework: VertexBuffer
- [x] #8 Framework: IndexBuffer
- [x] #9 Framework: Buffer Layouts and Vertex Array
- [x] #10 Framework: Shaders
- [x] #11 Creating a floor with the chessboard
- [x] #12 Creating a rotating 3D cube
- [x] #13 Creating basic render commands
- [x] #14 Managing texture files
- [x] #15 Add texture to the floor
- [x] #16 Applying texture to the cube through cubemaps
- [x] #17 Adding blending
- [x] #18 Create a TextureManager
- [x] #19 Create a Camera system
- [ ] #20 Add global illumination
- [ ] #21 Add diffuse illumination component to the cube
- [ ] #22 Add specular illumination component to the cube
- [x] #23 Create the base application
- [x] #24 Creation and placement of the game board
- [x] #25 Add a tile selector that can move through the board
- [x] #26 Add texturing and blending
- [x] #27 Add the units to the board
- [x] #28 Add units movement
- [x] #29 Camera settings and movements
## Workflow
### Issues and labs
We both wanted to learn as much as possible from this course. Although we were working in a group, we chose to complete all the issues leading up to the assignment individually. This approach created a healthy rivalry/race between us that proved beneficial for our progression.

During the lab sessions, we discussed our framework solutions and selected one implementation to move forward with, as they were very similar. As a result, there may appear to be an unbalanced number of commits between group members at first glance.

### Assignment
Upon completing the labs (and as the due date approached), we decided the most effective approach was to work together and discuss the assignment collaboratively.

We incorporated elements of extreme programming, working together on one computer to complete the assignment.

## Extras
We've added a feature where the selected tile not only turns yellow but also lifts itself up from the chessboard to further visualize that the tile is selected.
