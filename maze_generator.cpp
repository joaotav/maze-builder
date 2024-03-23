#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <sstream>
#include <random>
#include <fstream>

/* 
   Implemented by João Chervinski (2018)
*/

using namespace std;

// A struct to represent each of the maze's cells
typedef struct cell{
  int visited = 0;
  int left = 1;
  int top = 1;
  int right = 1;
  int bottom = 1;
} cell;


void save_to_file(vector<vector<cell*>> maze, int height, int width){
  // Saves the generated maze to a text file, where:
  // 0 - represents a path/empty space
  // 1 - represents a wall
  ofstream output;
  output.open("maze.txt");

  for(int h = 0; h < height; h++){
    if (h == 0){
      output << 1;
      for(int w = 0; w < width; w++){
        output << maze[h][w]->bottom;
        output << 1;
      }
      output << endl;
    }

    for(int w = 0; w < width; w++){
      if (w == 0){
        output << maze[h][w]->left;
      }
      output << 0;
      output << maze[h][w]->right;
    }
    output << endl;
    for(int w = 0; w < width; w++){
      if (w == 0){
        output << 1;
      }
      output << maze[h][w]->top;
      output << 1;
    }
    output << endl;
  }
}

vector<vector<cell*>> build_maze(vector<vector<cell*>> maze, int height, int width){
    int visited_cells = 0;
    int total_cells = width * height;
    int direction = 0;
    bool first = true;
    bool popped = false;
    vector<int> pos(2); // Vector to store cell positions
    vector<int> available_directions (4); // Vector that stores possible directions from the current cell
    stack<vector<int>> cell_stack; // Stack for backtracking

    // Build the random number generator
    random_device rdevice{};
    mt19937 engine{rdevice()}; // Mersenne-Twister Generator
    uniform_int_distribution<int> height_dist{0, height - 1};
    uniform_int_distribution<int> width_dist{0, width - 1};

    // Choose random initial position
    int h = height_dist(engine);
    int w = width_dist(engine);

    while(visited_cells < total_cells){

        if (maze[h][w]->visited == 0){
            // If the cell has not been visited yet, mark it as visited
            visited_cells += 1;
            maze[h][w]->visited = 1;
        }

        if (first){
            // If it's the first cell, no need to remove the wall from the direction it came from
            first = false;
        } else {
            // If it's not the first cell
            // Remove the wall from the direction it came from
            if (popped == false){
                if (direction == 0){ // Last move was upward
                    maze[h][w]->bottom = 0;
                } else if(direction == 1){ // Last move was to the right
                    maze[h][w]->left = 0;
                } else if(direction == 2){ // Last move was downward
                    maze[h][w]->top = 0;
                } else if(direction == 3){ // Last move was to the left
                    maze[h][w]->right = 0;
                }
            }
        }

        available_directions.clear();

        if ((h + 1 <= height - 1) and (maze[h+1][w]->visited == 0)) {
            // If the position above is available, add it to the vector
            available_directions.push_back(0);
        }
        if ((w + 1 <= width - 1) and (maze[h][w+1]->visited == 0)){
            // If the position to the right is available, add it to the vector
            available_directions.push_back(1);
        }
        if ((h - 1 >= 0) and (maze[h-1][w]->visited == 0)){
            // If the position below is available, add it to the vector
            available_directions.push_back(2);
        }
        if ((w - 1 >= 0) and (maze[h][w-1]->visited == 0)) {
            // If the position to the left is available, add it to the vector
            available_directions.push_back(3);
        }

        popped = false;
        
        if (available_directions.size() > 0){
    	    // 0 = Up; 1 = Right; 2 = Down; 3 = Left
            int directions = static_cast<int>(available_directions.size());
            uniform_int_distribution<int> new_direction{0, directions - 1};
            while(true){
                direction = available_directions[new_direction(engine)];
                if (direction == 0){ // Up
                    pos[0] = h; // Save the position in a vector to push it onto the stack
                    pos[1] = w;
                    cell_stack.push(pos);
                    maze[h][w]->top = 0; // Remove the top wall
                    h += 1;
                    break;
                }
                else if (direction == 1){ // Right
                    pos[0] = h; // Save the position in a vector to push it onto the stack
                    pos[1] = w;
                    cell_stack.push(pos);
                    maze[h][w]->right = 0; // Remove the right wall
                    w += 1;
                    break;
                }
                else if (direction == 2){ // Down
                    pos[0] = h; // Save the position in a vector to push it onto the stack
                    pos[1] = w;
                    cell_stack.push(pos);
                    maze[h][w]->bottom = 0; // Remove the bottom wall
                    h -= 1;
                    break;
                }
                else if (direction == 3){ // Left
                    pos[0] = h; // Save the position in a vector to push it onto the stack
                    pos[1] = w;
                    cell_stack.push(pos);
                    maze[h][w]->left = 0; // Remove the left wall
                    w -= 1;
                    break;
                }
            }
        } else {
            // Get the position of the cell at the top of the stack
            h = cell_stack.top()[0];
            w = cell_stack.top()[1];
            cell_stack.pop(); // Remove the top of the stack
            popped = true;
        }
    }
    uniform_int_distribution<int> make_entrance{0, height - 1};
    // Choose a position for the entrance of the maze
    h = make_entrance(engine);
    maze[h][0]->left = 0;

    // Choose a position for the exit of the maze
    h = make_entrance(engine);
    maze[h][width - 1]->right = 0;

    return maze;
}


int main(int argc, char* argv[]){
  vector<vector<cell*>> maze;

  int arg1, arg2;
  if (argc >= 3){
    // Argument check
    istringstream height(argv[1]);
    istringstream width(argv[2]);
    if (!(height >> arg1) || !(width >> arg2)){
      cout << endl << "[+] Usage: " << argv[0] << " <maze_height> <maze_width>" << endl;
      cout << endl << "Please provide two numbers as arguments." << endl << endl;
      exit(1);
    }
  }
  else {
    cout << endl << "[+] Usage: " << argv[0] << " <maze_height> <maze_width>" << endl;
    cout << endl << "[+] Please provide two numbers as arguments." << endl << endl;
    exit(1);
  }

  int height = arg1;
  int width = arg2;

  maze.resize(height);

  // Initialize maze cells
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      maze[i].push_back(new cell);
    }
  }

  // Build the maze
  maze = build_maze(maze, height, width);

  // Store the maze in a text file
  save_to_file(maze, height, width);

  // Draws the maze using the python script
  system("python3 draw_maze.py maze.txt");
  cout << "[+] Maze image has been saved as 'perfect_maze.ppm'." << endl;

  return 0;
}

