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
		fputs("  printf_test(", out);
		sscanf(buf, "%d \"%[^\"]\" \"%[^\"]\" %n", &num, expect, format,
		       &n);
		fprintf(out, "\"%d\", \"%s\", \"%s\"", num, expect, format);
		ptr = strtok(buf + n, " ");
		while (ptr != NULL)
		{
			rtrim(ptr);
			fprintf(out, ", %s", ptr);
			ptr = strtok(NULL, " ");
		}

		fputs(");\n", out);
	}

	fputs("}\n", out);

	fclose(in);
	fclose(out);

	return 0;
}
