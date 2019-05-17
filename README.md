


# Caramel Engine

**Caramel Engine** is a lite engine I'm working on in my free time to improve my understanding of engine development and graphics programming. I plan on using this project as an educational tool but I hope to one day possibly create a small game if the engine is complex enough.

# Features
## ECS
CaramelEngine uses an Entity-Component-System (**ECS**) for the foundations of the engine, this is what other engines like [Unity](https://unity.com/) or [Unreal Engine](https://www.unrealengine.com/en-US/) use. The reason I choose this architectural pattern is because its simple to understand and can be very powerful allowing for quick and easy creation of different types of entities in a scene.

## OpenGL
CaramelEngine currently renders using [OpenGL](https://www.opengl.org/), when this engine matures, I hope to abstract the rendering process of Caramel to allow for other rendering platforms such as [Vulkan](https://www.khronos.org/vulkan/).

## ImGui
CaramelEngine uses [ImGui](https://github.com/ocornut/imgui) to handle the UI of the engine. ImGui was picked because of how simple and effective it is to use. For game UI however, like drawing things to screen from a game, opposed to the current editor, that will be a later addition to Caramel.

# Controls
## Scene navigation
Hold right click on the editor window and use WASD EQ to move the camera about the scene, moving your mouse will also change the direction of the camera. **Note:** You must have atleast one active camera in the scene to see anything in the scene, add on by going to **Component**->**Rendering**->**Camera** with atleast one entity in the scene.
## Shortcuts
- `Ctrl + N`: Creates a new entity.
- `Ctrl + D`: Duplicates the selected entity **Note:** MeshFilter doesn't duplicate currently.
- `Del`: Deletes selected entity.
- `F2`: Highlights the selected entitys' name for editing.

## Demonstration
[![Demo](https://img.youtube.com/vi/HF1gQdjV20s/0.jpg)](https://www.youtube.com/watch?v=HF1gQdjV20s)
This is a demo of me using my engine to create a simple scene, nothing fancy but shows the usage!
