void api_putchar(int c);
void api_end(void);

/**
 * 本来这段程序是异常程序，但是在模拟器下居然正常。在真机上是无法运作的
 **/
void HariMain(void)
{
	char a[100];
	a[10] = 'A';		/* 合法 */
	api_putchar(a[10]);
	a[102] = 'B';		/* 非法 */
	api_putchar(a[102]);
	a[123] = 'C';		/* 非法 */
	api_putchar(a[123]);
	api_end();
}
