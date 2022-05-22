# null-render
<p align="center">
  <img width="738" height="307" src="https://user-images.githubusercontent.com/60057961/169716006-5866d678-3b92-4655-b910-156ac6995529.png">
</p>
This is an attempt to translate part of my previous null-gui project to c++20.

At the moment, only directx9 is available, but I hope to add all the other backends (directx11, opengl) later.
Also, after I add all the backends and rewrite the gui, shaders will be added (I hope this will be implemented closer to the middle or end of summer).

I tried to translate the entire ImGui render to c++20, most likely I didn’t get it very well, so if you notice some c-style code in the project, or incorrect use of stl - welcome to the issue or pull request tab, I will appreciate any help.

# supported renderers and features
- ### directx9
    - base render
# dependencies
[stb](https://github.com/nothings/stb)\
[null-sdk](https://github.com/nullptr-sources/null-sdk)