#include "globject2.h"

GLObject2::GLObject2()
{
}

void GLObject2::setUniform(std::string uniformName, progValue value)
{
    switch (value.type) {
    case progValue.VAL_FLOAT:
        mProgram.setUniformValue(uniformName, value.value.floatVal);
        break;
    case progValue.VAL_INT:
        mProgram.setUniformValue(uniformName, value.value.intVal);
        break;
    case progValue.VAL_MATRIX44:
        mProgram.setUniformValue(uniformName, value.value.intVal);
        break;
    }
}
