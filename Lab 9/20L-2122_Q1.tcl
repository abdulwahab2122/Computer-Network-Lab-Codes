puts "Enter number greater than 4"

set x [gets stdin]

if {$x%2==0} {

	for {set y 4} {$y <= $x} {incr y 2} {
		puts "$y "
	}
} else {
	for {set y 3} {$y <= $x} {incr y 2} {
		puts "$y "		
	}
}




