#pragma once



class Scale
{
public:

	static void SetFactor(float scaleFactor)
	{
		scale = scaleFactor;
	}
	static float Value(float num)
	{
		return num * scale;
	}

private:

	static float scale;
};
