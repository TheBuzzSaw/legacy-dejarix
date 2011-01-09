#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <CGE/ClusterVBO.h>
#include <CGE/Texture2D.h>

class CardModel
{
    public:
        CardModel();
        ~CardModel();

        void display(GLuniform inSwitch, const CGE::Texture2D& inFront,
            const CGE::Texture2D& inBack);

    private:
        void build(float inWidth, float inHeight, float inDepth, float inRadius,
            int inDetail = 0);
        void buildSimple(float inWidth, float inHeight, float inDepth);

        CGE::ClusterVBO<2> mVBO;
        CGE::IndexVBO mTopIVBO;
        CGE::IndexVBO mMiddleIVBO;
        CGE::IndexVBO mBottomIVBO;
};

#endif
