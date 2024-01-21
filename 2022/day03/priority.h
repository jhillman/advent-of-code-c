int priority(char item) {
	return item >= 'a' ? item - 'a' + 1 : item - 'A' + 27;
}