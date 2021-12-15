#include "Lib.h"

bool compare(const char* a, string b)
{
	int la = strlen(a), lb = b.length();
	//cout << la << " - " << lb;
	if (la != lb) return 0;
	else
	{
		for (int i = 0; i < la; i++)
		{
			if (a[i] != b[i]) return 0;
		}
	}

	return 1;
}

float char2float(char* argv[], int n)
{
	float temp = strtod(argv[n], NULL);
	return temp;
}

bool isOdd(int n)
{
	return (n % 2 == 0) ? false : true;
}

int makeOdd(int n)
{
	return --n;
}

vector<int> kernelIndex(int widthStep, int width, int height)
{
	vector<int> index;
	int halfW = width / 2, halfH = height / 2;
	for (int h = -halfH; h <= halfH; h++)
	{
		for (int w = -halfW; w <= halfW; w++)
		{
			index.push_back(h*widthStep + w);
		}
	}
	return index;
}
