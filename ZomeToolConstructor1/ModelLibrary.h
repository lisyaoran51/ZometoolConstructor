
//�ߤl�զ�
/*
�ߤl�զ�
	int ball [32];
	ball[0] = 0;
	ball[0] = constructor.Construct( ball[0] , 42 , 0 , 0 );
	ball[1] = constructor.Construct( ball[0] , 28 , 0 , 1 );
	ball[2] = constructor.Construct( ball[0] , 32 , 0 , 1 );
	ball[3] = constructor.Construct( ball[0] , 40 , 1 , 1 );
	ball[4] = constructor.Construct( ball[0] , 45 , 1 , 1 );

	ball[1] = constructor.Construct( ball[1] , 21 , 0 , 0 );
	ball[5] = constructor.Construct( ball[1] , 45 , 1 , 1 );
	ball[6] = constructor.Construct( ball[1] , 61 , 0 , 0 );//���ӬO6�A��ӵo�{6�M3���ƤF
	ball[7] = constructor.Construct( ball[1] , 10 , 1 , 1 );

	ball[2] = constructor.Construct( ball[2] , 56 , 0 , 0 );
	ball[8] = constructor.Construct( ball[2] , 9 , 0 , 1 );
	ball[9] = constructor.Construct( ball[2] , 40 , 1 , 1 );
	ball[10] = constructor.Construct( ball[2] , 44 , 2 , 1 );

	ball[3] = constructor.Construct( ball[3] , 32 , 0 , 0 );//��9
	ball[3] = constructor.Construct( ball[3] , 44 , 0 , 0 );//��7

	ball[4] = constructor.Construct( ball[4] , 50 , 0 , 0 );
	ball[4] = constructor.Construct( ball[4] , 48 , 0 , 0 );//��8
	ball[4] = constructor.Construct( ball[4] , 28 , 0 , 0 );//��5
	ball[11] = constructor.Construct( ball[4] , 44 , 0 , 1 );
	ball[12] = constructor.Construct( ball[4] , 11 , 1 , 1 );

	ball[5] = constructor.Construct( ball[5] , 34 , 0 , 0 );
	ball[5] = constructor.Construct( ball[5] , 1 , 0 , 0 );//��12
	ball[13] = constructor.Construct( ball[5] , 40 , 0 , 1 );

	ball[7] = constructor.Construct( ball[7] , 35 , 0 , 0 );
	ball[7] = constructor.Construct( ball[7] , 55 , 0 , 0 );//��13
	ball[14] = constructor.Construct( ball[7] , 32 , 0 , 1 );
	ball[15] = constructor.Construct( ball[7] , 1 , 0 , 1 );

	ball[8] = constructor.Construct( ball[8] , 37 , 0 , 0 );
	ball[8] = constructor.Construct( ball[8] , 20 , 0 , 0 );//��11
	ball[16] = constructor.Construct( ball[8] , 44 , 2 , 1 );//16�M18���ƤF

	ball[9] = constructor.Construct( ball[9] , 16 , 0 , 0 );
	ball[9] = constructor.Construct( ball[9] , 44 , 0 , 0 );//��14
	ball[9] = constructor.Construct( ball[9] , 45 , 1 , 0 );//��10
	ball[17] = constructor.Construct( ball[9] , 1 , 1 , 1 );

	ball[10] = constructor.Construct( ball[10] , 19 , 0 , 0 );
	ball[18] = constructor.Construct( ball[10] , 9 , 0 , 0 );//16�M18���ƤF
	ball[19] = constructor.Construct( ball[10] , 1 , 1 , 1 );
	ball[20] = constructor.Construct( ball[10] , 44 , 1 , 1 );

	ball[11] = constructor.Construct( ball[11] , 39 , 0 , 0 );
	ball[11] = constructor.Construct( ball[11] , 59 , 0 , 0 );//��12
	ball[21] = constructor.Construct( ball[11] , 1 , 1 , 1 );

	ball[12] = constructor.Construct( ball[12] , 23 , 0 , 0 );
	ball[22] = constructor.Construct( ball[12] , 11 , 0 , 1 );
	ball[23] = constructor.Construct( ball[12] , 1 , 1 , 1 );

	ball[13] = constructor.Construct( ball[13] , 46 , 0 , 0 );
	ball[24] = constructor.Construct( ball[13] , 44 , 1 , 1 );

	ball[14] = constructor.Construct( ball[14] , 36 , 0 , 0 );
	ball[14] = constructor.Construct( ball[14] , 45 , 0 , 0 );//��17
	ball[25] = constructor.Construct( ball[14] , 1 , 0 , 1 );

	ball[15] = constructor.Construct( ball[15] , 35 , 0 , 0 );
	ball[15] = constructor.Construct( ball[15] , 32 , 0 , 0 );//��25
	ball[15] = constructor.Construct( ball[15] , 33 , 0 , 0 );//��24

	ball[16] = constructor.Construct( ball[16] , 37 , 0 , 0 );
	ball[16] = constructor.Construct( ball[16] , 34 , 0 , 0 );//��21
	ball[16] = constructor.Construct( ball[16] , 11 , 0 , 0 );//��19
	ball[26] = constructor.Construct( ball[16] , 33 , 1 , 1 );

	ball[17] = constructor.Construct( ball[17] , 3 , 0 , 0 );
	ball[17] = constructor.Construct( ball[17] , 1 , 0 , 0 );//��20

	ball[19] = constructor.Construct( ball[19] , 3 , 0 , 0 );
	ball[27] = constructor.Construct( ball[19] , 1 , 0 , 1 );
	ball[28] = constructor.Construct( ball[19] , 44 , 1 , 1 );

	ball[20] = constructor.Construct( ball[20] , 36 , 0 , 0 );
	ball[20] = constructor.Construct( ball[20] , 43 , 0 , 0 );//��25
	ball[29] = constructor.Construct( ball[20] , 20 , 0 , 1 );
	ball[20] = constructor.Construct( ball[20] , 1 , 1 , 0 );//��28

	ball[21] = constructor.Construct( ball[21] , 4 , 0 , 0 );
	ball[21] = constructor.Construct( ball[21] , 59 , 0 , 0 );//��23
	ball[21] = constructor.Construct( ball[21] , 44 , 1 , 0 );

	ball[22] = constructor.Construct( ball[22] , 2 , 0 , 0 );
	ball[22] = constructor.Construct( ball[22] , 9 , 0 , 0 );//��23
	ball[30] = constructor.Construct( ball[22] , 44 , 0 , 1 );
	ball[22] = constructor.Construct( ball[22] , 52 , 0 , 0 );//��24

	ball[23] = constructor.Construct( ball[23] , 23 , 0 , 0 );
	ball[23] = constructor.Construct( ball[23] , 53 , 0 , 0 );//��30
	ball[23] = constructor.Construct( ball[23] , 32 , 0 , 0 );//��26
	ball[31] = constructor.Construct( ball[23] , 11 , 0 , 1 );

	ball[24] = constructor.Construct( ball[24] , 35 , 0 , 0 );
	ball[24] = constructor.Construct( ball[24] , 3 , 0 , 0 );//��25
	ball[24] = constructor.Construct( ball[24] , 8 , 0 , 0 );//��30

	ball[25] = constructor.Construct( ball[25] , 5 , 0 , 0 );
	ball[25] = constructor.Construct( ball[25] , 33 , 1 , 0 );//��29

	ball[26] = constructor.Construct( ball[26] , 25 , 0 , 0 );
	ball[26] = constructor.Construct( ball[26] , 35 , 0 , 0 );//��31
	ball[26] = constructor.Construct( ball[26] , 61 , 0 , 0 );//��27

	ball[27] = constructor.Construct( ball[27] , 37 , 0 , 0 );

	ball[28] = constructor.Construct( ball[28] , 3 , 0 , 0 );

	ball[29] = constructor.Construct( ball[29] , 5 , 0 , 0 );
	ball[29] = constructor.Construct( ball[29] , 54 , 0 , 0 );//��30

	ball[30] = constructor.Construct( ball[30] , 2 , 0 , 0 );
	ball[30] = constructor.Construct( ball[30] , 45 , 0 , 0 );//��31

	ball[31] = constructor.Construct( ball[31] , 15 , 0 , 0 );

*/

