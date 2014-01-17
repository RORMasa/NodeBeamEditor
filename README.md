NodeBeam Editor
==============

NodeBeam Editor is a simple tool for building and editing BeamNG and Rigs of Rods nodebeam structures. The editor is still at very early stage of development.

##Current features
Currently it is possible to add nodes and beams, organize them in groups and define beam arguments. User can set blueprints to orthographic views. 

For easy testing, it is possible to specify preselected map and vehicle in editor settings and launch the game from the Test button.

The active file can be saved in editors own xml file format.

###Importing nodes and beams is suppored from:
* BeamNG
* Rigs of Rods
* Wavefront OBJ

###Export is supported to
* BeamNG
* Rigs of Rods

##Future plans
* Improve BeamNG import and export features to support slots, and to be able to export to existing file and rewrite only modified sections
* Improve Rigs of Rods import / export
* Improve the basic tools to make the editor more user friendly
* Add support to LUA scripting
* Add support for hydros and wheels
* Add tool to measure angle and distance
* Engine calculator, visually show torque and power curve.
* Beam organizer
* Generator/Calculator for RoR tracks node beam structure

##Bugs
* BeamNG importer imports Beams that are connected to a node in a different file incorrectly.
* Hubwheels don't work properly yet and cannot be exported or deleted inside editor once added
* Rigs of rods import has trouble with set_node_defaults
* Rigs of rods importer does not import nodes2
* Array tool does not work

##What it can be used for
* Making of basic node beam structure
* Porting nodes and beams between Rigs of Rods and BeamNG
* Converting nodes and beams from 3D program into BeamNG, trough the OBJ format which nearly every 3D program supports

##What it cannot be used for
* Placing meshes/3D models on the nodebeam structure
* Importing DAE
* Adding wheels
* Adding hydros
* Adding anything else except nodes and beams