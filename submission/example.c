read(argc);
argc=FUN(argc){
	If( argc <= 1 ) {
		x = argc;
	} else {
		x = FUN(argc-1) * argc;
	}
	return x;
}
write(argc);
