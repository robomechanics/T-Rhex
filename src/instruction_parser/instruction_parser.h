/**
 * 
 * interface file for the instruction parser
 * 
 * Created by vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 */

enum InstructionState
{
    IDLE,
    GET_INST,
    PARSE_GOAL,
    PARSE_WAIT,
    VALIDATE,
    KEEP_INST,
    INVALID
};

class InstructionParser
{
private:
    InstructionState current_state;
};