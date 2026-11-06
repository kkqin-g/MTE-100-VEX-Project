#include "vex.h"
using namespace vex;

// A global instance of vex::brain used for printing to the IQ2 brain screen
vex::brain       Brain;

motor baseMotor(PORT3, bool(forward));
motor flyWheel(PORT1, bool(forward)); 
motor feedMotor(PORT2, bool(reverse));

const double BASE_GEAR_RATIO = 4.0; //motor 4x, gear 1x  
const double BASE_ROTATE_SPEED = 5.0; 
const int SETTLE_MSEC = 20; //rotation must settle for 20 msec before shooting out card 
const double FEED_CARD_ROTATION = 400.0; //how much does the feeder has to rotate to push out one card 
const int CARD_CLEAR_MSEC = 20; //pause time after card is shot out  
const bool STOP_REQUESTED = true; 

const int numPlayers = 0; //this value will become user input later in proj development 


//forwards declarations 
double numDealFor(int playerCounter, int numPlayers); 
double FEED_DEGREE_PER_CARD = 30.0; 



int main() {
    initRobot(); 
    //int player = 0; 

    while(!STOP_REQUESTED) {

    }
}


bool rotateTo(double target) {
  baseMotor.spinToPosition(target, degrees, false);
  while (!baseMotor.isDone()) {
    if (STOP_REQUESTED) {
      baseMotor.stop();
      return false;
    }
  }
  return true;
}


double numDealFor(int playerCounter, int numPlayers) {
    double turnBaseDegrees = playerCounter * (360.0 / numPlayers); 
    return turnBaseDegrees * BASE_GEAR_RATIO; 
}

//tracks that the base arrived at target location + has settled. we can lowkey add this later 
bool waitUntilSettled(double target) {
    int stableMsec = 0;

    while(stableMsec < SETTLE_MSEC) {
        
    } //bro imma do this later 
}

bool dealOneCard() {
    feedWheel.setPosition(0, degrees); 
    feedWheel.spinToPosition(FEED_DEGREE_PER_CARD, degrees, false); 
    while(!feedWheel.isDone()) {
        if(STOP_REQUESTED) {
            feedMotor.stop(brake); 
            return false; 
        }
        wait(20,msec); 
    }
    return true; 
    //logic on this one might be wrong, double check 
}


void initRobot() {

    //blah blah initialization
}



