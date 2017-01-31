# CAL
Repository to host Algorithm Design and Analysis project.

CAL is a course given at FEUP in the second year of the Master in Informatics and Computing Engineering.
 
## Goal
The goal of this project was to implement a program that identifies the path to follow, in a given network, from a source to the desired destination, accordingly to the criteria given by the user.

## Features
* Simple or complex itinerary (with points of interest along the desired path);
* Multiple path options (less distance, less travel time, avoid toll points);
* Graph connectivity evaluation (doesn't allow to choose paths with inaccessible zones);
* Real maps, extracted from [OpenStreetMaps](https://www.openstreetmap.org);
* Exact and approximate location search.

## Technologies
* C++ as programming language.
* OSM2TXT Parser.
* GraphViewer API to visualize graphs using Jung framework.

## Gallery

## How To Run 
1. Create an empty C++ project, using MinGW as ToolChain. 
2. Add all EasyPilot files to the project. 
3. To run GraphViewer, change the following settings: 
C++ Build > Settings > MinGW C++ Linker > Miscellaneous > Add new object: < MinGW Path >\libwsock32.a (ex: C:\MinGW\lib\libwsock32.a). 
4. Add "-static" to the linker flags. In order to run the jar GraphViewerController, Java has to be installed.

## Team 
[José Aleixo](https://github.com/jazzchipc)

[José Carlos](https://github.com/Evenilink)

[Renato Abreu](https://github.com/renatoabreu11)
