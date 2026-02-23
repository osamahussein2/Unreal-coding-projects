float2 normalizedUV = float2(uv.x - center.x, (uv.y * -1.0) + center.y);

// Calculate angle (0 to 1 range)
float angle = atan2(normalizedUV.y, normalizedUV.x) / (2.0 * 3.14159) + 0.5;

// Adjust starting angle to change the direction of starting radial value
// Apply offset and wrap around circle

// fmod method in HLSL returns a remainder of x/y as a float value
angle = fmod(angle + (0.5f * currentHealthAngle), 1);

// Calculate distance from center
float dist = length(normalizedUV) * 2.0;

// Create masks
float radialMask = step(angle, saturate(percentage) * 0.5f);
float ringMask = step(1.0f - thickness, dist) * step(dist, 1.0f);

opacityValue = radialMask * ringMask;
return radialMask * ringMask * color;