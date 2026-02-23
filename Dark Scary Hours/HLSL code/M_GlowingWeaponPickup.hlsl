// Draw circle
float result = step(length(uv - center), size);

// Only show the drawn circle (in decreasing opacity color depending on distance between the uv and center of the circle)
for (float f = 1.0f; f > 0.0f; f -= opacityDecrementValue)
{
    opacity += result * smoothstep(size * 1.0f, size * f, length(uv - center));
}

return result * emissiveColor * opacity * brightness;