struct RadialBar
{
    float DrawFullRadialBar(float2 uv_, float startAngleOffset_, float progress_, float radius_, float radiusOffset_)
    {
        // Normalize UVs to the center of the texture coordinate 
		// Y coordinate is inverted here to start the radial bar at the top
        float2 normalizedUV = float2(uv_.x - 0.5, (uv_.y * -1.0) + 0.5);

		// Calculate angle in radians [-3.14, 3.14]
        float angle = atan2(normalizedUV.y, normalizedUV.x);

		// Convert the angle to a normalized range between -0.5 and 0.5
		// Remap angle for the progress bar by incrementing it between 0 and 1
        angle = angle * (1.0 / (2.0 * 3.14159)) + 0.5f;

		// Adjust starting angle to change the direction of starting radial value
		// Apply offset and wrap around circle

		// fmod method in HLSL returns a remainder of x/y as a float value
        angle = fmod(angle + startAngleOffset_, 1);

		// Create the mask and clamp progress between 0 and 1 by using the saturate method in HLSL
		//float mask = step(angle, saturate(progress_));
        float mask = step(angle, saturate(progress_) * 0.5);

		// Create thickness by masking an inner circle
        float dist = length(normalizedUV);

		// Step through the radius and distance from the center (where we take the length of the normalized UVs)
		// Also step through the added radius offset so that the outer circle is visible
        float circleMask = step(radius_, dist) * step(dist, radius_ + radiusOffset_);

		// Return mask multipled by the circle mask
        return mask * circleMask;
    }
};

RadialBar radialBar;

// Output the masked circle as a radial shape
circle = radialBar.DrawFullRadialBar(uv, startAngleOffset, progress, radius, radiusOffset);

// Return the texture color we're looking for
return textureColor;