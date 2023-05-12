# null-render
<p align="center">
  <img src="https://i.imgur.com/9R6S6I6.png">
</p>
Heavily redesigned ImGui renderer with a lot of new features.

# supported renderers and features

| backends      | base render | shaders | msaa |
| ------------- |:-----------:|:-------:|:----:|
| directx 11    | ✔️          | ✔️     | ✔️   |
| directx 9     | ✔️          | ✔️     | ✔️   |
| opengl 3      | ✔️          | ✔️     | ✔️   |

# msaa example
<p float="left">
  <img width=250 src="https://i.imgur.com/pQJvFcA.png">
  <img width=250 src="https://i.imgur.com/i0lurGn.png">
</p>

# opengl3 renderer
To connect opengl3-renderer you need:
- project properties->resources->additional include directories: specify the path to the file "null-render-backend-opengl3-resources.rc"
- create a .rc file in the project and include "null-render-backend-opengl3-resources.rc" into it

# dependencies
[null-sdk](https://github.com/nullptr-sources/null-sdk)