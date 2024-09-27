# Unreal Engine 2D Flipbooks Tutorial

* My notes from [Game Dev TV Unreal 2D course].

Go to my [notes from the previous section of this course] for sprite sheets and general Paper2D.


[Game Dev TV Unreal 2D course] https://www.gamedev.tv/courses/unreal-2d

[notes from the previous section of this course]: https://github.com/sarah-j-smith/Unreal-Engine-2D-Tutorial/blob/main/Docs/HowTo.md

## Creating a Cursor

* In Blueprints folder, right click to create a new Blueprint
* Choose _User Interface_ > _Widget Blueprint_ > User Widget
* Name it `WidgetBP_MouseCursor`
* From the Palette drag a _Canvas Panel_ down into the _Hierarchy_ 
    - on top of `WidgetBP_MouseCursor`
* From the Palette drag a _Image_ down into the _Hierarchy_
    - on top of the _Canvas Panel_
* From the Image > Brush > Select the Sprite asset for the cursor
* Set Size-X and Size-Y appropriately - eg 100
* Select the Canvas Panel in the Hierarchy
* Use the drop-down top right to set it to _Desired On Screen_ to fit the canvas
* Select the image and set the anchor to the middle
* Save and compile
* In the Project Settings > Engine > Software Cursor
    - Add a new default cursor and select `WidgetBP_MouseCursor`