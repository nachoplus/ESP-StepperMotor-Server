//      *********************************************************************
//      *                                                                   *
//      *  ESP8266 and ESP32 Stepper Motor Server  - Stepper Config class   *
//      * a wrapper class to decouple the FlexyStepper class a bit better   *
//      *            Copyright (c) Paul Kerspe, 2019                        *
//      *                                                                   *
//      *********************************************************************
//
// MIT License
//
// Copyright (c) 2019 Paul Kerspe
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "ESPStepperMotorServer_StepperConfiguration.h"

ESPStepperMotorServer_StepperConfiguration::ESPStepperMotorServer_StepperConfiguration(const ESPStepperMotorServer_StepperConfiguration &espStepperConfiguration)
{
    this->_flexyStepper = new ESP_FlexyStepper;
    *this->_flexyStepper = *espStepperConfiguration._flexyStepper;

    this->_stepIoPin = espStepperConfiguration._stepIoPin;
    this->_directionIoPin = espStepperConfiguration._directionIoPin;
    this->_stepsPerMM = espStepperConfiguration._stepsPerMM;
    this->_stepsPerRev = espStepperConfiguration._stepsPerRev;
    this->_microsteppingDivisor = espStepperConfiguration._microsteppingDivisor;
    this->_displayName = espStepperConfiguration._displayName;
    this->_rpmLimit = espStepperConfiguration._rpmLimit;

    this->_flexyStepper->connectToPins(this->_stepIoPin, this->_directionIoPin);
}

ESPStepperMotorServer_StepperConfiguration::~ESPStepperMotorServer_StepperConfiguration()
{
    delete this->_flexyStepper;
}

//
// constructor for the stepper wrapper class
//
ESPStepperMotorServer_StepperConfiguration::ESPStepperMotorServer_StepperConfiguration(byte stepIoPin, byte directionIoPin)
{
    this->_stepIoPin = stepIoPin;
    this->_directionIoPin = directionIoPin;
    this->_flexyStepper = new ESP_FlexyStepper();
    this->_flexyStepper->connectToPins(this->_stepIoPin, this->_directionIoPin);
}

ESPStepperMotorServer_StepperConfiguration::ESPStepperMotorServer_StepperConfiguration(byte stepIoPin, byte directionIoPin, byte enableIoPin, String displayName, unsigned int stepsPerRev, unsigned int stepsPerMM, unsigned int microsteppingDivisor, unsigned int rpmLimit)
{
    this->_stepIoPin = stepIoPin;
    this->_directionIoPin = directionIoPin;
    this->_enableIoPin = enableIoPin;
    this->_microsteppingDivisor = microsteppingDivisor;
    this->_displayName = displayName;
    this->_rpmLimit = rpmLimit;

    this->_flexyStepper = new ESP_FlexyStepper();
    this->_flexyStepper->connectToPins(this->_stepIoPin, this->_directionIoPin);

    //Enable pin is optional, so we only set it if it is not the default value
    if (this->_enableIoPin != ESPServerStepperUnsetIoPinNumber)
    {
        this->_flexyStepper->setEnablePin(this->_enableIoPin,ESP_FlexyStepper::ACTIVE_HIGH);
    }

    //we store the value in flexistepper and locally, since flexystepper does not provider getters
    this->_flexyStepper->setStepsPerMillimeter(stepsPerMM * this->_microsteppingDivisor);
    this->_stepsPerMM = stepsPerMM;

    //we store the value in flexistepper and locally, since flexystepper does not provider getters
    this->_flexyStepper->setStepsPerRevolution(stepsPerRev * this->_microsteppingDivisor);
    this->_stepsPerRev = stepsPerRev;
}

// ---------------------------------------------------------------------------------
//                                  Getters / Setters
// ---------------------------------------------------------------------------------
ESP_FlexyStepper *ESPStepperMotorServer_StepperConfiguration::getFlexyStepper()
{
    return this->_flexyStepper;
}

void ESPStepperMotorServer_StepperConfiguration::setId(byte id)
{
    this->_stepperIndex = id;
}

byte ESPStepperMotorServer_StepperConfiguration::getId()
{
    return this->_stepperIndex;
}

String ESPStepperMotorServer_StepperConfiguration::getDisplayName()
{
    return this->_displayName;
}
void ESPStepperMotorServer_StepperConfiguration::setDisplayName(String displayName)
{
    if (displayName.length() > ESPSMS_Stepper_DisplayName_MaxLength)
    {
        char logString[160];
        sprintf(logString, "ESPStepperMotorServer_StepperConfiguration::setDisplayName: The display name for stepper with id %i is to long. Max length is %i characters. Name will be trimmed", this->getId(), ESPSMS_Stepper_DisplayName_MaxLength);
        ESPStepperMotorServer_Logger::logWarning(logString);
        this->_displayName = displayName.substring(0, ESPSMS_Stepper_DisplayName_MaxLength);
    }
    else
    {
        this->_displayName = displayName;
    }
}

byte ESPStepperMotorServer_StepperConfiguration::getStepIoPin()
{
    return this->_stepIoPin;
}

byte ESPStepperMotorServer_StepperConfiguration::getDirectionIoPin()
{
    return this->_directionIoPin;
}

byte ESPStepperMotorServer_StepperConfiguration::getEnableIoPin()
{
    return this->_enableIoPin;
}
//ENABLE 
void ESPStepperMotorServer_StepperConfiguration::setEnableIoPin(byte enablePin)
{
    this->_enableIoPin = enablePin;
    this->_flexyStepper->setEnablePin(enablePin);
}

// brake control settings

byte ESPStepperMotorServer_StepperConfiguration::getBrakeIoPin()
{
    return this->_brakeIoPin;
}

long ESPStepperMotorServer_StepperConfiguration::getBrakeEngageDelayMs()
{
    return this->_brakeEngageDelayMs;
}

long ESPStepperMotorServer_StepperConfiguration::getBrakeReleaseDelayMs()
{
    return this->_brakeReleaseDelayMs;
}

byte ESPStepperMotorServer_StepperConfiguration::getBrakePinActiveState()
{
    return this->_brakePinActiveState;
}

void ESPStepperMotorServer_StepperConfiguration::setBrakeIoPin(byte brakeIoPin, byte brakePinActiveState)
{
    this->_brakeIoPin = brakeIoPin;
    this->_brakePinActiveState = brakePinActiveState;
    this->_flexyStepper->setBrakePin(brakeIoPin, brakePinActiveState);
}

void ESPStepperMotorServer_StepperConfiguration::setBrakeEngageDelayMs(long delay)
{
    this->_brakeEngageDelayMs = delay;
    this->_flexyStepper->setBrakeEngageDelayMs(delay);
}

void ESPStepperMotorServer_StepperConfiguration::setBrakeReleaseDelayMs(long delay)
{
    this->_brakeReleaseDelayMs = delay;
    this->_flexyStepper->setBrakeReleaseDelayMs(delay);
}

void ESPStepperMotorServer_StepperConfiguration::setBrakePinActiveState(byte activeState)
{
    this->_brakePinActiveState = activeState;
    this->_flexyStepper->setBrakePin(this->_brakeIoPin, this->_brakePinActiveState);
}

// motion configurateion settings
void ESPStepperMotorServer_StepperConfiguration::setStepsPerRev(unsigned int stepsPerRev)
{
    this->_flexyStepper->setStepsPerRevolution(stepsPerRev * this->_microsteppingDivisor);
    this->_stepsPerRev = stepsPerRev;
}

unsigned int ESPStepperMotorServer_StepperConfiguration::getStepsPerRev()
{
    return this->_stepsPerRev;
}

void ESPStepperMotorServer_StepperConfiguration::setStepsPerMM(unsigned int stepsPerMM)
{
    this->_flexyStepper->setStepsPerMillimeter(stepsPerMM * this->_microsteppingDivisor);
    this->_stepsPerMM = stepsPerMM;
}

unsigned int ESPStepperMotorServer_StepperConfiguration::getStepsPerMM()
{
    return this->_stepsPerMM;
}

void ESPStepperMotorServer_StepperConfiguration::setMicrostepsPerStep(unsigned int microstepsPerStep)
{
    //check for power of two value, since others are not allowed in micro step sizes
    if (microstepsPerStep && !(microstepsPerStep & (microstepsPerStep - 1)))
    {
        this->_microsteppingDivisor = microstepsPerStep;
        //update flexy stepper as well in regards to steps/rev and steps/mm
        this->_flexyStepper->setStepsPerMillimeter(this->_stepsPerMM * this->_microsteppingDivisor);
        this->_flexyStepper->setStepsPerRevolution(this->_stepsPerRev * this->_microsteppingDivisor);
    }
    else
    {
        ESPStepperMotorServer_Logger::logWarningf("Invalid microstepping value given: %i. Only values which are power of two are allowed", microstepsPerStep);
    }
}

unsigned int ESPStepperMotorServer_StepperConfiguration::getMicrostepsPerStep()
{
    return this->_microsteppingDivisor;
}

void ESPStepperMotorServer_StepperConfiguration::setRpmLimit(unsigned int rpmLimit)
{
    if (rpmLimit > ESPSMS_MAX_UPPER_RPM_LMIT)
    {
        char logString[170];
        sprintf(logString, "ESPStepperMotorServer_StepperConfiguration::setRpmLimit: The given rpm limit value %u exceeds the allowed maximum rpm limit of %i, will set to %i", rpmLimit, ESPSMS_MAX_UPPER_RPM_LMIT, ESPSMS_MAX_UPPER_RPM_LMIT);
        ESPStepperMotorServer_Logger::logWarning(logString);
        this->_rpmLimit = ESPSMS_MAX_UPPER_RPM_LMIT;
    }
    else
    {
        this->_rpmLimit = rpmLimit;
    }
}

unsigned int ESPStepperMotorServer_StepperConfiguration::getRpmLimit()
{
    return this->_rpmLimit;
}
