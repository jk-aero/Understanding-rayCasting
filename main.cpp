
#include "raylib.h"
#include<iostream>
#include<cmath>
#include<vector>




const int screenWidth = 600;
const int screenHeight = 600;
const int cellSize = 30;
const int numOfCellX = screenWidth / cellSize;
const int numOfCellY = screenHeight / cellSize;


class Grid { 
public:
    int posX, posY; int type; Color GRAY; 
    Grid(int x, int y) :posX(x), posY(y) {}
    //type 1 means obstruction 
    //type 0 means no obstruction
    //type 2 means its start
    //type 3 means its end
};  

std::vector<Grid> grids(numOfCellY*numOfCellX, Grid(0, 0));
std::vector<Vector2>points;

Vector2 normalise(Vector2 s, Vector2 e) 
{
    int x = s.x - e.x;
    int y = s.y - e.y;
    float mag = std::sqrt(x * x + y * y);
    return { x / mag,y / mag };


}

void raycast(float startX, float startY, float endX, float endY, std::vector<Grid>& blocks) {
    float deltaX = endX - startX;
    float deltaY = endY - startY;
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

    float stepX = deltaX / distance;
    float stepY = deltaY / distance;

    float x = startX;
    float y = startY;

    while (distance > 0) {
        // Check collision with blocks
        for (auto& block : blocks) {
            if (std::abs(x - block.posX) < 0.5 && std::abs(y - block.posY) < 0.5) {
                if (block.type == 1) {
                    block.Color = ORANGE;  // Change color if it's an obstacle
                }
            }
        }

        // Move to the next point on the ray
        x += stepX;
        y += stepY;
        distance -= 1.0;  // Move 1 unit along the ray (you can adjust this step size)
    }
}


int main(){
   
    // Initialization
    //--------------------------------------------------------------------------------------
 
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
   
    for (int i = 0; i < numOfCellX; i ++) {
        for (int j = 0; j < numOfCellY; j ++) 
        {
            grids[j*numOfCellX+i] = Grid{ i * cellSize,j * cellSize };
            //std::cout << grids[j * numOfCellX + i].posX << " :x   "<< grids[j * numOfCellX + i].posY<<" :y \n";
           
        }
    };

    bool setStart = 0;
    bool setEnd = 0;
    float startX = 0; float startY = 0;
    float endX = 0; float endY = 0;

  


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
     
        
        raycast(startX, startY, endX, endY, grids);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);


        
            for (int x = 0; x < screenWidth; x+=cellSize) {
                for (int y = 0; y < screenHeight; y+=cellSize) {
                    int mouse_x = GetMousePosition().x;
                    int mouse_y = GetMousePosition().y;
                   

                    if ((mouse_x >= x && mouse_x <= x + cellSize) && (mouse_y >= y && mouse_y <= y + cellSize))
                    {
                        if (IsKeyDown(KEY_S)&& !setStart) 
                        {   //i=x+j*w
                            grids[(x / cellSize)+(y / cellSize)*numOfCellY].type = 2;
                            grids[(x / cellSize) + (y / cellSize) * numOfCellY].Color = GREEN;
                            startX = x ;
                            startY= y ;
                            setStart = 1;
                        }

                        if (IsKeyDown(KEY_E) && !setEnd)
                        {
                            //i=x+j*w
                            grids[(x / cellSize) + (y / cellSize) * numOfCellY].type = 3;
                            grids[(x / cellSize) + (y / cellSize) * numOfCellY].Color = RED;
                            endX = x;
                            endY = y;
                            setEnd = 1;
                        }
                        if (IsMouseButtonDown(0)) {
                            //  if we again click on the start/stop it will change to obstacle
                            if (grids[(x / cellSize) + (y / cellSize) * numOfCellY].type == 2) { setStart = 0; }
                            if (grids[(x / cellSize) + (y / cellSize) * numOfCellY].type ==3) { setEnd   = 0; }

                            grids[(x / cellSize) + (y / cellSize) * numOfCellY].type = 1;
                            grids[(x / cellSize) + (y / cellSize) * numOfCellY].Color = DARKPURPLE;


                        }
                        if (IsMouseButtonDown(1)) {
                            //c
                            if (grids[(x / cellSize) + (y / cellSize) * numOfCellY].type == 2) { setStart = 0; }
                            if (grids[(x / cellSize) + (y / cellSize) * numOfCellY].type ==3) { setEnd  =  0; }
                            grids[(x / cellSize) + (y / cellSize) * numOfCellY].type = 0;
                            grids[(x / cellSize) + (y / cellSize) * numOfCellY].Color = GRAY;
                        }
                    }


                 

                 
                    

                    DrawRectangle(x, y, cellSize - 1, cellSize - 1, grids[(x / cellSize) + (y / cellSize) * numOfCellY].Color);
                    if(setStart && setEnd)
                    {
                        for (auto p : points) {
                            DrawCircleV(p, 1, RED);
                        }
                        
                        DrawLine(startX+5, startY+5,  endX+5, endY+5, WHITE
                    );

                    
                }
                }
            }



          
        EndDrawing();
        //----------------------------------------------------------------------------------


       
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
