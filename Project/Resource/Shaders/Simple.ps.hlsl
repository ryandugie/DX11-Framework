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

struct InputData {
  float4 position : SV_POSITION;
  float4 color : COLOR;
};

float4 main(InputData input) : SV_TARGET {
    return input.color;
}