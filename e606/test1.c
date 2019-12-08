/* Denna kommentar ska bort. */

int a = 3;

#define ABC 123

int b = ABC;

#define HI hi(3)

char c[] = "/* ska ABC inte ställa \" \"till med problem */";
char cc = '"'; /* qwewqe***/
char bb = '\''; /*** qwe*/

main()
{
	return HI;
}
