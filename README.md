# Raven

## About
Raven is meant to be a programming exercise in building a compiler. As of right now, it is in the middle of development, and so can't be used to compile anything. The current stage of development is writing the Lexer and Parser. One of the goals of this project involved being able to change the scannable tokens on the fly, and also specify them using regex. Since C does not come built in with regex, I realized it would be fun to implement a regex engine to more easily specify what tokens should look like. To make a regex engine, I'd need a parser. 

## Building
Again, Raven is in the middle of development, but can be built by going to the main project directory and typing in **make**. Since this is a multi-tier project, there are subdirectories that are not compiled from the top directory. To compile them, navigate to the corresponding subdirectory, check to see if there is a makefile. If there is one, execute: **make** and you'll have the current project. 
