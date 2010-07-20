#include "CardProgram.h"

CardProgram::CardProgram() : ShaderProgram(2)
{
    attachShader("card.vs");
    attachShader("card.fs");

    glBindAttribLocation(handle(), VERTEX, "CardVertex");
    glBindAttribLocation(handle(), TEXTURE, "CardTextureCoordinate");
    linkAndBind();
    glUniform1i(mUniformCardTexture, 0);
}

CardProgram::~CardProgram()
{
}

void CardProgram::bindUniforms()
{
    mUniformUseTexture = glGetUniformLocation(handle(), "UseTexture");
    mUniformCardColor = glGetUniformLocation(handle(), "CardColor");
    mUniformCardTexture = glGetUniformLocation(handle(), "CardTexture");
}
