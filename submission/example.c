/**hi this is a commient**/
read(argc);
argc=FUN(argc){
	If( argc <= 1 ) {
		x = 1;
	} else {
		x = FUN(argc-1) * argc;
	}
	write(x);
	return x;
}
write(argc);
