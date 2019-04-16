/**
 * interface file for validator
 * 
 * created by vivaan bahl, vrbahl
 * last modified 4/15/2019
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

enum ValidatorState
{
    IDLE,
    EVAL_GOAL,
    INVALID
};

class Validator
{
private:
    ValidatorState current_state;
};

#endif