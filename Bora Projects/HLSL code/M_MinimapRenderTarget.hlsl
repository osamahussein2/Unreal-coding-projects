// Subtract the length of the UV coordinates by the center point of the UV coordinates
// Then subtract that resulting length by the radius to draw a circle
circle = length(uv - center) - radius;

// If the circle is below a certain point, add opacity mask to it
if (circle.x <= thresholdCoordinate && circle.y <= thresholdCoordinate)
{
	circle = (length(uv - center) - radius) + 1;
}

// Just return the texture sampler of the minimap texture
return texSampler;