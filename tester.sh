#!/bin/tcsh

if ( ! -f ./sudoku-console) then
	echo "sudoku-console file not found"
endif

foreach f_command (" `ls test_inputs/*` ")
	foreach f_board (" `ls test_file_inputs/*` ")
		set fcn=${f_command:t}
		set fbn=${f_board:t}
		set output_path="test_outputs/${fcn}_${fbn}/myTest"
		set output_path2="test_outputs/${fcn}_${fbn}"
		echo $output_path

		set v="`cat $f_command`"
		set i=1
		set firstline="$v[$i]test_file_inputs/${f_board:t}\n"
		@ i = $i + 1
		set rest=""
		while ( $i <= $#v )
			set rest="$rest$v[$i]\n"
			@ i = $i + 1
		end

		echo "$firstline$rest" > tester_input.txt
		./sudoku-console < tester_input.txt > $output_path

		if (  -f ./$output_path2/option1) then
		  echo ./$output_path2/option1
	    if ("`diff ./$output_path2/option1 ./$output_path2/myTest`" != "") then
        echo "\nfail test "
        echo "-------- input --------"
        cat tester_input.txt
        echo "---- expected output ----"
        cat ./$output_path2/option1
        echo "---- actual output ----"
        cat ./$output_path2/myTest
        echo ""
      else
        echo "test  success!!!!!"
        echo "---------------"
      endif
    endif
		if (  -f ./$output_path2/option3) then
		  echo ./$output_path2/option3
	    if ("`diff ./$output_path2/option3 ./$output_path2/myTest`" != "") then
        echo "\nfail test "
        echo "-------- input --------"
        cat tester_input.txt
        echo "---- expected output ----"
        cat ./$output_path2/option3
        echo "---- actual output ----"
        cat ./$output_path2/myTest
        echo ""
      else
        echo "test  success!!!!!"
        echo "---------------"
      endif
    endif
		if (  -f ./$output_path2/option4) then
		  echo ./$output_path2/option4
	    if ("`diff ./$output_path2/option4 ./$output_path2/myTest`" != "") then
        echo "\nfail test "
        echo "-------- input --------"
        cat tester_input.txt
        echo "---- expected output ----"
        cat ./$output_path2/option4
        echo "---- actual output ----"
        cat ./$output_path2/myTest
        echo ""
      else
        echo "test  success"
        echo "---------------"
      endif
    endif




    rm tester_input.txt

	end
end

exit
