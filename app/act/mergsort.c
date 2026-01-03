
 /* 将a开头的长为length的数组和b开头长为right的数组 合并 n为数组长度，用于最后一组 */
 #if 0
 void Merge(int* data, int a, int b, int length, int n)
 {
	int right;

	if(b+length-1 >= n-1) 
		right = n-b;
	else right = length;
	
 	int* temp = new int[length+right];
	 int i = 0, j = 0;
 	while(i<=length-1&&j<=right-1)
 	{
 		if(data[a+i] <= data[b+j])
 		{
 			temp[i+j] = data[a+i]; 
 			i++; 
 		}
 		else
 		{ 
 			temp[i+j] = data[b+j]; 
 			j++; 
 		}
 	}

 	if(j == right)
 	{
 		// a中还有元素，且全都比b中的大,a[i]还未使用
 		memcpy(data+a+i+j, data+a+i,(length-i)*sizeof(int));
 	}

 	memcpy(data+a, temp, (i+j)*sizeof(int) );
 	delete temp;
 }

 void MergeSort(int* data, int n)
 {
 	int step = 1;

 	while(step < n)
 	{
 		for(int i = 0; i <= n-step-1; i += 2*step)
 			Merge(data, i, i+step, step, n);
		 // 将i和i+step这两个有序序列进行合并
		 // 序列长度为step
		 // 当i以后的长度小于或者等于step时，退出
		 step *= 2;
 	}
 }
 #endif
 
