#pragma once

#include "BasicStepperDriver.h"
#include <functional>



class Motion
{
private:
    BasicStepperDriver stepper; 
    std::function<void(bool)> onHomeStatusChanged = nullptr;

    void setSpeedControl(bool enabled);



    
    void setupHoming();
    bool isHomeSensorTriggered();
    void homingSensorTask();

     static TaskHandle_t homingSensorTaskHandle;
    inline  static void homingSensorTaskWrapper(void*);
    int currentContainer = 0;
    static void homingSensorIsr();
public:
    Motion(BasicStepperDriver stepper);

    void setup();
    void goToHome(bool forceHoming);
    void goToContainerAt(int index);
};
