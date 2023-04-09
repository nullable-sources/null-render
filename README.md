# null-render
<p align="center">
  <img src="https://user-images.githubusercontent.com/60057961/205475625-1f49c685-4330-47f9-b583-30c094491026.png">
</p>
This is an attempt to translate part of my previous null-gui project to c++20.

At the moment, all more or less popular backends are implemented (dx9/dx11/opengl 3), in the future different functions will be added (shaders, maybe something else).

I tried to translate the entire ImGui render to c++20, most likely I didn’t get it very well, so if you notice some c-style code in the project, or incorrect use of stl - welcome to the issue or pull request tab, I will appreciate any help.

# supported renderers and features

| backends      | base render | shaders | msaa |
| ------------- |:-----------:|:-------:| ----:|
| directx 11    | ✔️          | ✔️     | ✔️   |
| directx 9     | ✔️          | ✔️     | ✔️   |
| opengl 3      | ✔️          | ✔️     | ❌   |

# msaa preview
<img src="https://i.imgur.com/pQJvFcA.png">
<img src="https://i.imgur.com/i0lurGn.png">

# opengl3 renderer
To connect opengl3-renderer you need:
- project properties->resources->additional include directories: specify the path to the file "null-renderer-opengl3-resources.rc"
- create a .rc file in the project and include "null-renderer-opengl3-resources.rc" into it

# dependencies
[null-sdk](https://github.com/nullptr-sources/null-sdk)