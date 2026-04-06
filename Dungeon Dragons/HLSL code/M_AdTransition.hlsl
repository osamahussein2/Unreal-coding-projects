float3 texture1 = Texture2DSample(Texture1, Texture1Sampler, uv);
float3 texture2 = Texture2DSample(Texture2, Texture2Sampler, uv);

float alpha = step((ceil(uv * transitionSmoothness) / transitionSmoothness) + time, 1.0f);
float3 adTransition = lerp(texture2, texture1, alpha);

return adTransition;