#include "vex.h"
#include <cmath>
using namespace vex;

// A global instance of vex::brain used for printing to the IQ2 brain screen
vex::brain       Brain;

motor baseMotor(PORT3, false);
motor flyWheel(PORT1, false);
motor feedMotor(PORT2, true);

const double BASE_GEAR_RATIO = 4.0; //motor 4x, gear 1x  
const double BASE_ROTATE_SPEED = 30.0;
const int SETTLE_MSEC = 150; //rotation must settle for 150 msec before shooting out card 
const int SETTLE_TIMEOUT_MSEC = 2000;
const double FEED_CARD_ROTATION = 400.0; //how much does the feeder has to rotate to push out one card 
const double FLYWHEEL_SPEED = 100.0;
const int CARD_CLEAR_MSEC = 200; //pause time after card is shot out  
const double TOLERANCE_DEG = 3.0; //how far off the target base motor is allowed to be
const int numPlayers = 4; //this value will become user input later in proj development 

volatile bool stopRequested = false;


//called when up button is pressed 
void onUpPressed() {
    stopRequested = true;
}

//converts a player's seat position to motor encoder angles 
double numDealFor(int playerCounter, int totalPlayers) {
    double turnBaseDegrees = playerCounter * (360.0 / totalPlayers);
    return turnBaseDegrees * BASE_GEAR_RATIO;
}

//moves the base to "target" and returns true upon arrival 
bool rotateTo(double target) {
    baseMotor.spinToPosition(target, degrees, false);
    while (!baseMotor.isDone()) {
        if (stopRequested) {
            baseMotor.stop();
            return false;
        }
        wait(10, msec);
    }
    return true;
}

//waits until the base has stopped before dealing cards 
bool waitUntilSettled(double target) {
    timer Timer;
    int stableMsec = 0;

    while (stableMsec < SETTLE_MSEC) {
        if (Timer.time(msec) > SETTLE_TIMEOUT_MSEC) {
            return false;
        }
        bool onTarget = std::fabs(baseMotor.position(degrees) - target) < TOLERANCE_DEG;
        bool still = std::fabs(baseMotor.velocity(rpm)) < 1.0;

        if (onTarget && still) {
            stableMsec = stableMsec + 10;
        } else {
            stableMsec = 0;
        }
        wait(10, msec);
    }
    return true;
}

bool dealOneCard() {
    feedMotor.setPosition(0, degrees);
    feedMotor.spinToPosition(FEED_CARD_ROTATION, degrees, false);
    while (!feedMotor.isDone()) {
        if (stopRequested) {
            feedMotor.stop(brake);
            return false;
        }
        wait(20, msec);
    }
    wait(CARD_CLEAR_MSEC, msec);
    return true;
}

void initRobot() {
    baseMotor.setStopping(hold);
    baseMotor.setVelocity(BASE_ROTATE_SPEED, percent);
    baseMotor.setPosition(0, degrees);

    feedMotor.setStopping(hold);

    Brain.buttonUp.pressed(onUpPressed);

    flyWheel.spin(forward, FLYWHEEL_SPEED, percent);
    wait(500, msec);
}

void shutdown() {
    feedMotor.stop();
    flyWheel.stop();
    stopRequested = false;
    rotateTo(numDealFor(0, numPlayers));
}


int main() {
    initRobot();

    int player = 0;
    bool ok = true;

    while (ok && !stopRequested) {
        double target = numDealFor(player, numPlayers);
        ok = rotateTo(target) && waitUntilSettled(target) && dealOneCard();
        player = (player + 1) % numPlayers;
    }

    shutdown();
    return 0;
}