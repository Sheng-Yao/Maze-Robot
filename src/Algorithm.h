#include <Arduino.h>

String tempBranches;

bool isReturning = false;

String transitionPoint = "";











// //class to store the branch information
// class Branch{

//     private:
//         byte numbering;
//         byte leveling;
//         char direction;
//         bool isFinalPath;
        
//     public:

//         //initilize the object
//         Branch(){

//         }

//         Branch(byte numberingInput, byte levelingInput, char directionInput){
//             init(numberingInput, levelingInput, directionInput);
//         }


//         void init(byte numberingInput, byte levelingInput, char directionInput){
//             setNumbering(numberingInput);
//             setLeveling(levelingInput);
//             setDirection(directionInput);
//         }

//         void setNumbering(byte input){
//             this->numbering = input;
//         }

//         byte getNumbering(){
//             return this->numbering;
//         }

//         void setLeveling(byte input){
//             this->leveling = input;
//         }

//         byte getLeveling(){
//             return this->leveling;
//         }

//         void setDirection(char input){
//             this->direction = input;
//         }

//         char getDirection(){
//             return this->direction;
//         }

//         void reverseDirection(){
//             if(this->direction == 'L'){
//                 this->direction = 'R';
//             }else if(this->direction == 'R'){
//                 this->direction = 'L';
//             }else{
//                 //infinity loop
//                 while(1);
//             }
//         }

//         void setIsFinalPath(bool input){
//             this->isFinalPath = input;
//         }

//         bool getIsFinalPath(){
//             return this->isFinalPath;
//         }
// };

// //to delete the class instance ==> using delete object instance;


// bool isReturning = false;
// bool isBranching = false;
// bool isRemainingWays = false;

// byte currentSubBranching = 0;

// String subBranch1 = "";
// String subBranch2 = "";
// String subBranch3 = "";

// char desiredDirection;

// Branch overallPath[10][10] = {};

// byte numberOfSubBranch = 0;



// //#######   Class definition about the branches in the maze #######//
// byte currentNumber = 0;
// byte currentLevel = 0;

// //indicator when find the end point
// bool isEndPoint = false;

// bool isBranchCalculated = false;
