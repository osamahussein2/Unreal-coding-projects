float result = 0;

for (int i = 0; i < numberOfCircles; i++)
{
	float angle = (i / numberOfCircles) * 2 * 3.14 + time * speed;
	float2 pos = (center + radius * float2(cos(angle), sin(angle)));

	result += step(length(pos - uv), size); // Draw circle

	// Only show the drawn circles
	opacityValue = step(size, result);
}

return result;