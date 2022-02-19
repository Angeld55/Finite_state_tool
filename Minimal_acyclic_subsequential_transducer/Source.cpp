#include <iostream>
#include <string>
#include "FilesUtills.h"
#include <fstream>
#include <sstream> 
#include <ctime>


int main()
{

	//{ //exam data - separator with a '\t'

	//	SubsequentialTransducer st = buildByFile("exam/dict.txt");
	//	st.smallPrint();

	//	removeFromFile("exam/delete.txt", st);
	//	st.smallPrint();

	//	insertFromFile("exam/add.txt", st);
	//	st.smallPrint();	
	//	cout << "-----------------------------------------------------" << endl;
	//}


	//for these tests we need to change the separator with a ' ' 
	//tests
	{
		SubsequentialTransducer st = buildByFile("tests2/t1_start.txt");


		insertFromFile("tests2/t1_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t1_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests2/t2_start.txt");



		insertFromFile("tests2/t2_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t2_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests2/t3_start.txt");



		insertFromFile("tests2/t3_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t3_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests2/t4_start.txt");


		insertFromFile("tests2/t4_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t4_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests2/t5_start.txt");


		insertFromFile("tests2/t5_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t5_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}
	{
		SubsequentialTransducer st = buildByFile("tests2/t6_start.txt");



		insertFromFile("tests2/t6_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t6_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests2/t7_start.txt");


		insertFromFile("tests2/t7_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t7_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests2/t8_start.txt");


		insertFromFile("tests2/t8_add.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests2/t8_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}
	{
		SubsequentialTransducer st = buildByFile("tests/t1_start.txt");

		removeFromFile("tests/t1_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t1_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests/t2_start.txt");

		removeFromFile("tests/t2_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t2_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}


	{
		SubsequentialTransducer st = buildByFile("tests/t3_start.txt");

		removeFromFile("tests/t3_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t3_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests/t4_start.txt");

		removeFromFile("tests/t4_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t4_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}
	{
		SubsequentialTransducer st = buildByFile("tests/t5_start.txt");

		removeFromFile("tests/t5_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t5_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}
	{
		SubsequentialTransducer st = buildByFile("tests/t6_start.txt");

		removeFromFile("tests/t6_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t6_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}
	{
		SubsequentialTransducer st = buildByFile("tests/t7_start.txt");

		removeFromFile("tests/t7_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t7_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}
		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests/t8_start.txt");

		removeFromFile("tests/t8_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t8_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}



	{
		SubsequentialTransducer st = buildByFile("tests/t9_start.txt");

		removeFromFile("tests/t9_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t9_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}


	{
		SubsequentialTransducer st = buildByFile("tests/t10_start.txt");

		removeFromFile("tests/t10_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t10_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests/t11_start.txt");

		removeFromFile("tests/t11_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t11_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}

	{
		SubsequentialTransducer st = buildByFile("tests/t12_start.txt");

		removeFromFile("tests/t12_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t12_result.txt");

		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}
	{
		SubsequentialTransducer st = buildByFile("tests/t13_start.txt");
		removeFromFile("tests/t13_remove.txt", st);

		SubsequentialTransducer st2 = buildByFile("tests/t13_result.txt");
		
		if (areEquivalent(st, st2))
			cout << "Test passed!" << endl;
		else
		{
			cout << "Test failed!" << endl;
			st.PRINTTRANSDUSER();
			st2.PRINTTRANSDUSER();
		}

		cout << "-----------------------------------------------------" << endl;
	}


	while (1)
	{
		;
	}

}

