// Plug this return node into UVs for SceneTexture:PostProcessInput0
return floor(ScreenPosition.xy * floor(scale)) / floor(scale);

// Plug in SceneTexture:PostProcessInput0 color node inside of PostProcessTexture
return ceil(PostProcessTexture * Multiplier) / Multiplier;