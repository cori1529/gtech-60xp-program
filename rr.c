void Send_to_Pc(void)  // �Ｚ���� , blue tooth ������ ���� 2014�� 5�� 14�� 
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
   



            // ����ũ�� 2�� 

		if(!Power_On)
        {
			Power_On=1; // ó���ѹ��� 
//<<add 1.10 2014.9.17  �μ� �� ���� ����
//          putchar1(0x1d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
			putchar1(0x21); delay_ms(SENDTOPCDELAY); 
			putchar1(0x11); delay_ms(SENDTOPCDELAY);   // 2��  



		    sprintf (&SendBuffer1[0], " OK OK OK !!!!");
			
			for(i=0;i<0;i++)
			{
				putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 

			putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);     

   	        sprintf (&SendBuffer1[0], "     ����:%03ld.%ld cm",height/10, height%10 );
			
			for(i=0;i<0;i++)
			{
				putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 

            putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);     
		

            if(weight>9999) //  100 kg �̻��϶� 
			{
            	weight=weight/10; 
               	sprintf (&SendBuffer1[0], "     ü��:%03ld.%ld kg",weight/10, weight%10 );
            	for(i=0;i<0;i++)
				{
					putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
				} 
			}
        	else
			{ 
            	sprintf (&SendBuffer1[0], "     ü��:%02ld.%02ld kg",weight/100, weight%100 );
               	for(i=0;i<0;i++)
			 	{
			  		putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			 	} 
			}

      		for(i=0;i<4;i++)
			{
	   			putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
				putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		 	}	
	
			putchar1(0x1b); delay_ms(SENDTOPCDELAY); // partial cutting 
			putchar1(0x6d); delay_ms(SENDTOPCDELAY);   

/* ���� ����
	 		for(i=0;i<1;i++)
	     	{
	   			putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
				putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		 	}	
*/
      	}// of power on if(!Power_On)

	    
		putchar1(0x1d); delay_ms(SENDTOPCDELAY); 
		putchar1(0x21); delay_ms(SENDTOPCDELAY); 
		putchar1(0x11); delay_ms(SENDTOPCDELAY);   // 2��  
   
   	
		sprintf (&SendBuffer1[0], "     ����:%03ld.%ld cm",height/10, height%10 );
			
		for(i=0;i<18;i++)
		{
			putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
		} 

//		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
//		putchar1(0x0a); delay_ms(SENDTOPCDELAY);     


		if(weight>9999) //  100 kg �̻��϶� 
		{
        	weight=weight/10; 
            sprintf (&SendBuffer1[0], "     ü��:%03ld.%ld kg",weight/10, weight%10 );
            for(i=0;i<18;i++)
			{
			  	putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 
		}	//if(weight>9999) //  100 kg �̻��϶� 
        else
		{ 
        	sprintf (&SendBuffer1[0], "     ü��:%02ld.%02ld kg",weight/100, weight%100 );
            for(i=0;i<18;i++)
			{
				putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
			} 
		} //if(weight>9999) //  100 kg �̻��϶� 


		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
		putchar1(0x0a); delay_ms(SENDTOPCDELAY);     

 // ���� �μ�

	 	sprintf (&SendBuffer1[0], "     ����:%03ld.%ld cm",height/10, height%10 );
			
		for(i=0;i<18;i++)
		{
			putchar1(SendBuffer1[i]); delay_ms(SENDTOPCDELAY);
		} 
		
		for(i=0;i<4;i++)
	    {
	   		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		}	

//<<add 1.10 2014.9.17  �μ� �� ���� �߰� �Ʒ��� �߰�
      	for(i=0;i<2;i++)
	    {
	   		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		}	
//>>add 1.10 2014.9.17	

		putchar1(0x1b); delay_ms(SENDTOPCDELAY); // partial cutting 
		putchar1(0x6d); delay_ms(SENDTOPCDELAY);   

     
/* ���� ����
	 	for(i=0;i<1;i++)
	    {
	   		putchar1(0x0d); delay_ms(SENDTOPCDELAY); // �ٹٲ� 
			putchar1(0x0a); delay_ms(SENDTOPCDELAY);   
		}	
*/
}
