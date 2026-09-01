# VLSI CAD Maze Router

This project implements a **Maze Router** for large-scale ASIC physical design, supporting dual-layer routing, non-uniform grid costs, bend penalties, and via penalties. It features both **Dijkstra** and **A*** search algorithms, along with a performance benchmarking mechanism.

---

##  Project Directory Structure

* `include/` : Header files (`types.h`, `io.h`, `router.h`)
* `src/` : Source implementation files (`main.cpp`, `io.cpp`, `router.cpp`)
* `data/` : Input benchmark test cases (**Note: Test files are proprietary and not included in this repository**)
* `build/` : Compiled binaries and routing result outputs
* `makefile` : Automation build and run script

---

##  Input File Formats

The program reads two plain-text format input files:

1. **Grid File (`.grid`)**
   * **First Line**: Defines columns (X), rows (Y), bend penalty, and via penalty.
   * **Matrix Body**: Grid costs for Layer 1 and Layer 2, where `1` represents available space, `-1` represents obstacles, and positive integers greater than `1` represent high-cost routing regions.

2. **Netlist File (`.nl`)**
   * **First Line**: Total number of routing nets.
   * **Subsequent Lines**: Each line specifies a single net: `Net ID`, `Start Layer`, `Start X`, `Start Y`, `End Layer`, `End X`, `End Y`.

---

##  How to Build and Run

This project uses a standard `Make` build automation script optimized with `-O3` performance flags.

### 1. Compile the Project
Open a terminal in the root directory and run:
```bash
make
```
### 2. Run Specific Benchmarks
You can execute and test individual benchmarks using the general make rule:
```Bash
make 
run TEST=bench1
```
This automatically reads data from `data/bench1.grid` and `data/bench1.nl`, and outputs `_Dijkstra.rout` and `_A_star.rout` results into the `build/` directory.
### 3. Clean Build Artifacts
To clear compiled binaries and previous outputs:

```Bash
make clean
```

## Benchmark Results Example
```text
--- Running Dijkstra ---
Net 1 routed with cost: 8
... (Net 2 to Net N-1) ...
Net N routed with cost: 28

--- Running A* ---
Net 1 routed with cost: 8
... (Net 2 to Net N-1) ...
Net N routed with cost: 28

========================================
          Performance Report            
========================================
Dijkstra Time : 7.1382 ms
A* Time       : 8.0434 ms
========================================
```

## Output File Format (`.rout`)

The routing results must be saved into a plain-text output file. The file format strictly follows this sequential structure:

* **Total Number of Nets:** The very first line of the file must be an integer representing the total number of nets to be routed.
* **Net ID:** When starting the output for a specific net, the first line must print its Net ID.
* **Grid Coordinates:** If the routing is successful, the path is printed line-by-line. The format for each step is `Layer X Y`. Internal layer representations are converted back to layer 1 and layer 2 for the final output. 
* **Via Insertion:** If a path crosses between layers, an extra via marker must be printed between the coordinates of the two layers. This via marker uses `3` as the designated layer code.
* **Failed Routing:** If the algorithm cannot find a path for a net, it will not print any coordinates. It will simply print the Net ID followed directly by the end marker.
* **End Marker:** A single `0` must be printed on a new line at the very end of each net's path to indicate its completion.

#### Example File Structure

```text
20          // Total number of nets (e.g., 20 nets)
1           // Start of Net 1
1 3 36      // Coordinate: Layer 1, X=3, Y=36
1 3 35      // Coordinate: Layer 1, X=3, Y=35
3 3 35      // Via marker (Layer code 3) indicating a jump between layers
2 3 35      // Coordinate: Layer 2, X=3, Y=35
0           // End marker for Net 1
2           // Start of Net 2
0           // End marker for Net 2 (Indicates routing failure)
```

## Visualization Tool
To graphically inspect and verify the routing paths, you can use the official web-based visualization tool (Note: This online visualizer is an external public utility, not owned or hosted by this project):

Online Router Visualizer: https://spark-public.s3.amazonaws.com/vlsicad/javascript_tools/router.html

<img width="1273" height="713" alt="image-1" src="https://github.com/user-attachments/assets/f7853ab9-0b65-4518-a0af-4a7a4f87cbc3" />

Usage: Drag and drop your .grid benchmark file and the corresponding .rout result file into the respective browser drop zones to render the multi-layer routing layout.
