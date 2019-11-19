/* /* Så får man inte göra! */ */
main (float a, double b)
{
	int *a = (int*)((int)a - (int)b);
	return *a;
}
