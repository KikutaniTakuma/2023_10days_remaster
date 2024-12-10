#include "WaterTex2D.hlsli"

ConstantHullShaderOutPut ConstantsHS_Main( InputPatch<VertexShaderOutputToHull, 3> input, uint32_t PatchID : SV_PrimitiveID )
{
  ConstantHullShaderOutPut output;
  uint32_t instanceID = input[0].instanceID;

  output.Edges[0] = output.Edges[1] = output.Edges[2] = kWaterData[instanceID].edgeDivision;
  output.Inside = kWaterData[instanceID].insideDivision;

  return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantsHS_Main")]
HullShaderOutPut main( InputPatch<VertexShaderOutputToHull, 3> input, uint32_t i : SV_OutputControlPointID, uint32_t PatchID : SV_PrimitiveID ) {
	HullShaderOutPut output;
	uint32_t ID = 2 - i;

	output.position = input[ID].position;
	output.normal = input[ID].normal;
	output.worldPosition = input[ID].worldPosition;
	output.uv = input[ID].uv;
	output.textureID = input[ID].textureID;
	output.instanceID = input[ID].instanceID;

	return output;
}