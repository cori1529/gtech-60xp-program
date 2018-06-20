/*
	matrix.h
*/

#ifndef ___MATRIX_H___
#define ___MATRIX_H___

//  낙상 matrix 좌표 

#define PERSON_OCCUPIED  4    // 현재 누워 있는 사람이 평상시 차지하고 있는 라인(행)의 갯수 

extern char a1b1_input;	 // 1행1열 
extern char a1b2_input;
extern char a1b3_input;
extern char a1b4_input;
extern char a1b5_input;	// 1행 5열 

extern char a2b1_input;	 // 2행1열 
extern char a2b2_input;
extern char a2b3_input;
extern char a2b4_input;
extern char a2b5_input;	// 2행 5열 

extern char a3b1_input;	 // 3행1열 
extern char a3b2_input;
extern char a3b3_input;
extern char a3b4_input;
extern char a3b5_input;	// 3행 5열 

extern char a4b1_input;	 // 4행1열 
extern char a4b2_input;
extern char a4b3_input;
extern char a4b4_input;
extern char a4b5_input;	// 4행 5열 

extern char a5b1_input;	 // 5행1열 
extern char a5b2_input;
extern char a5b3_input;
extern char a5b4_input;
extern char a5b5_input;	// 5행 5열 


extern char first_line;	     // 첫번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
extern char second_line;	 // 두번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
extern char third_line;	     // 세번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
extern char fourth_line;	 // 내번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
extern char fifth_line;	     // 다섯번째(머리쪽)이   점유 여부 1 : 점유, 0 : 비점유 
extern int occupied_line; //점유된 라인의 수 




extern char first_column;	     // 첫번째열(맨오른쪽)     점유 여부 1 : 점유, 0 : 비점유 
extern char second_column;	    //  두번째열                점유 여부 1 : 점유, 0 : 비점유 
extern char third_column;	    //  세번째(가운데 )       점유 여부 1 : 점유, 0 : 비점유 
extern char fourth_column;	    //  내번째(머리쪽)이        점유 여부 1 : 점유, 0 : 비점유 
extern char fifth_column;	    //  5번째열(맨왼쪽)        점유 여부 1 : 점유, 0 : 비점유 

extern int occupied_column;       //  점유된 열의수  








void matrix_input_test(void);
void matrix_input_display(void);
void matrix_input(void);

void matrix_input_usart(void);

  
//  낙상 matrix 좌표 

#endif	//___MATRIX_H___

/* end of file */