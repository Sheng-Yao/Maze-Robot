#include <Motor.h>
#include <Path_Finder.h>

// Flag to switch between Mapping and Solving the maze
bool isSolving = false;

void setup(){
  // Initialize serial monitor
  Serial.begin(115200);
  
  encoderSetup();
  motorSetup();
  mpuSetup();
  ultrasonicSetup();

  pinMode(A2,INPUT);
  if(digitalRead(A2) == HIGH){
    isSolving = true;
  }else if(digitalRead(A2) == LOW){
    isSolving = false;
  }

  Serial.println("Done setup..");
  delay(1000);
}

// State control of car movement (Stop when initial / block reached / end)
bool isMoving = false;

void loop(){
  // Stop when front distance smaller than 2cm
  if(getDistance(FRONT) > 4){
    // If car is not moving
    if(!isMoving){
      // Check variable holder whether the car need to turn
      if(isTurnLeft || isTurnRight || isUTurn){
        if(isTurnRight){
          // Control motor state
          turnRight(angle);
          // Completing the turning and forward action
          turnAndMoveForward();
          // Reset the variable once completed
          isTurnRight = false;
        }else if(isTurnLeft){
          // Control motor state
          turnLeft(angle);
          // Completing the turning and forward action
          turnAndMoveForward();
          // Reset the variable once completed
          isTurnLeft = false;
        }else if(isUTurn){
          // Control motor state
          uTurn(angle);
          // Completing the turning and forward action
          turnAndMoveForward();
          // Reset the variable once completed
          isUTurn = false;
        }
      }
      // Will enter here when robot finished the turning task
      else{
        resetDistance();
        moveForward();
        isMoving = true;
      }
    }
    // Will enter here when robot is moving between blocks
    else{
      moveForwardWithAlignment();
    }
  }else{
    Serial.println("Program stopped. Front Distance < 4cm");
    stop();
    while(1){;}
  }

  // Reach detection point
  if(isReachPoint){
    
    stop();
    delay(500);

    float distance[3] = {0,0,0};
    // Detect branches
    distance[0] = getDistance(LEFT);
    distance[1] = getDistance(RIGHT);
    distance[2] = getDistance(FRONT);
    
    if(!isSolving){
      // Check if the position is reached before
      if(maps[xPosition][yPosition] == "0"){


        // 1 branch
        if(distance[0] < ultrasonicMazeSize && distance[1] < ultrasonicMazeSize && distance[2] < ultrasonicMazeSize){// Dead End (X)
          Serial.println("Dead End (X)");
          moveCloseToWall();
          isUTurn = true;
          isTurnLeft = false;
          isTurnRight = false;
          maps[xPosition][yPosition] = 'X';
          Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
          if(currentMode == FORWARD){
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == BACKWARD){
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }
        }else if(distance[0] < ultrasonicMazeSize && distance[1] < ultrasonicMazeSize && distance[2] > ultrasonicMazeSize){// Front (F)
          Serial.println("Front (F)");
          isTurnLeft = false;
          isTurnRight = false;
          isUTurn = false;
          maps[xPosition][yPosition] = 'F';
          Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
          if(currentMode == FORWARD){
            yPosition++;
          }else if(currentMode == BACKWARD){
            yPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            xPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            xPosition++;
          }
        }else if(distance[0] > ultrasonicMazeSize && distance[1] < ultrasonicMazeSize && distance[2] < ultrasonicMazeSize){// Left (L)
          Serial.println("Left (L)");
          moveCloseToWall();
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          maps[xPosition][yPosition] = 'L';
          Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
          if(currentMode == FORWARD){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == BACKWARD){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = FORWARD;
            yPosition++;
          }
        }else if(distance[0] < ultrasonicMazeSize && distance[1] > ultrasonicMazeSize && distance[2] < ultrasonicMazeSize){// Right (R)
          Serial.println("Right (R)");
          moveCloseToWall();
          isTurnRight = true;
          isTurnLeft = false;
          isUTurn = false;
          maps[xPosition][yPosition] = 'R';
          Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
          if(currentMode == FORWARD){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }else if(currentMode == BACKWARD){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = BACKWARD;
            yPosition--;
          }
        }
        // 2 branches
        else if(distance[0] > ultrasonicMazeSize && distance[1] > ultrasonicMazeSize && distance[2] < ultrasonicMazeSize){// Left and Right (LR)
          Serial.println("Left and Right (LR)");
          moveCloseToWall();
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          maps[xPosition][yPosition] = "LR";
          Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
          if(currentMode == FORWARD){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == BACKWARD){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = FORWARD;
            yPosition++;
          }
        }else if(distance[0] > ultrasonicMazeSize && distance[1] < ultrasonicMazeSize && distance[2] > ultrasonicMazeSize){// Front and Left (FL)
          Serial.println("Front and Left (FL)");
          isTurnLeft = false;
          isTurnRight = false;
          isUTurn = false;
          maps[xPosition][yPosition] = "FL";
          Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
          if(currentMode == FORWARD){
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == BACKWARD){
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }
        }else if(distance[0] < ultrasonicMazeSize && distance[1] > ultrasonicMazeSize && distance[2] > ultrasonicMazeSize){// Front and Right (FR)
          Serial.println("Front and Right (FR)");
          isTurnLeft = false;
          isTurnRight = false;
          isUTurn = false;
          maps[xPosition][yPosition] = "FR";
          Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
          if(currentMode == FORWARD){
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == BACKWARD){
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }
        }
      }
      // Will enter here if the block reached before or is in solving mode
      else{
        if(maps[xPosition][yPosition] == "F"){// Front (F) [Written]
          Serial.println("Front (F) [Written]");
          isTurnLeft = false;
          isTurnRight = false;
          isUTurn = false;
          if(currentMode == FORWARD){
            if(maps[xPosition][yPosition - 1] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == BACKWARD){
            if(maps[xPosition][yPosition + 1] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            if(maps[xPosition + 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            if(maps[xPosition - 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }
        }else if(maps[xPosition][yPosition] == "L"){// Left (F) [Written]
          Serial.println("Left (L) [Written]");
          if(distance[2] < 9){
            moveCloseToWall();
          }
          if(currentMode == FORWARD){
            if(maps[xPosition][yPosition - 1] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == BACKWARD){
            if(maps[xPosition][yPosition + 1] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            if(maps[xPosition + 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnLeft = false;
            isTurnRight = false;
            isUTurn = false;
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            if(maps[xPosition - 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
            currentMode = BACKWARD;
            yPosition--;
          }  
        }else if(maps[xPosition][yPosition] == "R"){// Right (R) [Written]
          Serial.println("Right (R) [Written]");
          if(distance[2] < 9){
            moveCloseToWall();
          }
          if(currentMode == FORWARD){
            if(maps[xPosition][yPosition - 1] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnRight = false;
            isTurnLeft = true;
            isUTurn = false;
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == BACKWARD){
            if(maps[xPosition][yPosition + 1] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }else if(currentMode == RIGHT_DIRECTION){
            if(maps[xPosition - 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == LEFT_DIRECTION){
            if(maps[xPosition + 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "X";
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
            currentMode = BACKWARD;
            yPosition--;
          }  
        }
        // 2 branches
        else if(maps[xPosition][yPosition] == "LR"){// Left and Right (LR) [Written]
          Serial.println("Left and Right (LR) [Written]");
          if(currentMode == BACKWARD){
            if(maps[xPosition][yPosition + 1] == "X"){
              maps[xPosition][yPosition] = "R";
            }
            isTurnRight = false;
            isTurnLeft = false;
            isUTurn = false;
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            if(maps[xPosition - 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "R";
            }
            isTurnLeft = false;
            isTurnRight = false;
            isUTurn = false;
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }
        }else if(maps[xPosition][yPosition] == "FL"){// Front and Left (FL) [Written]
          Serial.println("Front and Left (FL) [Written]");
          if(currentMode == BACKWARD){
            if(maps[xPosition][yPosition - 1] == "X"){
              maps[xPosition][yPosition] = "L";
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            if(maps[xPosition + 1][yPosition] == "X"){
              maps[xPosition][yPosition] = "L";
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
            currentMode = FORWARD;
            yPosition++;
          }
        }else if(maps[xPosition][yPosition] == "FR"){// Front and Right (FR) [Written]
          Serial.println("Front and Right (FR) [Written]");
          if(currentMode == BACKWARD){
            if(maps[xPosition][yPosition - 1] == "X"){
              maps[xPosition][yPosition] = "R";
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }
        }
        // 1 branch
      }
    }else{
      if(finalSolution[0] != 'E'){
        if(finalSolution[0] == 'F'){
          isTurnLeft = false;
          isTurnRight = false;
          isUTurn = false;
          if(currentMode == FORWARD){
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == BACKWARD){
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }
        }else if(finalSolution[0] == 'L'){
          if(distance[2] < 9){
            moveCloseToWall();
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          if(currentMode == FORWARD){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == BACKWARD){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = BACKWARD;
            yPosition--;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = FORWARD;
            yPosition++;
          }
        }else if(finalSolution[0] == 'R'){
          if(distance[2] < 9){
            moveCloseToWall();
          }
          isTurnLeft = false;
          isTurnRight = true;
          isUTurn = false;
          if(currentMode == FORWARD){
            currentMode = RIGHT_DIRECTION;
            xPosition++;
          }else if(currentMode == BACKWARD){
            currentMode = LEFT_DIRECTION;
            xPosition--;
          }else if(currentMode == LEFT_DIRECTION){
            currentMode = FORWARD;
            yPosition++;
          }else if(currentMode == RIGHT_DIRECTION){
            currentMode = BACKWARD;
            yPosition--;
          }
        }
      }else if(finalSolution[0] == 'E'){
        stop();
        while(1){;}
      }
      finalSolution.remove(0,1);
    }

    // Reset state holder as car will be moving to next block
    isMoving = false;
    isReachPoint = false;

    // ending point
    if(xPosition == PUZZLE_X - 1 && yPosition == PUZZLE_Y - 1){
      Serial.println("Reached End Point");
      maps[xPosition][yPosition] == "E";
      Serial.println(maze_Solving(maps));
      while(1){;}
    }

    if(xPosition < 0 || yPosition < 0 || xPosition == PUZZLE_X || yPosition == PUZZLE_Y){
      Serial.println("Array out of range.");
      while(1){;}
    }

    // Print the current maps
    printMaps();
  }
}