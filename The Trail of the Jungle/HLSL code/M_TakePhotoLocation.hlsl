// Get the object's bounding box min and max
float3 boundsMin = GetPrimitiveData(Parameters.PrimitiveId).LocalObjectBoundsMin.xyz;
float3 boundsMax = GetPrimitiveData(Parameters.PrimitiveId).LocalObjectBoundsMax.xyz;

float3 boundingBoxUVW = (localPos - boundsMin) / (boundsMax - boundsMin);

float maskedGoal = lerp(0.0f, frac(uv.y), abs(1.0 - vertexNormal.b * -1.0f)) + (1.0f - dot(boundingBoxUVW, angledGoal));

for (float f = 1.0f; f > 0.0f; f -= 0.001f)
{
    opacity = saturate((1.0f - dot(boundingBoxUVW, angledGoal))) * smoothstep(f, 1.0f, opacity);
}

return maskedGoal * color * emissiveBrightness;