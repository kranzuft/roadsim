# RoadSim - A Road Traffic Simulator

RoadSim is a road traffic simulation program that uses raylib and related libraries to create and visualize a road traffic system. The simulator allows users to create and manipulate road networks consisting of points (entrances, exits, and connections) and cars that move between them.

## Features

Current features include:
- Creating entrance and exit points with various colors
- A* pathfinding algorithm to determine the shortest path between entrance and exit points
- Cars that follow the paths generated by the A* algorithm
- Adjustable settings for road width, point radius, line width, car length, car speed, and car spawn interval
- Basic UI for modifying simulation settings

## Planned Features

The following features are planned for future development:

1. Add proper art assets to replace placeholder graphics
2. Add multiple layers to simulate bridges and multi-level road networks
3. Improve the A* pathfinding algorithm for better performance and accuracy
4. Make entrances more flexible by allowing weightings for car distribution to exits, and improve the RNG for better traffic simulation
5. Add a 3D viewer for the road network to provide a more immersive visualization
6. Add barriers to restrict car movement and control traffic flow
7. Implement a full-fledged UI for easier control and interaction with the simulator
8. Introduce different types of vehicles (e.g., trucks and buses), along with special road conditions to account for their presence
9. Generate road graphics with directions placed more sparingly for better visualization

## Dependencies

The following libraries are used in this project:

- raylib
- raymath
- raygui
- Various C++ standard libraries (memory, array, unordered_map, queue, etc.)

## Building

RoadSim uses CMake as the build tool. To build and run the project, follow these steps:

1. Install the raylib library, if you haven't already
2. Clone the repository and navigate to the project directory
3. Create a build directory: `mkdir build && cd build`
4. Run CMake to generate build files: `cmake ..`
5. Compile the source code: `make`
6. Run the compiled executable to launch the RoadSim application: `./roads`

## Contributing

Contributions are welcome! Please feel free to open issues, submit pull requests, or help improve the documentation for this project.
