#include <ctype.h>
#include <stdio.h>
#include <string.h>

char buf[1000];

static void rtrim(char *s)
{
	s = s + strlen(s) - 1;
	while (isspace(*s))
	{
		*s = '\0';
		--s;
	}
}

int main(int argc, char **argv)
{
	char *ptr;
	int   num;
	int   n;
	char  expect[1000];
	char  format[1000];
	char  arg[1000];
	FILE *in;
	FILE *out;

	if (argc != 3)
		return 1;

	in  = fopen(argv[1], "r");
	out = fopen(argv[2], "w");

	if (!in || !out)
		return 1;

	fputs("#include \"test.h\"\n", out);
	fputs("void run_tests(void){\n", out);

	while (fgets(buf, 1000, in) != NULL)
	{
		if (buf[0] == '#' || buf[0] == '!' || buf[0] == '\n')
			continue;
		if (sscanf(buf, "%d \"%[^\"]\" \"%[^\"]\" %n", &num, expect,
			   format, &n) != 3)
			continue;
		fputs("  printf_test(", out);
		fprintf(out, "\"%d\", \"%s\", \"%s\"", num, expect, format);

		ptr = buf + n;
		for (; isspace(*ptr); ++ptr)
			;
		while (ptr != NULL)
		{
			if (sscanf(ptr, " \"%[^\"]\"%n ", arg, &n) > 0)
			{
				fprintf(out, ", \"%s\"", arg);
				ptr += n;
			}
			else if (sscanf(ptr, " '%[^']'%n ", arg, &n) > 0)
			{
				fprintf(out, ", '%s'", arg);
				ptr += n;
			}
			else if (sscanf(ptr, " %s%n ", arg, &n) > 0)
			{
				fprintf(out, ", %s", arg);
				ptr += n;
			}
			else
				ptr = NULL;
		}

		fputs(");\n", out);
	}

	fputs("}\n", out);

	fclose(in);
	fclose(out);

	return 0;
}
