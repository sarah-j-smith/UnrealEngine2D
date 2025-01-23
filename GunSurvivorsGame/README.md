# Unreal Engine 2D Flipbooks Tutorial

* My notes from [Game Dev TV Unreal 2D course].

Go to my [notes from the previous section of this course](../DesertRacerGame/README.md) for sprite sheets and general Paper2D.

[Game Dev TV Unreal 2D course] https://www.gamedev.tv/courses/unreal-2d

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

## Packaging for iOS

* Very good video on how to do this in 2024, with new XCode integration in Unreal

* https://youtu.be/fuUYPrZNE6Y?si=pWJ10xz67kRE4uvj
* Have to create an app on the [Apple Developer portal]
* From there assign a new App Id:
    * `au.com.smithsoft.splodgehunters`

[Apple Developer portal]: https://developer.apple.com/account/