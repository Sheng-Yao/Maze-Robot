#include <Arduino.h>

String tempBranches;

bool isReturning = false;

String transitionPoint = "";

bool isAtPotentialEndingPoint = false;

String finalSequences = "";

bool isEndingPoint = false;








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