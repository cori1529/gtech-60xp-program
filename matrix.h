/*
	matrix.h
*/

#ifndef ___MATRIX_H___
#define ___MATRIX_H___

//  ���� matrix ��ǥ 

#define PERSON_OCCUPIED  4    // ���� ���� �ִ� ����� ���� �����ϰ� �ִ� ����(��)�� ���� 

extern char a1b1_input;	 // 1��1�� 
extern char a1b2_input;
extern char a1b3_input;
extern char a1b4_input;
extern char a1b5_input;	// 1�� 5�� 

extern char a2b1_input;	 // 2��1�� 
extern char a2b2_input;
extern char a2b3_input;
extern char a2b4_input;
extern char a2b5_input;	// 2�� 5�� 

extern char a3b1_input;	 // 3��1�� 
extern char a3b2_input;
extern char a3b3_input;
extern char a3b4_input;
extern char a3b5_input;	// 3�� 5�� 

extern char a4b1_input;	 // 4��1�� 
extern char a4b2_input;
extern char a4b3_input;
extern char a4b4_input;
extern char a4b5_input;	// 4�� 5�� 

extern char a5b1_input;	 // 5��1�� 
extern char a5b2_input;
extern char a5b3_input;
extern char a5b4_input;
extern char a5b5_input;	// 5�� 5�� 


extern char first_line;	     // ù��°(�Ӹ���)��     ���� ���� 1 : ����, 0 : ������ 
extern char second_line;	 // �ι�°(�Ӹ���)��     ���� ���� 1 : ����, 0 : ������ 
extern char third_line;	     // ����°(�Ӹ���)��     ���� ���� 1 : ����, 0 : ������ 
extern char fourth_line;	 // ����°(�Ӹ���)��     ���� ���� 1 : ����, 0 : ������ 
extern char fifth_line;	     // �ټ���°(�Ӹ���)��   ���� ���� 1 : ����, 0 : ������ 
extern int occupied_line; //������ ������ �� 




extern char first_column;	     // ù��°��(�ǿ�����)     ���� ���� 1 : ����, 0 : ������ 
extern char second_column;	    //  �ι�°��                ���� ���� 1 : ����, 0 : ������ 
extern char third_column;	    //  ����°(��� )       ���� ���� 1 : ����, 0 : ������ 
extern char fourth_column;	    //  ����°(�Ӹ���)��        ���� ���� 1 : ����, 0 : ������ 
extern char fifth_column;	    //  5��°��(�ǿ���)        ���� ���� 1 : ����, 0 : ������ 

extern int occupied_column;       //  ������ ���Ǽ�  








void matrix_input_test(void);
void matrix_input_display(void);
void matrix_input(void);

void matrix_input_usart(void);

  
//  ���� matrix ��ǥ 

#endif	//___MATRIX_H___

/* end of file */