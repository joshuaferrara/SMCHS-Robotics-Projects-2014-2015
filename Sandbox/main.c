
int main(void) {
	char* ret = "Y+127";
	printf("a");
	int mVal;
	printf("b");
	char* substr;
	printf("c");
	strncpy(substr, ret+1, strlen(ret));
	printf("d");
	mVal = atoi(substr);
	printf("e");

	return 0;
}
