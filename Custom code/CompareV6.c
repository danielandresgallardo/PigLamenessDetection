#include <string.h>
#include <stdio.h>

int main() {
	//================================Variable declaration==========================================
	char steps[] = "D:/project/darknet/Release/csv/steps.csv";        		//steps csv file save location
	char frames[] = "D:/project/darknet/Release/csv/frames.csv";        	//frames csv file save location
	char output1[] = "D:/project/darknet/Release/csv/output1.csv";      	//output1 csv file save location
	char output2[] = "D:/project/darknet/Release/csv/output2.csv";      	//output2 csv file save location
	char finaloutput1[] = "D:/project/darknet/Release/csv/finaloutput.csv"; //finaloutput1 csv file save location
	char totalFootSize[] = "D:/project/darknet/Release/csv/totalFootSize.csv"; //totalFootSize csv file save location
	char Result[] = "D:/project/darknet/Release/csv/Result.csv"; //stepLengthAvg csv file save location
	char stepCounting[] = "D:/project/darknet/Release/csv/stepCounting.csv"; //stepCounting csv file save location
	FILE* stepPointer = fopen(steps, "r");                            	//steps pointer
	FILE* framesPointer = fopen(frames, "r");                           //frames pointer
	FILE* output1Pointer = fopen(output1, "w");                         //output1 pointer
	FILE* output2Pointer = fopen(output2, "w");                         //output2 pointer
	FILE* finaloutput1Pointer = fopen(finaloutput1, "w");               //finaloutput1 pointer
	FILE* totalFootSizePointer = fopen(totalFootSize, "r");             //totalFootSize pointer
	FILE* ResultPointer = fopen(Result, "a");             //stepLengthAvg pointer
	FILE* stepCountingPointer = fopen(stepCounting, "a");             //stepCounting pointer
	
	char temp[11];														//Total frames and position of one leg
	int totalframes;													//Number of frames in video
	int totalFootSize1;
	int totalBoxes;
	while (fgets(temp, 10, framesPointer))				//Get amount of frames from csv
	{
		totalframes = atoi(temp);
		printf("%d\n", totalframes);
	}
	fclose(framesPointer);
	int saved[totalframes][4][2];
	int output2fliped[totalframes][4][2];
	int finaloutput[totalframes][4][2];
	int FLD = 0;		//First legs detected
	int LPF = 0;		//Legs per frame, (4 in one frame)
	int frame = 0;		//Frame number
	int legs[4][2];		//inputed legs in one frame (FR, FL, RR, RL)
	int legstemp[4][2] = { {0,0},{0,0},{0,0},{0,0} };;
	int compare[4][2] = { {0,0},{0,0},{0,0},{0,0} };	//legs from last frame
	int last[4][2] = { {0,0},{0,0},{0,0},{0,0} };		//last time leg was seen
	int lastCount[4] = {0, 0, 0, 0};					//last frame seen
	int legcount = 0;
	char* token;
	char* search = ",";
	while (fgets(temp, 11, totalFootSizePointer))		//Calculate foot width (Every foot width added up / total amount of feet)
	{
		
		// Token will point to x.
        token = strtok(temp, search);
		totalFootSize1 = atoi(token);
		
       	// Token will point to y.
       	token = strtok(NULL, search);
		totalBoxes = atoi(token);
		totalFootSize1 = totalFootSize1/totalBoxes;
	}
	fclose(totalFootSizePointer);
	int firstleg = 0;
	int direction = 0;	// 1 = left to right, 2 = right to left
	int midpoint = 0;
	int backfoot = 0;
	int vartemp = totalFootSize1;
	int skipedframes[4] = {0, 0, 0, 0};
	int i;
	int x;
	int u;	//fist frame with midpoint
	
	int j;
	int z;
	//============================End of variable declaration=======================================
	while (fgets(temp, 10, stepPointer))
 	{
		//printf("temp = %s\n", temp);
		
 		if(LPF == 0)
 		{
 			printf("=========== Frame %d ===========\n", frame+1);
 			printf("\n= Input = \n");
 			frame ++;
		}
		
		
		if (temp[0] != 'n') //check if there is a foot detected
    	{
        	char temp2[10];

        	// Token will point to x.
        	token = strtok(temp, search);

			if(atoi(token) != 0)
			{
        		legs[LPF][0] = atoi(token);

        		// Token will point to y.
        		token = strtok(NULL, search);

        		legcount++;
        		legs[LPF][1] = atoi(token);

        		printf("x = %d, y = %d\n", legs[LPF][0], legs[LPF][1]);
        		saved[frame-1][LPF][0] = legs[LPF][0];
        		saved[frame-1][LPF][1] = legs[LPF][1];
			}
			else
			{
				LPF--;
			}
    	}
    	else //if no foot detected, print null
    	{
        	printf("null\n");
        	legs[LPF][0] = 0;
        	legs[LPF][1] = 0;
        	saved[frame-1][LPF][0] = 0;
        	saved[frame-1][LPF][1] = 0;
    	}
    	
    	//check if already 4 legs per frame, then set lpf to 0 and sort front rear left and right
		if(LPF == 3)
		{
			printf(" Legcount = %d", legcount);
        	printf("\n\n= compare (previouse frame) =\n\n %d, %d\n %d, %d\n %d, %d\n %d, %d\n\n", compare[0][0], compare[0][1], compare[1][0], compare[1][1], compare[2][0], compare[2][1], compare[3][0], compare[3][1]);
    	
        	//printf("\nlegcount = %d\n", legcount);
        	//printf("\nFirst algorithm =");
			LPF = 0;
			
			//check if first two legs were seen or not
			if(FLD==0)
			{
				if(legcount == 1 && firstleg == 0)	//save first leg time
				{
					firstleg = frame;
				}
				if(legcount == 2)	//if both front legs are seen
				{
					if(direction == 0 && legs[0][0] < legs[1][0])	//set direction, right to left
					{
						direction = 2;
					}
					else if (direction == 0 && legs[0][0] > legs[1][0])	//set direction, left to right
					{
						direction = 1;
					}
					
					
					if(direction == 1)	//left to right
					{
                        if (legs[0][1] < legs[1][1])
                        {
                            compare[0][0] = legs[1][0];
                            compare[0][1] = legs[1][1];

                            compare[1][0] = legs[0][0];
                            compare[1][1] = legs[0][1];
                        }
                        else
                        {
                            compare[0][0] = legs[0][0];
                            compare[0][1] = legs[0][1];

                            compare[1][0] = legs[1][0];
                            compare[1][1] = legs[1][1];
						}
					}
					else if (direction == 2)	//right to left
					{
                        if (legs[0][1] > legs[1][1])
                        {
                            compare[0][0] = legs[1][0];
                            compare[0][1] = legs[1][1];

                            compare[1][0] = legs[0][0];
                            compare[1][1] = legs[0][1];
                        }
                        else
                        {
                            compare[0][0] = legs[0][0];
                            compare[0][1] = legs[0][1];

                            compare[1][0] = legs[1][0];
                            compare[1][1] = legs[1][1];
						}
					}
					FLD = 1;
					legcount = 0;
					//printf("\nFR = %d, %d\nFL = %d, %d\n\n",compare[0][0], compare[0][1], compare[1][0], compare[1][1]);
				}
			}
			else //First two legs already detected
			{
				if(legcount == 4)	//4 legs in one frame
				{
					midpoint = 1;
					u = frame-1;
					if (direction == 1)	//left to right
					{
                        if (legs[0][1] < legs[1][1])
                        {
                            compare[0][0] = legs[1][0];
                            compare[0][1] = legs[1][1];

                            compare[1][0] = legs[0][0];
                            compare[1][1] = legs[0][1];
                        }
                        else
                        {
                            compare[0][0] = legs[0][0];
                            compare[0][1] = legs[0][1];

                            compare[1][0] = legs[1][0];
                            compare[1][1] = legs[1][1];
						}
						
						if (legs[2][1] < legs[3][1])
                        {
                            compare[2][0] = legs[3][0];
                            compare[2][1] = legs[3][1];

                            compare[3][0] = legs[2][0];
                            compare[3][1] = legs[2][1];
                        }
                        else
                        {
                            compare[2][0] = legs[2][0];
                            compare[2][1] = legs[2][1];

                            compare[3][0] = legs[3][0];
                            compare[3][1] = legs[3][1];
						}
					}
					else if (direction == 2)	//right to left
					{
                        if (legs[0][1] > legs[1][1])
                        {
                            compare[0][0] = legs[1][0];
                            compare[0][1] = legs[1][1];

                            compare[1][0] = legs[0][0];
                            compare[1][1] = legs[0][1];
                        }
                        else
                        {
                            compare[0][0] = legs[0][0];
                            compare[0][1] = legs[0][1];

                            compare[1][0] = legs[1][0];
                            compare[1][1] = legs[1][1];
						}
						
						if (legs[2][1] > legs[3][1])
                        {
                            compare[2][0] = legs[3][0];
                            compare[2][1] = legs[3][1];

                            compare[3][0] = legs[2][0];
                            compare[3][1] = legs[2][1];
                        }
                        else
                        {
                            compare[2][0] = legs[2][0];
                            compare[2][1] = legs[2][1];

                            compare[3][0] = legs[3][0];
                            compare[3][1] = legs[3][1];
						}
					}
					legcount = 0;
				}
				else	//less than 4 legs
				{
					for(i = 0; i < 4; i++)	//Compare to previous frame
					{
						for(x = 0; x < 4; x++)
						{
							if(compare[x][0] != 0 && legstemp[x][0] == 0)
							{
								if(legs[i][0] < compare[x][0]+vartemp && legs[i][0] > compare[x][0]-vartemp)
								{
									if(legs[i][1] < compare[x][1]+35 && legs[i][1] > compare[x][1]-35)
									{
										legstemp[x][0] = legs[i][0];
										legstemp[x][1] = legs[i][1];
										legs[i][0] = 0;
										legs[i][1] = 0;
										legcount--;
									}
								}
							}
						}
					}
					for(i = 0; i < 4; i++)	//Compare to last time seen
					{
						for(x = 0; x < 4; x++)
						{
							if(last[x][0] != 0 && legstemp[x][0] == 0)
							{
								if(legs[i][0] < last[x][0]+vartemp && legs[i][0] > last[x][0]-vartemp)
								{
									if(legs[i][1] < last[x][1]+50 && legs[i][1] > last[x][1]-50)
									{
										if(frame-lastCount[x] < 9)
										{
											legstemp[x][0] = legs[i][0];
											legstemp[x][1] = legs[i][1];
											legs[i][0] = 0;
											legs[i][1] = 0;
											legcount--;
										}
									}
								}
							}
						}
					}
					
					for(i = 0; i<4; i++)
					{
						compare[i][0] = legstemp[i][0];
						compare[i][1] = legstemp[i][1];
						legstemp[i][0] = 0;
						legstemp[i][1] = 0;
					}
					
					/*
					for(i = 0; i<4; i++)
					{
						if(legstemp[i][0] == 0)
						{
							skipedframes[i]++;
							if(skipedframes > 2)
							{
								compare[i][0] = 0;
								compare[i][1] = 0;
							}
						}
						else
						{
							skipedframes[i] = 0;
							compare[i][0] = legstemp[i][0];
							compare[i][1] = legstemp[i][1];
							legstemp[i][0] = 0;
							legstemp[i][1] = 0;
						}
					}*/
					
					
				}
				//printf("\nCompare\nFR = %d, %d\nFL = %d, %d\nRR = %d, %d\nRL = %d, %d\n\n", compare[0][0], compare[0][1], compare[1][0], compare[1][1], compare[2][0], compare[2][1], compare[3][0], compare[3][1]);
			}
        	//printf("legcount = %d\n", legcount);
			
        	//printf("\nSecond algorithm =\n");
        	
        	for(i=0;i<4;i++)	//Check if current position is the next step of last time seen
        	{
        		if(legs[i][0] != 0)
        		{
        			for(x=0;x<4;x++)
        			{
        				if(direction == 1)	//left to right
        				{
        					if(legs[i][0] > last[x][0] && compare[x][0] == 0 && last[x][0] != 0)
        					{
        						if(frame-lastCount[x] < 12)
        						{
        							compare[x][0] = legs[i][0];
        							compare[x][1] = legs[i][1];
									legs[i][0] = 0;
									legs[i][1] = 0;
									legcount--;
								}
							}
						}
						else if(direction == 2)	//right to left
						{
							if(legs[i][0] < last[x][0] && compare[x][0] == 0 && last[x][0] != 0)
        					{
        						if(frame-lastCount[x] < 12)
        						{
        							compare[x][0] = legs[i][0];
        							compare[x][1] = legs[i][1];
									legs[i][0] = 0;
									legs[i][1] = 0;
									legcount--;
								}
							}
						}
					}
				}
			}
			
			if(midpoint == 0 && legcount == 2)	//if the two rear legs are in frame (two unknown legs)
			{
				midpoint = 1;
				int rearlegs[2][2];
				x = 0;
				for(i = 0; i<4; i++)
				{
					if(legs[i][0] != 0 && x != 2)
					{
						rearlegs[x][0] = legs[i][0];
						rearlegs[x][1] = legs[i][1];
						legs[i][0] = 0;
						legs[i][1] = 0;
						x++;
						legcount--;
					}
				}
				if(direction == 1)
					{
                        if (rearlegs[0][1] < rearlegs[1][1])
   		                {
           		            compare[2][0] = rearlegs[1][0];
               			    compare[2][1] = rearlegs[1][1];

                       		compare[3][0] = rearlegs[0][0];
                            compare[3][1] = rearlegs[0][1];
   		                }
           		        else
   		                {
           		            compare[2][0] = rearlegs[0][0];
                   		    compare[2][1] = rearlegs[0][1];

                            compare[3][0] = rearlegs[1][0];
   		                    compare[3][1] = rearlegs[1][1];
						}
					}
					else if(direction == 2)
					{
                        if (rearlegs[0][1] > rearlegs[1][1])
   		                {
           		            compare[2][0] = rearlegs[1][0];
               			    compare[2][1] = rearlegs[1][1];

                       		compare[3][0] = rearlegs[0][0];
                            compare[3][1] = rearlegs[0][1];
   		                }
           		        else
   		                {
           		            compare[2][0] = rearlegs[0][0];
                   		    compare[2][1] = rearlegs[0][1];

                            compare[3][0] = rearlegs[1][0];
   		                    compare[3][1] = rearlegs[1][1];
						}
					}
					rearlegs[0][0] = 0;
					rearlegs[0][1] = 0;
					rearlegs[1][0] = 0;
					rearlegs[1][1] = 0;
			}
			
			
        	printf("\n\n= last =\n\n %d, %d\n %d, %d\n %d, %d\n %d, %d\n\n", last[0][0], last[0][1], last[1][0], last[1][1], last[2][0], last[2][1], last[3][0], last[3][1]);
			printf("\n= Output =\nFR = %d, %d\nFL = %d, %d\nRR = %d, %d\nRL = %d, %d\n\n", compare[0][0], compare[0][1], compare[1][0], compare[1][1], compare[2][0], compare[2][1], compare[3][0], compare[3][1]);
        	
			fprintf(output1Pointer,"%d,%d\n%d,%d\n%d,%d\n%d,%d\n",compare[0][0], compare[0][1], compare[1][0], compare[1][1], compare[2][0], compare[2][1], compare[3][0], compare[3][1]);

        	finaloutput[frame-1][0][0] = compare[0][0];
        	finaloutput[frame-1][0][1] = compare[0][1];
        	
        	finaloutput[frame-1][1][0] = compare[1][0];
        	finaloutput[frame-1][1][1] = compare[1][1];
        	
        	finaloutput[frame-1][2][0] = compare[2][0];
        	finaloutput[frame-1][2][1] = compare[2][1];
        	
        	finaloutput[frame-1][3][0] = compare[3][0];
        	finaloutput[frame-1][3][1] = compare[3][1];
			
			for(i=0;i<4;i++)
        	{
        		if(compare[i][0] != 0)
        		{
        			last[i][0] = compare[i][0];
        			last[i][1] = compare[i][1];
        			lastCount[i] = frame;
				}
			}
			legcount = 0;
		}
		else
		{
			LPF++;
		}
	}
	fclose(stepPointer);
	frame = 0;
	LPF = 0;
	FLD = 0;
	legcount = 0;
	firstleg = 0;
	midpoint = 0;
	
	int legtemp[4][2];
	int legstemp2[4][2] = { {0,0},{0,0},{0,0},{0,0} };
	int last2[4][2] = { {0,0},{0,0},{0,0},{0,0} };		//last time leg was seen
	int lastCount2[4] = {0, 0, 0, 0};
	int compare2[4][2] = { {0,0},{0,0},{0,0},{0,0} };	//legs from last frame
	
	for (j=totalframes-1; j>=0; j--)
	{
		printf("=========== Frame %d ===========\n", j);
 		printf("Direction = %d\n", direction);
 		printf("FLD = %d\n", FLD);
 		printf("\n= Input = \n");
 		frame = j;
 		legcount = 0;
		for(z=0; z<4; z++)
		{
			legs[z][0] = saved[j][z][0];
	    	legs[z][1] = saved[j][z][1];
	    	if(saved[j][z][0] != 0)
			{
				legcount++;
			} 
		}
		for(z=0; z<legcount; z++)
		{
			legtemp[z][0] = saved[j][z][0];
			legtemp[z][1] = saved[j][z][1];
		}
		for(z=0; z<legcount	; z++)
		{
			saved[j][legcount-z-1][0] = legtemp[z][0];
			saved[j][legcount-z-1][1] = legtemp[z][1];
		}
	    printf("x = %d, y = %d\n", saved[j][0][0], saved[j][0][1]);
	    printf("x = %d, y = %d\n", saved[j][1][0], saved[j][1][1]);
	    printf("x = %d, y = %d\n", saved[j][2][0], saved[j][2][1]);
	    printf("x = %d, y = %d\n", saved[j][3][0], saved[j][3][1]);
	    
	    //============================================ Code in reverse ============================================
	    
	    printf("\n\n= compare (previouse frame) =\n\n %d, %d\n %d, %d\n %d, %d\n %d, %d\n\n", compare2[0][0], compare2[0][1], compare2[1][0], compare2[1][1], compare2[2][0], compare2[2][1], compare2[3][0], compare2[3][1]);
	    
	    if(FLD==0)
	    {
			if(legcount == 1 && firstleg == 0)	//save first leg time
			{
				firstleg = frame;
			}
			
			if(legcount == 2)	//if both front legs are seen
			{
				if(direction == 1)	//left to right
				{
                    if (saved[j][0][1] < saved[j][1][1])
                    {
                        compare2[2][0] = saved[j][1][0];
                        compare2[2][1] = saved[j][1][1];
                        compare2[3][0] = saved[j][0][0];
                        compare2[3][1] = saved[j][0][1];
                    }
                    else
                    {
                        compare2[2][0] = saved[j][0][0];
                        compare2[2][1] = saved[j][0][1];
                        compare2[3][0] = saved[j][1][0];
                        compare2[3][1] = saved[j][1][1];
					}
				}
				else if (direction == 2)	//right to left
				{
                    if (saved[j][0][1] > saved[j][1][1])
                    {
                        compare2[2][0] = saved[j][1][0];
                        compare2[2][1] = saved[j][1][1];
                        compare2[3][0] = saved[j][0][0];
                        compare2[3][1] = saved[j][0][1];
                    }
                    else
                    {
                        compare2[2][0] = saved[j][0][0];
                        compare2[2][1] = saved[j][0][1];
                        compare2[3][0] = saved[j][1][0];
                        compare2[3][1] = saved[j][1][1];
					}
				}
				FLD = 1;
				legcount = 0;
				//printf("\nFR = %d, %d\nFL = %d, %d\n\n",compare[0][0], compare[0][1], compare[1][0], compare[1][1]);
				output2fliped[j][0][0] = 0;
				output2fliped[j][0][1] = 0;
				output2fliped[j][1][0] = 0;
				output2fliped[j][1][1] = 0;
				output2fliped[j][2][0] = compare2[2][0];
				output2fliped[j][2][1] = compare2[2][1];
				output2fliped[j][3][0] = compare2[3][0];
				output2fliped[j][3][1] = compare2[3][1];
				printf("\n= Output compare 2 =\nFR = %d, %d\nFL = %d, %d\nRR = %d, %d\nRL = %d, %d\n\n", output2fliped[j][0][0], output2fliped[j][0][1], output2fliped[j][1][0], output2fliped[j][1][1], output2fliped[j][2][0], output2fliped[j][2][1], output2fliped[j][3][0], output2fliped[j][3][1]);
			}
		}
	    else //First two legs already detected
		{
			if(legcount == 4) //if 4 legs
			{
				midpoint = 1;
				if (direction == 1)	//left to right
				{
                    if (saved[j][0][1] < saved[j][1][1])
                    {
                        compare2[2][0] = saved[j][1][0];
                        compare2[2][1] = saved[j][1][1];
                        
						compare2[3][0] = saved[j][0][0];
                        compare2[3][1] = saved[j][0][1];
                    }
                    else
                    {
                        compare2[2][0] = saved[j][0][0];
                        compare2[2][1] = saved[j][0][1];

                        compare2[3][0] = saved[j][1][0];
                        compare2[3][1] = saved[j][1][1];
					}
						
					if (saved[j][2][1] < saved[j][3][1])
                    {
                        compare2[0][0] = saved[j][3][0];
                        compare2[0][1] = saved[j][3][1];

                        compare2[1][0] = saved[j][2][0];
                        compare2[1][1] = saved[j][2][1];
                    }
                    else
                    {
                        compare2[0][0] = saved[j][2][0];
                        compare2[0][1] = saved[j][2][1];

                        compare2[1][0] = saved[j][3][0];
                        compare2[1][1] = saved[j][3][1];
					}
				}
				else if (direction == 2)	//right to left
				{
                    if (saved[j][0][1] > saved[j][1][1])
                    {
                        compare2[2][0] = saved[j][1][0];
                        compare2[2][1] = saved[j][1][1];

                        compare2[3][0] = saved[j][0][0];
                        compare2[3][1] = saved[j][0][1];
                    }
                    else
                    {
                        compare2[2][0] = saved[j][0][0];
                        compare2[2][1] = saved[j][0][1];

                        compare2[3][0] = saved[j][1][0];
                        compare2[3][1] = saved[j][1][1];
					}
						
					if (saved[j][2][1] > saved[j][3][1])
                    {
                        compare2[0][0] = saved[j][3][0];
						compare2[0][1] = saved[j][3][1];
						
						compare2[1][0] = saved[j][2][0];
						compare2[1][1] = saved[j][2][1];
					}
                    else
                    {
                        compare2[0][0] = saved[j][2][0];
                    	compare2[0][1] = saved[j][2][1];

                        compare2[1][0] = saved[j][3][0];
                        compare2[1][1] = saved[j][3][1];
					}
				}
					legcount = 0;
			}
			else //if less than 4 legs
			{
				for(i = 0; i < 4; i++)	//Compare to previous frame
				{
					for(x = 0; x < 4; x++)
					{
						if(compare2[x][0] != 0 && legstemp2[x][0] == 0)
						{
							if(saved[j][i][0] < compare2[x][0]+vartemp && saved[j][i][0] > compare2[x][0]-vartemp)
							{
								legstemp2[x][0] = saved[j][i][0];
								legstemp2[x][1] = saved[j][i][1];
								saved[j][i][0] = 0;
								saved[j][i][1] = 0;
								legcount--;
							}
						}
					}
				}
				
				for(i = 0; i < 4; i++)	//Compare to last time seen
				{
					for(x = 0; x < 4; x++)
					{
						if(last2[x][0] != 0 && legstemp2[x][0] == 0)
						{
							if(saved[j][i][0] < last2[x][0]+vartemp && saved[j][i][0] > last2[x][0]-vartemp)
							{
								if(lastCount2[x]-j < 9)
								{
									legstemp2[x][0] = saved[j][i][0];
									legstemp2[x][1] = saved[j][i][1];
									saved[j][i][0] = 0;
									saved[j][i][1] = 0;
									legcount--;
								}
							}
						}
					}
				}
				
				for(i = 0; i<4; i++)
				{
					compare2[i][0] = legstemp2[i][0];
					compare2[i][1] = legstemp2[i][1];
					legstemp2[i][0] = 0;
					legstemp2[i][1] = 0;
				}
				for(i=0;i<4;i++)	//Check if current position is the next step of last time seen
	        	{
	        		if(saved[j][i][0] != 0)
	        		{
	        			for(x=0;x<4;x++)
	        			{
	        				if(direction == 1)	//left to right(reverse)
	        				{
	        					if(saved[j][i][0] < last2[x][0] && compare2[x][0] == 0 && last2[x][0] != 0)
	        					{
									if(lastCount2[x]-j < 12)
									{
	        							compare2[x][0] = saved[j][i][0];
	        							compare2[x][1] = saved[j][i][1];
										saved[j][i][0] = 0;
										saved[j][i][1] = 0;
										legcount--;
									}
								}
							}
							else if(direction == 2)	//right to left(reverse)
							{
	    	    				if(saved[j][i][0] > last2[x][0] && compare2[x][0] == 0 && last2[x][0] != 0)
	        					{
									if(lastCount2[x]-j < 12)
									{
	        							compare2[x][0] = saved[j][i][0];
	        							compare2[x][1] = saved[j][i][1];
										saved[j][i][0] = 0;
										saved[j][i][1] = 0;
										legcount--;
									}
								}
							}
						}
					}
				}
		
				if(midpoint == 0 && legcount == 2)	//if the two front legs are in frame (two unknown legs)
				{
					midpoint = 1;
					int rearlegs2[2][2]; //front legs
					x = 0;
					for(i = 0; i<4; i++)
					{
						if(saved[j][i][0] != 0 && x != 2)
						{
							rearlegs2[x][0] = saved[j][i][0];
							rearlegs2[x][1] = saved[j][i][1];
							saved[j][i][0] = 0;
							saved[j][i][1] = 0;
							x++;
							legcount--;
						}
					}
					if(direction == 1)
					{
                        if (rearlegs2[0][1] < rearlegs2[1][1])
   		                {
           		            compare2[0][0] = rearlegs2[1][0];
               			    compare2[0][1] = rearlegs2[1][1];
	
                       		compare2[1][0] = rearlegs2[0][0];
                            compare2[1][1] = rearlegs2[0][1];
   		                }
           		        else
   		                {
           		            compare2[0][0] = rearlegs2[0][0];
                   		    compare2[0][1] = rearlegs2[0][1];

                            compare2[1][0] = rearlegs2[1][0];
   		                    compare2[1][1] = rearlegs2[1][1];
						}
					}
					else if(direction == 2)
					{
                        if (rearlegs2[0][1] > rearlegs2[1][1])
   		                {
           		            compare2[0][0] = rearlegs2[1][0];
               			    compare2[0][1] = rearlegs2[1][1];

                       		compare2[1][0] = rearlegs2[0][0];
                            compare2[1][1] = rearlegs2[0][1];
   		                }
           		        else
   		                {
           		            compare2[0][0] = rearlegs2[0][0];
                   		    compare2[0][1] = rearlegs2[0][1];

                            compare2[1][0] = rearlegs2[1][0];
   		                    compare2[1][1] = rearlegs2[1][1];
						}
					}
					rearlegs2[0][0] = 0;
					rearlegs2[0][1] = 0;
					rearlegs2[1][0] = 0;
					rearlegs2[1][1] = 0;
					}
				} 
			}
			
			for(i=0;i<4;i++)
        	{
        		if(compare2[i][0] != 0)
        		{
        			last2[i][0] = compare2[i][0];
        			last2[i][1] = compare2[i][1];
        			lastCount2[i] = frame;
				}
			}
			output2fliped[j][0][0] = compare2[0][0];
			output2fliped[j][0][1] = compare2[0][1];
			output2fliped[j][1][0] = compare2[1][0];
			output2fliped[j][1][1] = compare2[1][1];
			output2fliped[j][2][0] = compare2[2][0];
			output2fliped[j][2][1] = compare2[2][1];
			output2fliped[j][3][0] = compare2[3][0];
			output2fliped[j][3][1] = compare2[3][1];
			
			if(midpoint == 1)
			{
				finaloutput[j][0][0] = compare2[0][0];
    	    	finaloutput[j][0][1] = compare2[0][1];
        		finaloutput[j][1][0] = compare2[1][0];
        		finaloutput[j][1][1] = compare2[1][1];
				finaloutput[j][2][0] = compare2[2][0];
        		finaloutput[j][2][1] = compare2[2][1];
        		finaloutput[j][3][0] = compare2[3][0];
        		finaloutput[j][3][1] = compare2[3][1];
			}
			
			printf("\n\n= last =\n\n %d, %d\n %d, %d\n %d, %d\n %d, %d\n\n", last2[0][0], last2[0][1], last2[1][0], last2[1][1], last2[2][0], last2[2][1], last2[3][0], last2[3][1]);
			
			printf("\n\n= Output compare 2 =\n\nFR = %d, %d\nFL = %d, %d\nRR = %d, %d\nRL = %d, %d\n\n", output2fliped[j][0][0], output2fliped[j][0][1], output2fliped[j][1][0], output2fliped[j][1][1], output2fliped[j][2][0], output2fliped[j][2][1], output2fliped[j][3][0], output2fliped[j][3][1]);
			//printf("\= Output compare 1 =\nFR = %d, %d\nFL = %d, %d\nRR = %d, %d\nRL = %d, %d\n\n", compare2[0][0], compare2[0][1], compare2[1][0], compare2[1][1], compare2[2][0], compare2[2][1], compare2[3][0], compare2[3][1]);		
		    //FINISHHHHHHHHHHHHHHHHHHHHHHHHHHHHhh
	}
	for (j=0; j < totalframes; j++)
	{
		fprintf(output2Pointer,"%d,%d\n%d,%d\n%d,%d\n%d,%d\n", output2fliped[j][0][0], output2fliped[j][0][1], output2fliped[j][1][0], output2fliped[j][1][1], output2fliped[j][2][0], output2fliped[j][2][1], output2fliped[j][3][0], output2fliped[j][3][1]);
		//printf("\= Output compare 2 = Frame = %d\nFR = %d, %d\nFL = %d, %d\nRR = %d, %d\nRL = %d, %d\n\n", j, output2fliped[j][0][0], output2fliped[j][0][1], output2fliped[j][1][0], output2fliped[j][1][1], output2fliped[j][2][0], output2fliped[j][2][1], output2fliped[j][3][0], output2fliped[j][3][1]);
		//printf("\n= Frame = %d =\nFR = %d, %d\nFL = %d, %d\nRR = %d, %d\nRL = %d, %d\n\n", (j+1), finaloutput[j][0][0], finaloutput[j][0][1], finaloutput[j][1][0], finaloutput[j][1][1], finaloutput[j][2][0], finaloutput[j][2][1], finaloutput[j][3][0], finaloutput[j][3][1]);
		fprintf(finaloutput1Pointer, "%d,%d\n%d,%d\n%d,%d\n%d,%d\n", finaloutput[j][0][0], finaloutput[j][0][1], finaloutput[j][1][0], finaloutput[j][1][1], finaloutput[j][2][0], finaloutput[j][2][1], finaloutput[j][3][0], finaloutput[j][3][1]);
	}
	fclose(output1Pointer);
	fclose(output2Pointer);
	fclose(finaloutput1Pointer);
	//==========================Variables==================================
	int save[4][3];
	int gone[4]={0,0,0,0};	//gone check
	int confirm[4]={0,0,0,0};
	int finall[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	int legnum[4]={0,0,0,0};
	x=0;			//leg count
	u=0;			//leg coordinates temp
	i=1;			//frame counter
	
	//=====================================================================
	
	finaloutput1Pointer = fopen(finaloutput1, "r");               //finaloutput1 pointer
	while (fgets(temp, 11, finaloutput1Pointer))
 	{
		// Token will point to x.
        token = strtok(temp, search);
		u = atoi(token);			//leg x coordinates
		if(save[x][0] == 0)			//if empty, set start time and coordinate
 		{
 			if(u!=0)
 			{
	 			save[x][0] = i;
 				save[x][1] = u;
				confirm[x]++;
			}
		}
		else
		{
			if(u==0)
			{
				gone[x]++;
				if(gone[x]==3)			//check if foot is gone for 3 frames
				{
					gone[x]=0;
					confirm[x]=0;
					save[x][0] = 0;
					save[x][1] = 0;
					save[x][2] = 0;
				}
			}
			else
			{
				confirm[x]=confirm[x]+1;
				if(confirm[x]==3)		//check if foot steped for at least 3 frames
				{
					finall[x][legnum[x]]=u;
					legnum[x]++;
				}
			}
		}
		
 		if(x<3)		//set leg and frame
 		{
 			x++;
		}
		else
		{
			x=0;
			i++;
		}
 		
 	}
 	printf("FR = %d, %d, %d, %d\n",finall[0][0], finall[0][1], finall[0][2], finall[0][3]);
 	printf("FL = %d, %d, %d, %d\n",finall[1][0], finall[1][1], finall[1][2], finall[1][3]);
 	printf("RR = %d, %d, %d, %d\n",finall[2][0], finall[2][1], finall[2][2], finall[2][3]);
 	printf("RL = %d, %d, %d, %d\n",finall[3][0], finall[3][1], finall[3][2], finall[3][3]);
 	int lengths[4]={0,0,0,0};
	fprintf(stepCountingPointer,"\n");
 	for(x=0;x<4;x++)
 	{
 		u=0;
		z=0;
 		for(i=0;i<4;i++)
 		{
 			if(finall[x][i] != 0)
 			{
 				u=i;
			}
		}
		if(u>0)
		{
			z=(finall[x][0]-finall[x][u])/u;
		}
			fprintf(stepCountingPointer,"%d\n",u+1);
		lengths[x]=abs(z);
	}
	fprintf(ResultPointer,"%d, %d, %d, %d\n",lengths[0],lengths[1],lengths[2],lengths[3]);
	fclose(finaloutput1Pointer);
	fclose(ResultPointer);
	return 0;
}
