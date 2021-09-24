#include <Servo.h>

int limitDifference(int difference, int max) {
  if (abs(difference) > max) {
    return difference < 0 ? -max : max;
  } else {
    return difference;
  }
}

class Motors {
  private:
    const int MAX_PWM_ACCELERATION = 10;

    int pinLeft;
    int pinRight;
    Servo left;
    Servo right;
    int speedLeft;
    int speedRight;
    bool initialized;

  public:
    Motors(int pinLeft, int pinRight) {
      this->pinLeft = pinLeft;
      this->pinRight = pinRight;
      this->initialized = false;
    }

    void init() {
      this->left.attach(this->pinLeft);
      this->right.attach(this->pinRight);
      setSpeed(1500, 1500);
      delay(1000);
      initialized = true;
    }

    void setSpeed(int leftPWM, int rightPWM) {
      if (isEStopped == true)  {
        this->left.writeMicroseconds(1500);
        this->right.writeMicroseconds(1500);
        return;
      }
      // make sure it is in range
      if (leftPWM > 1900 || leftPWM < 1100) {
        // set speeds to whatever when not initialized
      } else if (!initialized) {
        this->speedLeft = leftPWM;
        this->left.writeMicroseconds(this->speedLeft);

        // if it is initialized, make sure the speed is less the the max
        // acceleration
      } else {
        int change = limitDifference(
                       leftPWM - speedLeft,
                       MAX_PWM_ACCELERATION
                     );
        this->speedLeft += change;
        this->left.writeMicroseconds(this->speedLeft);
      }


      // make sure it is in range
      if (rightPWM > 1900 || rightPWM < 1100) {

        // set speeds to whatever when not initialized
      } else if (!initialized) {
        this->speedRight = rightPWM;
        this->right.writeMicroseconds(this->speedRight);

        // if it is initialized, make sure the speed is less the the max
        // acceleration
      } else {
        int change = limitDifference(
                       rightPWM - speedRight,
                       MAX_PWM_ACCELERATION
                     );
        this->speedRight += change;
        this->right.writeMicroseconds(this->speedRight);
      }
      
      if (DEBUG) {
        Serial.print("• left: ");
        Serial.print(this->speedLeft);
        Serial.print(" right: ");
        Serial.println(this->speedRight);
      }
    }

};
