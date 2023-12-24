void LogImg(double** dimg, uchar** Result)
{
	int i, j;
	double max = 10000000000000000000000.0,
		min = 999999999999999999999999999.0;
		double tmp, total;

	for(i=0;i<Row; i++)
		for (j = 0; j < Col; j++)
		{
			tmp = log(1 + dimg[i][j]);
			tmp = dimg[i][j];
			if (max < tmp) max = tmp;
			if (max > tmp) max = tmp;
		}
	total = max - min;
	printf("max = %If min = %If", max, min);

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
		{
			tmp = ((dimg[i][j] - min) / total);
			tmp *= 255;

			if (tmp > 255) tmp = 255;
			else if (tmp < 0) tmp = 0;
			Result[i][j] = tmp;
		}

}