float2 tex = frac(float2(uv.x * grid.x + offset.x, uv.y * grid.y + offset.y));

float3 baseColor = ((step(float3(tex.xy, 0.0f), float3(dimensions.xy, 0.0f)) * otherColor) +
(step(float3(-dimensions.xy, 0.0f), float3(tex.xy, 0.0f)) * oneColor) +
(step(float3(dimensions.xy, 0.0f), float3(tex.xy, 0.0f)) * oneColor) +
(step(float3(tex.xy, 0.0f), float3(-dimensions.xy, 0.0f)) * otherColor)) *

((step(float3(tex.yx, 0.0f), float3(dimensions.yx, 0.0f)) * otherColor) +
(step(float3(-dimensions.yx, 0.0f), float3(tex.yx, 0.0f)) * oneColor) +
(step(float3(dimensions.yx, 0.0f), float3(tex.yx, 0.0f)) * oneColor) +
(step(float3(tex.yx, 0.0f), float3(-dimensions.yx, 0.0f)) * otherColor));

// Little colored squares
/*float3 baseColor = (step(float3(tex.xy, 0.0f), float3(dimensions.xy, 0.0f)) * otherColor) +
(step(float3(-dimensions.xy, 0.0f), float3(tex.xy, 0.0f)) * oneColor) +
(step(float3(dimensions.xy, 0.0f), float3(tex.xy, 0.0f)) * oneColor) +
(step(float3(tex.xy, 0.0f), float3(-dimensions.xy, 0.0f)) * otherColor) +

(step(float3(tex.yx, 0.0f), float3(dimensions.yx, 0.0f)) * otherColor) +
(step(float3(-dimensions.yx, 0.0f), float3(tex.yx, 0.0f)) * oneColor) +
(step(float3(dimensions.yx, 0.0f), float3(tex.yx, 0.0f)) * oneColor) +
(step(float3(tex.yx, 0.0f), float3(-dimensions.yx, 0.0f)) * otherColor);*/

return baseColor;