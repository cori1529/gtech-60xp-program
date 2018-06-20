void Send_to_Pc(void)  // 삼성병원 , blue tooth 프린터 관련 2014년 5월 14일 
  {
       
    long  weight,height,obesity,bmi,standard_weight;
	unsigned char  Judge_obesidy_flag1;
	char i;

      
	  weight=gxlWeightStable;
	  height=gxlHeightStable;
	  obesity=gxlObesity;
	  bmi=gxlBMI;

 
      if (gxiGender == FEMALE) 
	{
		gxlStandardWeight = ((gxlHeightStable * gxlHeightStable) * 21 /10000);	//999.99 kg
	}
	else
	{
		gxlStandardWeight = ((gxlHeightStable * gxlHeightStable) * 22 /10000);	//999.99 kg

    }
     standard_weight=gxlStandardWeight;


    if(gxlBMI<185)
		    Judge_obesidy_flag1=1;
		  else if(gxlBMI>=185 && gxlBMI<=249)
		    Judge_obesidy_flag1=2;
       
	      else if(gxlBMI>=250 && gxlBMI<=269)
		    Judge_obesidy_flag1=3;

          else if(gxlBMI >=270 && gxlBMI<=299)
		    Judge_obesidy_flag1=4;

          else if(gxlBMI>= 300 && gxlBMI<=349)
		    Judge_obesidy_flag1=5;

          else if(gxlBMI>=350 && gxlBMI<=399)
		    Judge_obesidy_flag1=6;
       
            else
		    Judge_obesidy_flag1=7;
   



            // 글자크기 2배 

		if(!Power_On)
        {
			Power_On=1; // 처음한번마 
//<<add 1.10 2014.9.17  인쇄 전 빈줄 삭제
//          putchar1(0x1d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
			putchar1(0x21); delay_ms(SENDTOPCDELAY); 
			putchar1(0x11); delay_ms(SENDTOPCDELAY);   // 2배  



		    sprintf (&SendBuffer1[0], " OK OK OK !!!!");
			
			for(i=0;i<0;i++)
			{
				putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 

			putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);     

   	        sprintf (&SendBuffer1[0], "     신장:%03ld.%ld cm",height/10, height%10 );
			
			for(i=0;i<0;i++)
			{
				putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 

            putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);     
		

            if(weight>9999) //  100 kg 이상일때 
			{
            	weight=weight/10; 
               	sprintf (&SendBuffer1[0], "     체중:%03ld.%ld kg",weight/10, weight%10 );
            	for(i=0;i<0;i++)
				{
					putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
				} 
			}
        	else
			{ 
            	sprintf (&SendBuffer1[0], "     체중:%02ld.%02ld kg",weight/100, weight%100 );
               	for(i=0;i<0;i++)
			 	{
			  		putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			 	} 
			}

      		for(i=0;i<4;i++)
			{
	   			putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
				putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		 	}	
	
			putchar1(0x1b); delay_ms(SENDTOPCDELAY); // partial cutting 
			putchar1(0x6d); delay_ms(SENDTOPCDELAY);   

/* 윗줄 제거
	 		for(i=0;i<1;i++)
	     	{
	   			putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
				putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		 	}	
*/
      	}// of power on if(!Power_On)

	    
		putchar1(0x1d); delay_ms(SENDTOPCDELAY); 
		putchar1(0x21); delay_ms(SENDTOPCDELAY); 
		putchar1(0x11); delay_ms(SENDTOPCDELAY);   // 2배  
   
   	
		sprintf (&SendBuffer1[0], "     신장:%03ld.%ld cm",height/10, height%10 );
			
		for(i=0;i<18;i++)
		{
			putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
		} 

//		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
//		putchar1(0x0a); delay_ms(SENDTOPCDELAY);     


		if(weight>9999) //  100 kg 이상일때 
		{
        	weight=weight/10; 
            sprintf (&SendBuffer1[0], "     체중:%03ld.%ld kg",weight/10, weight%10 );
            for(i=0;i<18;i++)
			{
			  	putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 
		}	//if(weight>9999) //  100 kg 이상일때 
        else
		{ 
        	sprintf (&SendBuffer1[0], "     체중:%02ld.%02ld kg",weight/100, weight%100 );
            for(i=0;i<18;i++)
			{
				putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 
		} //if(weight>9999) //  100 kg 이상일때 


		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
		putchar1(0x0a); delay_ms(SENDTOPCDELAY);     

 // 신장 인쇄

	 	sprintf (&SendBuffer1[0], "     신장:%03ld.%ld cm",height/10, height%10 );
			
		for(i=0;i<18;i++)
		{
			putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
		} 
		
		for(i=0;i<4;i++)
	    {
	   		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		}	

//<<add 1.10 2014.9.17  인쇄 후 빈줄 추가 아랫줄 추가
      	for(i=0;i<2;i++)
	    {
	   		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		}	
//>>add 1.10 2014.9.17	

		putchar1(0x1b); delay_ms(SENDTOPCDELAY); // partial cutting 
		putchar1(0x6d); delay_ms(SENDTOPCDELAY);   

     
/* 윗줄 제거
	 	for(i=0;i<1;i++)
	    {
	   		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // 줄바뀜 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		}	
*/
}
