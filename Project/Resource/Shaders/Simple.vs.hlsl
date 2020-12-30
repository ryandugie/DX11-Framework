/****************************************************************************/
/*!
\file
   Simple.vs.hlsl
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0
*/
/****************************************************************************/

cbuffer Matrices : register( b0 ) {
    matrix worldMatrix;
    matrix projectionMatrix;
    matrix viewMatrix;
};

struct OutData {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct InData {
    float4 position : POSITION;
    float4 normal : NORMAL;
};

OutData main(InData inData) {
    OutData outData;
    
    outData.position = mul(mul(projectionMatrix, mul(viewMatrix, worldMatrix)), inData.position);
    outData.color = inData.normal;

    return outData;
}